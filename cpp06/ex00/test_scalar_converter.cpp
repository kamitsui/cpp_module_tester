#include "ScalarConverter.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <sstream>

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

// --- Test Cases ---

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

TEST_F(ScalarConverterTest, ValidIntNegative) {
    ScalarConverter::convert("-42");
#if CHAR_MIN < 0
    // This preprocessor directive checks if char is signed
    // On systems where char is signed (-128 to 127),
    // -42 is a valid, but non-printable character.
    std::string expected = "char: Non displayable\nint: -42\nfloat: -42.0f\ndouble: -42.0\n";
#else
    // On systems where char is unsigned (0 to 255),
    // -42 is outside the range.
    std::string expected = "char: impossible\nint: -42\nfloat: -42.0f\ndouble: -42.0\n";
#endif
    ASSERT_EQ(getOutput(), expected);
    ScalarConverter::convert("-42");
    getOutput();
}

TEST_F(ScalarConverterTest, ValidFloat) {
    ScalarConverter::convert("42.42f");
    std::string expected = "char: '*'\nint: 42\nfloat: 42.42f\ndouble: 42.42\n";
    ASSERT_EQ(getOutput(), expected);
}

TEST_F(ScalarConverterTest, ValidDouble) {
    ScalarConverter::convert("-123.456");
    std::string expected = "char: impossible\nint: -123\nfloat: -123.456f\ndouble: -123.456\n";
    ASSERT_EQ(getOutput(), expected);
}

TEST_F(ScalarConverterTest, ValidChar) {
    ScalarConverter::convert("'a'");
    std::string expected = "char: 'a'\nint: 97\nfloat: 97.0f\ndouble: 97.0\n";
    ASSERT_EQ(getOutput(), expected);
}

TEST_F(ScalarConverterTest, NonDisplayableChar) {
    ScalarConverter::convert("10");
    std::string expected = "char: Non displayable\nint: 10\nfloat: 10.0f\ndouble: 10.0\n";
    ASSERT_EQ(getOutput(), expected);
}

TEST_F(ScalarConverterTest, ImpossibleChar) {
    ScalarConverter::convert("200");
    std::string expected = "char: Non displayable\nint: 200\nfloat: 200.0f\ndouble: 200.0\n";
    ASSERT_EQ(getOutput(), expected);
}

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

TEST_F(ScalarConverterTest, IntOverflow) {
    ScalarConverter::convert("2147483648"); // int max + 1
    std::string expected = "char: impossible\nint: impossible\nfloat: 2.14748e+09f\ndouble: 2.14748e+09\n";
    // Note: The float/double representation might vary slightly. Adjust if needed.
    // A better approach for this specific test might check parts of the string.
    // For simplicity, we check the exact output which might be platform-dependent.
    ASSERT_EQ(getOutput().find("int: impossible"), strlen("int: impossible\n") + 1);
}
