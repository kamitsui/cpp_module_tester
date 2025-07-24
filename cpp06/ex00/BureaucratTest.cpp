#include "Bureaucrat.hpp"
#include "gtest/gtest.h"
#include <sstream>

// Bureaucratのテストフィクスチャ（共通設定）
// 今回は単純なため空ですが、複雑なテストでは共通のオブジェクトなどをここで定義します。
class BureaucratTest : public ::testing::Test {
  protected:
    // virtual void SetUp() override {}
    // virtual void TearDown() override {}
};

// --- Test Case: Constructor ---
// Bureaucratオブジェクトの正常な生成と、不正な値での例外発生をテストします。
TEST_F(BureaucratTest, ConstructorAndGetters) {
    // 正常な値でのコンストラクタ呼び出し
    ASSERT_NO_THROW(Bureaucrat b("Zaphod", 42));
    Bureaucrat b("Arthur", 75);
    EXPECT_EQ(b.getName(), "Arthur");
    EXPECT_EQ(b.getGrade(), 75);

    // 境界値(1と150)でのテスト
    ASSERT_NO_THROW(Bureaucrat("Top", 1));
    ASSERT_NO_THROW(Bureaucrat("Bottom", 150));
}

// --- Test Case: Constructor Exceptions ---
// 不正なグレードで初期化しようとした際に例外がスローされるかテストします。
TEST_F(BureaucratTest, ConstructorExceptions) {
    // グレードが高すぎる場合 (0 < 1)
    EXPECT_THROW(Bureaucrat("Highflyer", 0), Bureaucrat::GradeTooHighException);

    // グレードが低すぎる場合 (151 > 150)
    EXPECT_THROW(Bureaucrat("Lowballer", 151), Bureaucrat::GradeTooLowException);
}

// --- Test Case: Grade Manipulation ---
// グレードのインクリメント・デクリメント機能をテストします。
TEST_F(BureaucratTest, GradeManipulation) {
    Bureaucrat b("Marvin", 50);

    // グレードをインクリメント (50 -> 49)
    b.incrementGrade();
    EXPECT_EQ(b.getGrade(), 49);

    // グレードをデクリメント (49 -> 50)
    b.decrementGrade();
    EXPECT_EQ(b.getGrade(), 50);
}

// --- Test Case: Grade Boundary Exceptions ---
// 限界グレード(1と150)での操作時に例外がスローされるかテストします。
TEST_F(BureaucratTest, GradeBoundaryExceptions) {
    Bureaucrat top("Top", 1);
    Bureaucrat bottom("Bottom", 150);

    // グレード1からのインクリメントはGradeTooHighExceptionをスローするはず
    EXPECT_THROW(top.incrementGrade(), Bureaucrat::GradeTooHighException);

    // グレード150からのデクリメントはGradeTooLowExceptionをスローするはず
    EXPECT_THROW(bottom.decrementGrade(), Bureaucrat::GradeTooLowException);
}

// --- Test Case: Orthodox Canonical Form ---
// コピーコンストラクタと代入演算子をテストします。
TEST_F(BureaucratTest, OrthodoxCanonicalForm) {
    Bureaucrat original("Original", 42);

    // コピーコンストラクタのテスト
    Bureaucrat copy(original);
    EXPECT_EQ(copy.getName(), "Original");
    EXPECT_EQ(copy.getGrade(), 42);

    // 代入演算子のテスト
    Bureaucrat assigned("Assigned", 100);
    assigned = original;
    // 名前(const)は変わらないはず
    EXPECT_EQ(assigned.getName(), "Assigned");
    // グレードはコピーされるはず
    EXPECT_EQ(assigned.getGrade(), 42);
}

// --- Test Case: Stream Insertion Operator ---
// << 演算子の出力形式が正しいかテストします。
TEST_F(BureaucratTest, StreamInsertionOperator) {
    Bureaucrat b("Trillian", 5);
    std::stringstream ss;
    ss << b;
    EXPECT_EQ(ss.str(), "Trillian, bureaucrat grade 5.");
}
