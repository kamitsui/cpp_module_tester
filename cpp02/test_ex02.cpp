#include "Fixed.hpp"
#include "gtest/gtest.h"
#include <cmath>
#include <limits>
#include <sstream>

// Helper function to compare floats with a small epsilon
bool floatEquals(float a, float b, float epsilon = 1e-5) {
    std::cout << "[[debug]] " << " | " << "a[" << a << "]" << " | " << "b[" << b << "]" << " | " << "epsilon["
              << epsilon << "]" << std::endl;
    std::cout << "abs(a - b) < epsilon" << " ... " << std::abs(a - b) << " < " << epsilon << " ... "
              << (std::abs(a - b) < epsilon) << std::endl;
    std::cout << "\t" << "0.00001" << " == " << 1e-5 << " ... [" << (0.00001f == epsilon) << "]" << std::endl;
    std::cout << "\t" << "FLT_EPSILON | " << FLT_EPSILON << std::endl;
    return std::abs(a - b) < epsilon;
}
// 固定小数点数の最小分解能を考慮した許容範囲
void fixedEquals(float expected, float actual) {
    int k = 1;
    float epsilon = (1.0f / (1 << 8)) * k; // fractionalBits は 8 と仮定
    ASSERT_NEAR(expected, actual, epsilon);
}

TEST(FixedTest, DefaultConstructor) {
    Fixed a;
    ASSERT_EQ(a.getRawBits(), 0);
}

// Constructor
TEST(FixedTest, IntConstructor) {
    Fixed b(10);
    ASSERT_EQ(b.getRawBits(), 10 << 8);
    ASSERT_FLOAT_EQ(b.toFloat(), 10.0f);
    ASSERT_EQ(b.toInt(), 10);
}

TEST(FixedTest, FloatConstructor) {
    Fixed c(42.42f);
    ASSERT_EQ(c.getRawBits(), static_cast<int>(roundf(42.42f * 256)));
    ASSERT_TRUE(floatEquals(c.toFloat(), 42.421875f)); // Due to fixed-point precision
    ASSERT_EQ(c.toInt(), 42);
}

TEST(FixedTest, CopyConstructor) {
    Fixed original(12.34f);
    Fixed copy(original);
    ASSERT_EQ(copy.getRawBits(), original.getRawBits());
    ASSERT_TRUE(floatEquals(copy.toFloat(), original.toFloat()));
    ASSERT_EQ(copy.toInt(), original.toInt());
}

TEST(FixedTest, CopyAssignmentOperator) {
    Fixed a(5.67f);
    Fixed b;
    b = a;
    ASSERT_EQ(b.getRawBits(), a.getRawBits());
    ASSERT_TRUE(floatEquals(b.toFloat(), a.toFloat()));
    ASSERT_EQ(b.toInt(), a.toInt());
}

// 比較演算子のテスト
TEST(FixedComparisonTest, GreaterThan) {
    Fixed a(5.0f);
    Fixed b(3.0f);
    Fixed c(5.0f);
    ASSERT_TRUE(a > b);
    ASSERT_FALSE(b > a);
    ASSERT_FALSE(a > a);
    ASSERT_FALSE(a > c);
}

TEST(FixedComparisonTest, LessThan) {
    Fixed a(3.0f);
    Fixed b(5.0f);
    Fixed c(3.0f);
    ASSERT_TRUE(a < b);
    ASSERT_FALSE(b < a);
    ASSERT_FALSE(a < a);
    ASSERT_FALSE(a < c);
}

TEST(FixedComparisonTest, GreaterThanOrEqual) {
    Fixed a(5.0f);
    Fixed b(3.0f);
    Fixed c(5.0f);
    ASSERT_TRUE(a >= b);
    ASSERT_TRUE(a >= c);
    ASSERT_FALSE(b >= a);
}

TEST(FixedComparisonTest, LessThanOrEqual) {
    Fixed a(3.0f);
    Fixed b(5.0f);
    Fixed c(3.0f);
    ASSERT_TRUE(a <= b);
    ASSERT_TRUE(a <= c);
    ASSERT_FALSE(b <= a);
}

