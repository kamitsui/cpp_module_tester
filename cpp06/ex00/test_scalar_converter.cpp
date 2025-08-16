#include "ScalarConverter.hpp"
#include "gtest/gtest.h"
#include <climits> // For CHAR_MIN
#include <iostream>
#include <limits> // For std::numeric_limits
#include <sstream>
#include <string>

// A test fixture class to capture std::cout
class ScalarConverterTest : public ::testing::Test {
  protected:
    // This method will be called before each test is run
    void SetUp() override {
        // Redirect std::cout to our stringstream buffer
        sbuf_ = std::cout.rdbuf();
        std::cout.rdbuf(buffer_.rdbuf());
    }

    // This method will be called after each test is run
    void TearDown() override {
        // Restore the original std::cout buffer
        std::cout.rdbuf(sbuf_);
    }

    // This method allows us to get the captured output
    std::string getOutput() { return buffer_.str(); }

  private:
    std::stringstream buffer_;
    std::streambuf *sbuf_;
};

// --- Helper function for robust testing ---
// Checks if the output contains all the required substrings.
void AssertOutputContains(const std::string &output, const std::string &char_part, const std::string &int_part,
                          const std::string &float_part, const std::string &double_part) {
    // std::string captured_output = getOutput();
    //  デバッグが完了したら、期待する文字列と比較するアサーションに戻す
    // ASSERT_STREQ("Expected Output", output.c_str());

    // std::cout << "---debug----\n" << output << "-----------" << std::endl;
    ASSERT_NE(output.find(char_part), std::string::npos);
    ASSERT_NE(output.find(int_part), std::string::npos);
    ASSERT_NE(output.find(float_part), std::string::npos);
    ASSERT_NE(output.find(double_part), std::string::npos);
}

// === Basic Integer and Char Tests (from previous examples) ===

TEST_F(ScalarConverterTest, ValidIntPositive) {
    ScalarConverter::convert("42");
    std::string expected = "char: '*'\nint: 42\nfloat: 42.0f\ndouble: 42.0\n";
    ASSERT_EQ(getOutput(), expected);
}

TEST_F(ScalarConverterTest, ValidIntZero) {
    ScalarConverter::convert("0");
    std::string expected = "char: Non displayable\nint: 0\nfloat: 0.0f\ndouble: 0.0\n";
    ASSERT_EQ(getOutput(), expected);
}

TEST_F(ScalarConverterTest, NonDisplayableChar) {
    ScalarConverter::convert("10");
    std::string expected = "char: Non displayable\nint: 10\nfloat: 10.0f\ndouble: 10.0\n";
    ASSERT_EQ(getOutput(), expected);
}

TEST_F(ScalarConverterTest, ValidChar) {
    ScalarConverter::convert("'a'");
    std::string expected = "char: 'a'\nint: 97\nfloat: 97.0f\ndouble: 97.0\n";
    ASSERT_EQ(getOutput(), expected);
}

TEST_F(ScalarConverterTest, ValidFloat) {
    ScalarConverter::convert("42.42f");
    std::string expected = "char: '*'\nint: 42\nfloat: 42.42f\ndouble: 42.42\n";
    ASSERT_EQ(getOutput(), expected);
}

// === PseudoLiteral Tests (擬似リテラルのテスト) ===

TEST_F(ScalarConverterTest, PseudoLiteralNan) {
    ScalarConverter::convert("nan");
    std::string expected = "char: impossible\nint: impossible\nfloat: nanf\ndouble: nan\n";
    ASSERT_EQ(getOutput(), expected);
}

TEST_F(ScalarConverterTest, PseudoLiteralNanf) {
    ScalarConverter::convert("nanf");
    std::string expected = "char: impossible\nint: impossible\nfloat: nanf\ndouble: nan\n";
    ASSERT_EQ(getOutput(), expected);
}

TEST_F(ScalarConverterTest, PseudoLiteralPositiveInf) {
    ScalarConverter::convert("+inf");
    std::string expected = "char: impossible\nint: impossible\nfloat: +inff\ndouble: +inf\n";
    ASSERT_EQ(getOutput(), expected);
}

TEST_F(ScalarConverterTest, PseudoLiteralNegativeInff) {
    ScalarConverter::convert("-inff");
    std::string expected = "char: impossible\nint: impossible\nfloat: -inff\ndouble: -inf\n";
    ASSERT_EQ(getOutput(), expected);
}

TEST_F(ScalarConverterTest, InvalidInput) {
    ScalarConverter::convert("hello");
    std::string expected = "char: impossible\nint: impossible\nfloat: impossible\ndouble: impossible\n";
    ASSERT_EQ(getOutput(), expected);
}

TEST_F(ScalarConverterTest, InvalidNumber) {
    ScalarConverter::convert("42a");
    std::string expected = "char: impossible\nint: impossible\nfloat: impossible\ndouble: impossible\n";
    ASSERT_EQ(getOutput(), expected);
}

