#include "easyfind.hpp"
#include "gtest/gtest.h"
#include <list>
#include <vector>

// --- Test Fixture for std::vector ---
// テスト間で共通のセットアップを行うためのクラス
class VectorTest : public ::testing::Test {
  protected:
    // 各テストケースの開始前に呼ばれる
    void SetUp() override {
        vec.push_back(10);
        vec.push_back(20);
        vec.push_back(30);
        vec.push_back(40);
        vec.push_back(50);

        vec_with_duplicates.push_back(5);
        vec_with_duplicates.push_back(10);
        vec_with_duplicates.push_back(5);
        vec_with_duplicates.push_back(15);

        vec_with_negatives.push_back(-1);
        vec_with_negatives.push_back(-100);
        vec_with_negatives.push_back(0);
    }

    std::vector<int> vec;
    std::vector<int> empty_vec;
    std::vector<int> vec_with_duplicates;
    std::vector<int> vec_with_negatives;
};

// --- Tests for std::vector ---

TEST_F(VectorTest, FindValueInTheMiddle) {
    std::vector<int>::iterator it = easyfind(vec, 30);
    // 返されたイテレータが指す値が30であることを確認
    ASSERT_EQ(*it, 30);
    // 返されたイテレータがコンテナの末尾でないことを確認
    ASSERT_NE(it, vec.end());
}

TEST_F(VectorTest, FindValueAtTheBeginning) {
    std::vector<int>::iterator it = easyfind(vec, 10);
    ASSERT_EQ(*it, 10);
    // 返されたイテレータがコンテナの先頭と一致することを確認
    ASSERT_EQ(it, vec.begin());
}

TEST_F(VectorTest, FindValueAtTheEnd) {
    std::vector<int>::iterator it = easyfind(vec, 50);
    ASSERT_EQ(*it, 50);
    // 返されたイテレータの次がコンテナの末尾であることを確認
    ASSERT_EQ(++it, vec.end());
}

TEST_F(VectorTest, ThrowExceptionWhenValueNotFound) {
    // easyfind(vec, 99) を実行した際に NotFoundException がスローされることを期待
    ASSERT_THROW(easyfind(vec, 99), NotFoundException);
}

TEST_F(VectorTest, ThrowExceptionOnEmptyVector) { ASSERT_THROW(easyfind(empty_vec, 1), NotFoundException); }

TEST_F(VectorTest, FindFirstOccurrenceInDuplicates) {
    std::vector<int>::iterator it = easyfind(vec_with_duplicates, 5);
    ASSERT_EQ(*it, 5);
    // 見つかったのが最初の5であることを確認（イテレータがコンテナの先頭と一致）
    ASSERT_EQ(it, vec_with_duplicates.begin());
}

TEST_F(VectorTest, FindNegativeValue) {
    std::vector<int>::iterator it = easyfind(vec_with_negatives, -100);
    ASSERT_EQ(*it, -100);
}

// --- Test Fixture for std::list ---
class ListTest : public ::testing::Test {
  protected:
    void SetUp() override {
        lst.push_back(11);
        lst.push_back(22);
        lst.push_back(33);
    }
    std::list<int> lst;
    std::list<int> empty_lst;
};

// --- Tests for std::list ---

TEST_F(ListTest, FindValueSuccessfully) {
    std::list<int>::iterator it = easyfind(lst, 22);
    ASSERT_EQ(*it, 22);
    ASSERT_NE(it, lst.end());
}

TEST_F(ListTest, ThrowExceptionWhenValueNotFound) { ASSERT_THROW(easyfind(lst, 99), NotFoundException); }

TEST_F(ListTest, ThrowExceptionOnEmptyList) { ASSERT_THROW(easyfind(empty_lst, 1), NotFoundException); }

// メイン関数はGoogle Testが提供してくれるので不要
