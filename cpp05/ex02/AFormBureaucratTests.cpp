#include "AForm.hpp"
#include "Bureaucrat.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "gtest/gtest.h"
#include <cstdio>  // For remove()
#include <fstream> // For file checking

// テスト用のフィクスチャクラス
class AFormExecutionTest : public ::testing::Test {
  protected:
    Bureaucrat *pres;       // Grade 1
    Bureaucrat *midManager; // Grade 40
    Bureaucrat *lowIntern;  // Grade 140

    void SetUp() override {
        pres = new Bureaucrat("President Zaphod", 1);
        midManager = new Bureaucrat("Mid-Manager Ford", 40);
        lowIntern = new Bureaucrat("Intern Arthur", 140);
        // RobotomyRequestFormで使うrand()のシードを設定
        srand(time(NULL));
    }

    void TearDown() override {
        delete pres;
        delete midManager;
        delete lowIntern;
        // テストで生成された可能性のあるファイルを削除
        remove("shrub_test_target_shrubbery");
    }

    // ファイルが存在するか確認するヘルパー関数
    bool fileExists(const std::string &filename) {
        std::ifstream f(filename.c_str());
        return f.good();
    }
};

// --- Test Case: Execution Precondition Checks ---
// フォーム実行の前提条件（署名済みか、権限があるか）をテストします。
TEST_F(AFormExecutionTest, ExecutePreconditionExceptions) {
    ShrubberyCreationForm form("garden");

    // 失敗ケース1: 未署名のフォームを実行しようとする
    EXPECT_THROW(form.execute(*pres), AForm::FormNotSignedException);

    // フォームに署名する
    form.beSigned(*pres);

    // 失敗ケース2: 権限のないBureaucratが実行しようとする (lowIntern:140 > exec:137)
    EXPECT_THROW(form.execute(*lowIntern), AForm::GradeTooLowException);
}

// --- Test Case: ShrubberyCreationForm Successful Execution ---
// ShrubberyCreationFormが正常に実行され、ファイルが作成されることをテストします。
TEST_F(AFormExecutionTest, ShrubberyExecutionSuccess) {
    ShrubberyCreationForm form("shrub_test_target");
    const std::string filename = "shrub_test_target_shrubbery";

    // 事前確認：ファイルが存在しないこと
    ASSERT_FALSE(fileExists(filename));

    form.beSigned(*pres);
    // 実行権限があるBureaucrat (midManager:40 < exec:137)
    ASSERT_NO_THROW(form.execute(*midManager));

    // 事後確認：ファイルが作成されていること
    ASSERT_TRUE(fileExists(filename));
}

// --- Test Case: RobotomyRequestForm Successful Execution ---
// RobotomyRequestFormが正常に実行されることをテストします。
TEST_F(AFormExecutionTest, RobotomyExecutionSuccess) {
    RobotomyRequestForm form("Bender");

    // 署名 (pres:1 < sign:72)
    form.beSigned(*pres);
    // 実行 (midManager:40 < exec:45)
    ASSERT_NO_THROW(form.execute(*midManager));
}

// --- Test Case: PresidentialPardonForm Successful Execution ---
// PresidentialPardonFormが正常に実行されることをテストします。
TEST_F(AFormExecutionTest, PresidentialPardonExecutionSuccess) {
    PresidentialPardonForm form("Marvin");

    // 署名 (pres:1 < sign:25)
    form.beSigned(*pres);
    // 実行 (pres:1 < exec:5)
    ASSERT_NO_THROW(form.execute(*pres));
}

// --- Test Case: Bureaucrat::executeForm() Interaction ---
// BureaucratのexecuteFormメソッドが正しくフォームの実行をトリガーするかテストします。
TEST_F(AFormExecutionTest, BureaucratExecutesForm) {
    PresidentialPardonForm form("Trillian");

    // 失敗ケース：未署名のため実行できない
    // Bureaucrat::executeFormは例外をキャッチしてメッセージを出すので、ここでは例外は発生しない
    ASSERT_NO_THROW(pres->executeForm(form));

    // 署名して成功ケースをテスト
    pres->signForm(form);
    ASSERT_TRUE(form.getIsSigned());
    ASSERT_NO_THROW(pres->executeForm(form));
}
