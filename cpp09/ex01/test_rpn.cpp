#include "RPN.hpp"
#include "gtest/gtest.h"
#include <sstream>
#include <streambuf>

// --- Helper Function ---
// RPN::evaluateの標準出力をキャプチャし、文字列として返す
std::string getRPNOutput(const std::string &expression) {
    std::stringstream buffer;
    std::streambuf *old_cout = std::cout.rdbuf(buffer.rdbuf()); // coutの出力先をbufferに変更

    try {
        RPN rpn;
        rpn.evaluate(expression);
    } catch (const std::exception &e) {
        // 例外が発生した場合、テストはASSERT_THROWで別途キャッチするため、ここでは何もしない
    }

    std::cout.rdbuf(old_cout); // coutの出力先を元に戻す

    // 末尾の改行を削除して比較しやすくする
    std::string output = buffer.str();
    if (!output.empty() && output[output.length() - 1] == '\n') {
        output.erase(output.length() - 1);
    }
    return output;
}

// --- Normal Calculation Tests ---

TEST(RPNTest, SimpleAddition) { EXPECT_EQ(getRPNOutput("1 2 +"), "3"); }

TEST(RPNTest, SimpleSubtraction) { EXPECT_EQ(getRPNOutput("9 5 -"), "4"); }

TEST(RPNTest, SimpleMultiplication) { EXPECT_EQ(getRPNOutput("8 9 *"), "72"); }

TEST(RPNTest, SimpleDivision) { EXPECT_EQ(getRPNOutput("8 4 /"), "2"); }

TEST(RPNTest, SubjectExample1) { EXPECT_EQ(getRPNOutput("8 9 * 9 - 9 - 9 - 4 - 1 +"), "42"); }

TEST(RPNTest, SubjectExample2) { EXPECT_EQ(getRPNOutput("7 7 * 7 -"), "42"); }

TEST(RPNTest, SubjectExample3) { EXPECT_EQ(getRPNOutput("1 2 * 2 / 2 * 2 4 - +"), "0"); }

TEST(RPNTest, NegativeResult) { EXPECT_EQ(getRPNOutput("3 8 -"), "-5"); }

TEST(RPNTest, SingleNumber) { EXPECT_EQ(getRPNOutput("5"), "5"); }

// --- Error Handling Tests ---

TEST(RPNErrorTest, ThrowsOnOperandShortage) {
    RPN rpn;
    ASSERT_THROW(rpn.evaluate("1 +"), std::exception);
    // 2回目の呼び出しのために新しいインスタンスを作成
    RPN rpn2;
    ASSERT_THROW(rpn2.evaluate("8 9 * *"), std::exception);
}

TEST(RPNErrorTest, ThrowsOnOperandSurplus) {
    RPN rpn;
    ASSERT_THROW(rpn.evaluate("1 2 3 +"), std::exception);
    RPN rpn2;
    ASSERT_THROW(rpn2.evaluate("5 5"), std::exception);
}

TEST(RPNErrorTest, ThrowsOnDivisionByZero) {
    RPN rpn;
    ASSERT_THROW(rpn.evaluate("6 0 /"), std::exception);
}

TEST(RPNErrorTest, ThrowsOnInvalidToken) {
    RPN rpn;
    ASSERT_THROW(rpn.evaluate("1 a +"), std::exception);
    RPN rpn2;
    ASSERT_THROW(rpn2.evaluate("1 . 2 +"), std::exception);
    RPN rpn3;
    ASSERT_THROW(rpn3.evaluate("10 2 +"), std::exception); // 10は要件外
}

TEST(RPNErrorTest, ThrowsOnParentheses) {
    RPN rpn;
    ASSERT_THROW(rpn.evaluate("(1 + 1)"), std::exception);
}

TEST(RPNErrorTest, ThrowsOnEmptyInput) {
    // 空の入力はmain関数で弾かれるが、クラスがどう振る舞うかもテストする
    // この実装では、スタックが空のままループが終了し、最終チェックで例外がスローされる
    RPN rpn;
    ASSERT_THROW(rpn.evaluate(""), std::exception);
}