TEST(FixedComparisonTest, EqualTo) {
    Fixed a(5.0f);
    Fixed b(5.0f);
    Fixed c(3.0f);
    ASSERT_TRUE(a == b);
    ASSERT_FALSE(a == c);
}

TEST(FixedComparisonTest, NotEqualTo) {
    Fixed a(5.0f);
    Fixed b(3.0f);
    Fixed c(5.0f);
    ASSERT_TRUE(a != b);
    ASSERT_FALSE(a != a);
    ASSERT_FALSE(a != c);
}

// 算術演算子のテスト
TEST(FixedArithmeticTest, Addition) {
    Fixed a(2.0f);
    Fixed b(3.0f);
    ASSERT_FLOAT_EQ((a + b).toFloat(), 5.0f);
    Fixed c(-1.5f);
    ASSERT_FLOAT_EQ((a + c).toFloat(), 0.5f);
}

TEST(FixedArithmeticTest, Addition2) {
    Fixed x(2.5f);
    Fixed y(3.7f);
    Fixed result = x + y;
    // 固定小数点数の最小分解能を考慮した許容範囲
    fixedEquals(result.toFloat(), 6.2f);
    // Bad test code
    // ASSERT_TRUE(floatEquals(result.toFloat(), 6.2f));
    // ASSERT_FLOAT_EQ(result.toFloat(), 6.2f);
}

TEST(FixedArithmeticTest, Subtraction) {
    Fixed a(5.0f);
    Fixed b(2.0f);
    ASSERT_FLOAT_EQ((a - b).toFloat(), 3.0f);
    Fixed c(-1.0f);
    ASSERT_FLOAT_EQ((a - c).toFloat(), 6.0f);

    Fixed x(5.9f);
    Fixed y(2.1f);
    Fixed result = x - y;
    fixedEquals(result.toFloat(), 3.8f);
    // Bad test code
    // ASSERT_TRUE(floatEquals(result.toFloat(), 3.8f));
}

TEST(FixedArithmeticTest, Multiplication) {
    Fixed a(2.0f);
    Fixed b(3.0f);
    ASSERT_FLOAT_EQ((a * b).toFloat(), 6.0f);
    Fixed c(-2.0f);
    ASSERT_FLOAT_EQ((a * c).toFloat(), -4.0f);

    Fixed x(2.0f);
    Fixed y(3.5f);
    Fixed result = x * y;
    ASSERT_TRUE(floatEquals(result.toFloat(), 7.0f));
}

TEST(FixedArithmeticTest, Division) {
    Fixed a(6.0f);
    Fixed b(3.0f);
    ASSERT_FLOAT_EQ((a / b).toFloat(), 2.0f);
    Fixed c(-2.0f);
    ASSERT_FLOAT_EQ((a / c).toFloat(), -3.0f);
    Fixed zero(0.0f);
    // Division by zeroはエラーメッセージを出力し、最大float値を返すはず
    ASSERT_FLOAT_EQ((a / zero).toFloat(), std::numeric_limits<float>::max());

    Fixed x(10.0f);
    Fixed y(2.5f);
    Fixed result = x / y;
    ASSERT_TRUE(floatEquals(result.toFloat(), 4.0f));
}

TEST(FixedArithmeticTest, DivisionByZero) {
    Fixed a(5.0f);
    Fixed b(0.0f);
    std::stringstream ss;
    std::streambuf *oldCerr = std::cerr.rdbuf();
    std::cerr.rdbuf(ss.rdbuf()); // Redirect cerr to stringstream

    Fixed result = a / b;
    ASSERT_TRUE(floatEquals(result.toFloat(), std::numeric_limits<float>::max()));
    ASSERT_NE(ss.str().find("Division by zero is undefined"), std::string::npos);

    std::cerr.rdbuf(oldCerr); // Restore cerr
}

