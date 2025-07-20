#include "AForm.hpp"
#include "Bureaucrat.hpp"
#include "Intern.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "gtest/gtest.h"

// Internクラスのテスト用フィクスチャ
class InternTest : public ::testing::Test {
  protected:
    Intern *intern;

    void SetUp() override { intern = new Intern(); }

    void TearDown() override { delete intern; }
};

// --- Test Case: Successful Form Creation ---
// Internが3種類のフォームを正しく生成できるかテストします。
TEST_F(InternTest, SuccessfulFormCreation) {
    AForm *form = NULL;

    // 1. ShrubberyCreationFormのテスト
    ASSERT_NO_THROW(form = intern->makeForm("shrubbery creation", "garden"));
    // 生成されたオブジェクトがNULLでないことを確認
    ASSERT_NE(form, (AForm *)NULL);
    // 正しい型かdynamic_castで確認
    EXPECT_NE(dynamic_cast<ShrubberyCreationForm *>(form), (ShrubberyCreationForm *)NULL);
    EXPECT_EQ(form->getTarget(), "garden");
    delete form;

    // 2. RobotomyRequestFormのテスト
    ASSERT_NO_THROW(form = intern->makeForm("robotomy request", "Bender"));
    ASSERT_NE(form, (AForm *)NULL);
    EXPECT_NE(dynamic_cast<RobotomyRequestForm *>(form), (RobotomyRequestForm *)NULL);
    EXPECT_EQ(form->getTarget(), "Bender");
    delete form;

    // 3. PresidentialPardonFormのテスト
    ASSERT_NO_THROW(form = intern->makeForm("presidential pardon", "Ford"));
    ASSERT_NE(form, (AForm *)NULL);
    EXPECT_NE(dynamic_cast<PresidentialPardonForm *>(form), (PresidentialPardonForm *)NULL);
    EXPECT_EQ(form->getTarget(), "Ford");
    delete form;
}

// --- Test Case: Unknown Form Exception ---
// 存在しないフォーム名を指定した際に、適切な例外をスローするかテストします。
TEST_F(InternTest, ThrowsExceptionOnUnknownForm) {
    EXPECT_THROW(intern->makeForm("unknown form type", "target"), Intern::UnknownFormException);
    EXPECT_THROW(intern->makeForm("lunch request", "kitchen"), Intern::UnknownFormException);
    EXPECT_THROW(intern->makeForm("", "empty"), Intern::UnknownFormException); // 空文字列のケース
}

// --- Test Case: Case Sensitivity ---
// フォーム名の大文字・小文字が区別されることをテストします（要件通りなら失敗するはず）。
TEST_F(InternTest, IsCaseSensitive) {
    // 例えば、"Robotomy Request" のように大文字が含まれていると失敗するはず
    EXPECT_THROW(intern->makeForm("Robotomy Request", "CaseTest"), Intern::UnknownFormException);
}
