#include "Form.hpp"
#include "Bureaucrat.hpp"
#include "gtest/gtest.h"

// FormとBureaucratのテストのためのフィクスチャ
// テスト全体で使う共通のオブジェクトをここで定義します。
class FormBureaucratTest : public ::testing::Test {
  protected:
    Bureaucrat *highGradeBureaucrat;
    Bureaucrat *midGradeBureaucrat;
    Bureaucrat *lowGradeBureaucrat;

    // 各テストケースの実行前に呼ばれるセットアップ関数
    void SetUp() override {
        highGradeBureaucrat = new Bureaucrat("Zaphod", 1);
        midGradeBureaucrat = new Bureaucrat("Ford", 50);
        lowGradeBureaucrat = new Bureaucrat("Arthur", 150);
    }

    // 各テストケースの実行後に呼ばれるクリーンアップ関数
    void TearDown() override {
        delete highGradeBureaucrat;
        delete midGradeBureaucrat;
        delete lowGradeBureaucrat;
    }
};

// --- Test Case: Form Constructor ---
// Formオブジェクトの正常な生成と、不正な値での例外発生をテストします。
TEST_F(FormBureaucratTest, FormConstructorAndGetters) {
    ASSERT_NO_THROW(Form f("A38", 50, 25));
    Form f("B42", 75, 45);

    EXPECT_EQ(f.getName(), "B42");
    EXPECT_EQ(f.getGradeToSign(), 75);
    EXPECT_EQ(f.getGradeToExecute(), 45);
    EXPECT_FALSE(f.getIsSigned()); // 初期状態は未署名のはず
}

TEST_F(FormBureaucratTest, FormConstructorExceptions) {
    // 署名グレードが高すぎる (0 < 1)
    EXPECT_THROW(Form("InvalidHigh", 0, 50), Form::GradeTooHighException);
    // 実行グレードが高すぎる (0 < 1)
    EXPECT_THROW(Form("InvalidHigh", 50, 0), Form::GradeTooHighException);

    // 署名グレードが低すぎる (151 > 150)
    EXPECT_THROW(Form("InvalidLow", 151, 50), Form::GradeTooLowException);
    // 実行グレードが低すぎる (151 > 150)
    EXPECT_THROW(Form("InvalidLow", 50, 151), Form::GradeTooLowException);
}

// --- Test Case: Form::beSigned() method ---
// FormのbeSignedメソッドが正しく動作するかをテストします。
TEST_F(FormBureaucratTest, BeSignedFunctionality) {
    Form formToSign("C73", 50, 25);

    // 成功ケース: グレードが高いBureaucrat (1 < 50)
    ASSERT_NO_THROW(formToSign.beSigned(*highGradeBureaucrat));
    EXPECT_TRUE(formToSign.getIsSigned());

    // 既に署名済みのフォームにもう一度署名を試みても、状態は変わらない
    ASSERT_NO_THROW(formToSign.beSigned(*highGradeBureaucrat));
    EXPECT_TRUE(formToSign.getIsSigned());

    // 失敗ケース: グレードが低いBureaucrat (150 > 50)
    Form formToFail("D88", 50, 25);
    EXPECT_THROW(formToFail.beSigned(*lowGradeBureaucrat), Form::GradeTooLowException);
    EXPECT_FALSE(formToFail.getIsSigned());

    // 境界値ケース: グレードがちょうど同じBureaucrat (50 == 50)
    Form formOnEdge("E99", 50, 25);
    ASSERT_NO_THROW(formOnEdge.beSigned(*midGradeBureaucrat));
    EXPECT_TRUE(formOnEdge.getIsSigned());
}

// --- Test Case: Bureaucrat::signForm() Interaction ---
// BureaucratがFormに署名を試みる際の相互作用をテストします。
TEST_F(FormBureaucratTest, BureaucratSignsFormInteraction) {
    Form form("F-alpha", 40, 20);

    // 成功ケース: midGradeBureaucrat (50) は署名できないはず
    midGradeBureaucrat->signForm(form);
    EXPECT_FALSE(form.getIsSigned()); // Grade 50 > 40 なので失敗

    // 成功ケース: highGradeBureaucrat (1) は署名できるはず
    highGradeBureaucrat->signForm(form);
    EXPECT_TRUE(form.getIsSigned());

    // 失敗ケース: lowGradeBureaucrat (150) は署名できない
    Form anotherForm("G-beta", 100, 50);
    lowGradeBureaucrat->signForm(anotherForm);
    EXPECT_FALSE(anotherForm.getIsSigned());
}

// --- Test Case: Form Orthodox Canonical Form ---
// Formクラスのコピーコンストラクタと代入演算子をテストします。
TEST_F(FormBureaucratTest, FormOrthodoxCanonicalForm) {
    Form original("Original", 50, 50);
    original.beSigned(*midGradeBureaucrat); // originalを署名済みにする

    // コピーコンストラクタのテスト
    Form copy(original);
    EXPECT_EQ(copy.getName(), "Original");
    EXPECT_EQ(copy.getGradeToSign(), 50);
    EXPECT_TRUE(copy.getIsSigned()); // isSignedの状態がコピーされるか

    // 代入演算子のテスト
    Form assigned("Assigned", 100, 100);
    EXPECT_FALSE(assigned.getIsSigned());
    assigned = original;
    // constメンバーは変わらない
    EXPECT_EQ(assigned.getName(), "Assigned");
    EXPECT_EQ(assigned.getGradeToSign(), 100);
    // isSignedの状態はコピーされる
    EXPECT_TRUE(assigned.getIsSigned());
}
