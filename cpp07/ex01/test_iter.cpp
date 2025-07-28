#include "iter.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <string>

// --- テスト用のヘルパー関数 ---

// 要素をインクリメントする関数 (非const)
// template <typename T> void incrementElement(T &element) { element++; } // iter.hppに記述

// 文字列に文字を追加する関数 (非const)
void appendChar(std::string &str) { str += '!'; }

// 要素を合計する関数 (const)
// テスト内で合計値を確認するために、static変数を使用
template <typename T> void sumElements(const T &element) {
    static T sum = T();
    // このテストでは実際の合計値は使わないが、
    // const参照を受け取る関数の例として定義
    (void)sum;
    sum += element;
}

// 何もしない関数 (const)
template <typename T> void doNothing(const T &element) { (void)element; }

// --- Google Testのテストケース ---

// 非const配列のテスト
TEST(IterNonConstTest, ModifiesIntArray) {
    int array[] = {1, 2, 3, 4, 5};
    size_t length = sizeof(array) / sizeof(array[0]);

    iter(array, length, incrementElement<int>);

    EXPECT_EQ(array[0], 2);
    EXPECT_EQ(array[1], 3);
    EXPECT_EQ(array[2], 4);
    EXPECT_EQ(array[3], 5);
    EXPECT_EQ(array[4], 6);
}

TEST(IterNonConstTest, ModifiesStringArray) {
    std::string array[] = {"a", "b", "c"};
    size_t length = sizeof(array) / sizeof(array[0]);

    iter(array, length, appendChar);

    EXPECT_EQ(array[0], "a!");
    EXPECT_EQ(array[1], "b!");
    EXPECT_EQ(array[2], "c!");
}

TEST(IterNonConstTest, HandlesEmptyArray) {
    int *array = NULL;
    // 空の配列に対してiterを実行してもクラッシュしないことを確認
    EXPECT_NO_THROW(iter(array, 0, incrementElement<int>));

    int empty_array[] = {};
    EXPECT_NO_THROW(iter(empty_array, 0, incrementElement<int>));
}

TEST(IterNonConstTest, HandlesSingleElementArray) {
    int array[] = {100};
    iter(array, 1, incrementElement<int>);
    EXPECT_EQ(array[0], 101);
}

// const配列のテスト
TEST(IterConstTest, ReadsConstIntArray) {
    const int array[] = {10, 20, 30};
    size_t length = sizeof(array) / sizeof(array[0]);

    // const配列に対して読み取り専用関数を渡せることを確認
    // クラッシュしなければOK
    EXPECT_NO_THROW(iter(array, length, doNothing<int>));
    EXPECT_NO_THROW(iter(array, length, sumElements<int>));
}

TEST(IterConstTest, ReadsConstDoubleArray) {
    const double array[] = {1.1, 2.2, 3.3};
    size_t length = sizeof(array) / sizeof(array[0]);

    EXPECT_NO_THROW(iter(array, length, doNothing<double>));
}

TEST(IterConstTest, HandlesEmptyConstArray) {
    const int *array = NULL;
    EXPECT_NO_THROW(iter(array, 0, doNothing<int>));

    const int empty_array[] = {};
    EXPECT_NO_THROW(iter(empty_array, 0, doNothing<int>));
}

// 以下のテストは、コンパイルエラーになることを確認するためのものです。
// コメントを解除すると、意図通りコンパイルに失敗します。
// これは`iter`がconstの安全性を正しく保証している証拠です。
/*
TEST(IterConstTest, FailsToCompileWithModifyingFunction) {
    const int array[] = {1, 2, 3};
    size_t length = sizeof(array) / sizeof(array[0]);

    // この行はコンパイルエラーになるべき
    // 理由：const配列（変更不可）を、値を変更する関数に渡そうとしているため
    iter(array, length, incrementElement<int>);
}
*/