// インクリメント/デクリメント演算子のテスト
TEST(FixedIncrementDecrementTest, PreIncrement) {
    Fixed a(2.0f);
    ASSERT_FLOAT_EQ((++a).toFloat(), 2.0f + (1.0f / 256.0f));
    ASSERT_FLOAT_EQ(a.toFloat(), 2.0f + (1.0f / 256.0f));

    Fixed b(1.5f);
    Fixed &result = ++b;
    ASSERT_EQ(b.toInt(), 2);
    ASSERT_EQ(result.toInt(), 2);
    ASSERT_EQ(&result, &b);
}

TEST(FixedIncrementDecrementTest, PostIncrement) {
    Fixed a(2.0f);
    ASSERT_FLOAT_EQ((a++).toFloat(), 2.0f);
    ASSERT_FLOAT_EQ(a.toFloat(), 2.0f + (1.0f / 256.0f));

    Fixed b(1.5f);
    Fixed result = b++;
    ASSERT_EQ(b.toInt(), 2);
    ASSERT_EQ(result.toInt(), 1);
    ASSERT_NE(&result, &b);
}

TEST(FixedIncrementDecrementTest, PreDecrement) {
    Fixed a(2.0f);
    ASSERT_FLOAT_EQ((--a).toFloat(), 2.0f - (1.0f / 256.0f));
    ASSERT_FLOAT_EQ(a.toFloat(), 2.0f - (1.0f / 256.0f));

    Fixed b(2.5f);
    Fixed &result = --b;
    ASSERT_EQ(b.toInt(), 2);
    ASSERT_EQ(result.toInt(), 2);
    ASSERT_EQ(&result, &b);
}

TEST(FixedIncrementDecrementTest, PostDecrement) {
    Fixed a(2.0f);
    ASSERT_FLOAT_EQ((a--).toFloat(), 2.0f);
    ASSERT_FLOAT_EQ(a.toFloat(), 2.0f - (1.0f / 256.0f));

    Fixed b(2.5f);
    Fixed result = b--;
    ASSERT_EQ(b.toInt(), 2);
    ASSERT_EQ(result.toInt(), 3); // Note: Post-decrement returns the value before decrement
    ASSERT_NE(&result, &b);
}

// Min/Max 関数のテスト
TEST(FixedMinMaxTest, MinNonConst) {
    Fixed a(5.0f);
    Fixed b(3.0f);
    Fixed &min_ref = Fixed::min(a, b);
    ASSERT_EQ(min_ref.toInt(), 3);
    ASSERT_FLOAT_EQ(min_ref.toFloat(), 3.0f);
    ASSERT_EQ(&min_ref, &b);

    min_ref = Fixed(1.0f);
    ASSERT_FLOAT_EQ(b.toFloat(), 1.0f); // min_ref は b を参照している
}

TEST(FixedMinMaxTest, MinConst) {
    Fixed const a(5.0f);
    Fixed const b(3.0f);
    Fixed const &min_ref = Fixed::min(a, b);
    ASSERT_EQ(min_ref.toInt(), 3);
    ASSERT_FLOAT_EQ(min_ref.toFloat(), 3.0f);
    ASSERT_EQ(&min_ref, &b);
}

TEST(FixedMinMaxTest, MaxNonConst) {
    Fixed a(5.0f);
    Fixed b(3.0f);
    Fixed &max_ref = Fixed::max(a, b);
    ASSERT_EQ(max_ref.toInt(), 5);
    ASSERT_FLOAT_EQ(max_ref.toFloat(), 5.0f);
    ASSERT_EQ(&max_ref, &a);

    max_ref = Fixed(10.0f);
    ASSERT_FLOAT_EQ(a.toFloat(), 10.0f); // max_ref は a を参照している
}

TEST(FixedMinMaxTest, MaxConst) {
    Fixed const a(5.0f);
    Fixed const b(3.0f);
    Fixed const &max_ref = Fixed::max(a, b);
    ASSERT_EQ(max_ref.toInt(), 5);
    ASSERT_FLOAT_EQ(max_ref.toFloat(), 5.0f);
    ASSERT_EQ(&max_ref, &a);
}

TEST(FixedOutputTest, OutputStreamOperator) {
    Fixed a(123.45f);
    std::stringstream ss;
    ss << a;
    ASSERT_EQ(ss.str(), "123.453125"); // Due to fixed-point precision
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
