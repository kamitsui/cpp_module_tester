#include "whatever.hpp"
#include "gtest/gtest.h"
#include <string>

// --- swap 関数のテスト ---

TEST(SwapTest, Integers) {
    int a = 10;
    int b = 20;
    int original_a = a;
    int original_b = b;
    ::swap(a, b);
    EXPECT_EQ(a, original_b);
    EXPECT_EQ(b, original_a);
}

TEST(SwapTest, Floats) {
    float a = 1.23f;
    float b = 4.56f;
    float original_a = a;
    float original_b = b;
    ::swap(a, b);
    EXPECT_FLOAT_EQ(a, original_b);
    EXPECT_FLOAT_EQ(b, original_a);
}

TEST(SwapTest, Strings) {
    std::string a = "Hello";
    std::string b = "World";
    std::string original_a = a;
    std::string original_b = b;
    ::swap(a, b);
    EXPECT_EQ(a, original_b);
    EXPECT_EQ(b, original_a);
}

// --- min 関数のテスト ---

TEST(MinTest, IntsPositive) {
    int a = 5;
    int b = 10;
    EXPECT_EQ(::min(a, b), 5);
    EXPECT_EQ(::min(b, a), 5);
}

TEST(MinTest, IntsNegative) {
    int a = -5;
    int b = -10;
    EXPECT_EQ(::min(a, b), -10);
    EXPECT_EQ(::min(b, a), -10);
}

TEST(MinTest, IntsMixedSign) {
    int a = -5;
    int b = 5;
    EXPECT_EQ(::min(a, b), -5);
    EXPECT_EQ(::min(b, a), -5);
}

TEST(MinTest, IntsEqual) {
    int a = 7;
    int b = 7;
    // 仕様:「等しい場合は2番目の値を返す」
    // これを検証するため、返された値のアドレスが
    // 2番目の引数のアドレスと一致するかを確認します。
    EXPECT_EQ(&::min(a, b), &b);
}

TEST(MinTest, Strings) {
    std::string a = "chaine1";
    std::string b = "chaine2";
    EXPECT_EQ(::min(a, b), "chaine1");
    EXPECT_EQ(::min(b, a), "chaine1");
}

TEST(MinTest, StringsEqual) {
    std::string a = "equal";
    std::string b = "equal";
    EXPECT_EQ(&::min(a, b), &b);
}

// --- max 関数のテスト ---

TEST(MaxTest, IntsPositive) {
    int a = 5;
    int b = 10;
    EXPECT_EQ(::max(a, b), 10);
    EXPECT_EQ(::max(b, a), 10);
}

TEST(MaxTest, IntsNegative) {
    int a = -5;
    int b = -10;
    EXPECT_EQ(::max(a, b), -5);
    EXPECT_EQ(::max(b, a), -5);
}

TEST(MaxTest, IntsMixedSign) {
    int a = -5;
    int b = 5;
    EXPECT_EQ(::max(a, b), 5);
    EXPECT_EQ(::max(b, a), 5);
}

TEST(MaxTest, IntsEqual) {
    int a = 7;
    int b = 7;
    // 仕様:「等しい場合は2番目の値を返す」
    // min と同様にアドレスを比較して検証します。
    EXPECT_EQ(&::max(a, b), &b);
}

TEST(MaxTest, Strings) {
    std::string a = "chaine1";
    std::string b = "chaine2";
    EXPECT_EQ(::max(a, b), "chaine2");
    EXPECT_EQ(::max(b, a), "chaine2");
}

TEST(MaxTest, StringsEqual) {
    std::string a = "equal";
    std::string b = "equal";
    EXPECT_EQ(&::max(a, b), &b);
}