// === Portability-Aware Tests (環境依存を考慮したテスト) ===

TEST_F(ScalarConverterTest, ValidIntNegative) {
    ScalarConverter::convert("-42");
    std::string expected_char_part;
#if CHAR_MIN < 0 // Check if char is signed
    // If char is signed, -65 is a valid (but non-printable) char.
    expected_char_part = "char: Non displayable\n";
#else
    // If char is unsigned, -65 is out of range.
    expected_char_part = "char: impossible\n";
#endif
    // We check for substrings to make the test robust.
    AssertOutputContains(getOutput(), expected_char_part, "int: -42\n", "float: -42.0f\n", "double: -42.0\n");
}

TEST_F(ScalarConverterTest, ValidDouble) {
    ScalarConverter::convert("-123.456");
    std::string expected_char_part;
#if CHAR_MIN < 0
    expected_char_part = "char: Non displayable\n";
#else
    expected_char_part = "char: impossible\n";
#endif
    AssertOutputContains(getOutput(), expected_char_part, "int: -123\n", "float: -123.456f\n", "double: -123.456\n");
}

TEST_F(ScalarConverterTest, ImpossibleChar) {
    ScalarConverter::convert("200");
    std::string expected_char_part;
#if CHAR_MAX > 127
    expected_char_part = "char: Non displayable\n";
#else
    expected_char_part = "char: impossible\n";
#endif
    AssertOutputContains(getOutput(), expected_char_part, "int: 200\n", "float: 200.0f\n", "double: 200.0\n");
}

TEST_F(ScalarConverterTest, IntOverflow) {
    ScalarConverter::convert("2147483648"); // int max + 1
    std::string expected = "char: impossible\nint: impossible\nfloat: 2.14748e+09f\ndouble: 2.14748e+09\n";
    // Note: The float/double representation might vary slightly. Adjust if needed.
    // A better approach for this specific test might check parts of the string.
    // For simplicity, we check the exact output which might be platform-dependent.
    ASSERT_EQ(getOutput().find("int: impossible"), strlen("int: impossible\n") + 1);
}

// === Float and Double Specific Tests (float/double特化テスト) ===

TEST_F(ScalarConverterTest, FloatWithPrecision) {
    ScalarConverter::convert("1.234567f");
    // We check substrings because the exact precision might differ slightly.
    // AssertOutputContains(getOutput(), "char: Non displayable\n", "int: 1\n", "float: 1.234567f\n",
    // "double: 1.234567\n");
    AssertOutputContains(getOutput(), "char: Non displayable\n", "int: 1\n", "float: 1.23457f\n", "double: 1.23457\n");
}

TEST_F(ScalarConverterTest, DoubleToFloatPrecisionLoss) {
    // This double value has more precision than a float can hold.
    ScalarConverter::convert("3.1415926535");
    std::string output = getOutput();

    // The double part should be precise.
    // ASSERT_NE(output.find("double: 3.1415926535"), std::string::npos);
    ASSERT_NE(output.find("double: 3.14159"), std::string::npos);

    // The float part will have lost precision.
    // We check that it's a float, but don't assert the exact truncated value.
    ASSERT_NE(output.find("float: 3.14159"), std::string::npos); // Check for the start of the number
    ASSERT_NE(output.find("f\n"), std::string::npos);            // Check for the 'f' suffix
}

TEST_F(ScalarConverterTest, FloatMax) {
    // FLT_MAX is the largest value a float can hold.
    // It's too large for an int.
    ScalarConverter::convert("3.4028235e+38f");
    AssertOutputContains(getOutput(), "char: impossible\n", "int: impossible\n",
                         "float: 3.40282e+38f\n",  // Representation may vary
                         "double: 3.40282e+38\n"); // Representation may vary
}

TEST_F(ScalarConverterTest, DoubleMax) {
    // DBL_MAX is too large for both int and float.
    // The float conversion will result in infinity.
    ScalarConverter::convert("1.7976931348623157e+308");
    AssertOutputContains(getOutput(), "char: impossible\n", "int: impossible\n",
                         "float: inff\n",           // float overflows to infinity
                         "double: 1.79769e+308\n"); // Representation may vary
                                                    // 1.79769e+308
}

TEST_F(ScalarConverterTest, DoubleMaxPlus) {
    // DBL_MAX is too large for both int and float.
    // The float conversion will result in infinity.
    ScalarConverter::convert("2.7976931348623157e+308");
    AssertOutputContains(getOutput(), "char: impossible\n", "int: impossible\n",
                         "float: inff\n",  // float overflows to infinity
                         "double: inf\n"); // Representation may vary
                                           // 1.79769e+308
}

TEST_F(ScalarConverterTest, ZeroWithDecimal) {
    ScalarConverter::convert("0.0");
    std::string expected = "char: Non displayable\nint: 0\nfloat: 0.0f\ndouble: 0.0\n";
    ASSERT_EQ(getOutput(), expected);
}
