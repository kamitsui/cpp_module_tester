#include "MutantStack.hpp"
#include "gtest/gtest.h"
#include <algorithm> // For std::equal
#include <list>
#include <vector>

// --- 1. StackFunctionalityTest ---
// 継承したstd::stackの基本機能が壊れていないかを確認
TEST(StackFunctionalityTest, PushAndTop) {
    MutantStack<int> mstack;
    mstack.push(1);
    EXPECT_EQ(mstack.top(), 1);
    mstack.push(2);
    EXPECT_EQ(mstack.top(), 2);
    EXPECT_EQ(mstack.size(), 2ul);
}

TEST(StackFunctionalityTest, Pop) {
    MutantStack<int> mstack;
    mstack.push(1);
    mstack.push(2);
    mstack.pop();
    EXPECT_EQ(mstack.top(), 1);
    EXPECT_EQ(mstack.size(), 1ul);
}

TEST(StackFunctionalityTest, EmptyAndSize) {
    MutantStack<int> mstack;
    EXPECT_TRUE(mstack.empty());
    EXPECT_EQ(mstack.size(), 0ul);
    mstack.push(42);
    EXPECT_FALSE(mstack.empty());
    EXPECT_EQ(mstack.size(), 1ul);
    mstack.pop();
    EXPECT_TRUE(mstack.empty());
    EXPECT_EQ(mstack.size(), 0ul);
}

// --- 2. IteratorTest ---
// 核心機能であるイテレータを徹底的にテスト
class IteratorTest : public ::testing::Test {
  protected:
    void SetUp() override {
        mstack.push(10);
        mstack.push(20);
        mstack.push(30);
    }
    MutantStack<int> mstack;
};

TEST_F(IteratorTest, ForwardIteration) {
    std::vector<int> expected;
    expected.push_back(10);
    expected.push_back(20);
    expected.push_back(30);

    int i = 0;
    for (MutantStack<int>::iterator it = mstack.begin(); it != mstack.end(); ++it) {
        EXPECT_EQ(*it, expected[i++]);
    }
}

TEST_F(IteratorTest, ReverseIteration) {
    std::vector<int> expected;
    expected.push_back(30);
    expected.push_back(20);
    expected.push_back(10);

    int i = 0;
    for (MutantStack<int>::reverse_iterator it = mstack.rbegin(); it != mstack.rend(); ++it) {
        EXPECT_EQ(*it, expected[i++]);
    }
}

TEST_F(IteratorTest, ConstIteration) {
    const MutantStack<int> &const_mstack = mstack;
    std::vector<int> expected;
    expected.push_back(10);
    expected.push_back(20);
    expected.push_back(30);

    int i = 0;
    // const_iteratorが使われる
    for (MutantStack<int>::const_iterator it = const_mstack.begin(); it != const_mstack.end(); ++it) {
        EXPECT_EQ(*it, expected[i++]);
    }
}

TEST_F(IteratorTest, ModifyThroughIterator) {
    MutantStack<int>::iterator it = mstack.begin();
    *it = 99; // Change the first element (bottom of the stack)
    EXPECT_EQ(mstack.begin()[0], 99);
}

TEST(EmptyIteratorTest, BeginEqualsEnd) {
    MutantStack<int> empty_stack;
    EXPECT_EQ(empty_stack.begin(), empty_stack.end());
}

// --- 3. CopySemanticsTest ---
// クラスが安全にコピー・代入できるかを確認
class CopySemanticsTest : public ::testing::Test {
  protected:
    void SetUp() override {
        original.push(1);
        original.push(2);
        original.push(3);
    }
    MutantStack<int> original;
};

TEST_F(CopySemanticsTest, CopyConstructor) {
    MutantStack<int> copy(original);

    ASSERT_EQ(original.size(), copy.size());
    EXPECT_TRUE(std::equal(original.begin(), original.end(), copy.begin()));

    // コピーを変更してもオリジナルに影響がないことを確認 (ディープコピー)
    copy.pop();
    EXPECT_NE(original.size(), copy.size());
    EXPECT_EQ(original.top(), 3);
}

TEST_F(CopySemanticsTest, AssignmentOperator) {
    MutantStack<int> assigned;
    assigned.push(99);
    assigned = original;

    ASSERT_EQ(original.size(), assigned.size());
    EXPECT_TRUE(std::equal(original.begin(), original.end(), assigned.begin()));

    // 自己代入テスト ... clangのコンパイラは自己代入を防ぐため、このテストは不要。
    // assigned = assigned; // gnu のコンパイラは通るが、clang より
    // ASSERT_EQ(original.size(), assigned.size());
    // EXPECT_TRUE(std::equal(original.begin(), original.end(), assigned.begin()));
}

// --- 4. CompatibilityTest ---
// std::listとの互換性を証明
TEST(CompatibilityTest, IsEquivalentToList) {
    MutantStack<int> mstack;
    std::list<int> list;

    for (int i = 0; i < 5; ++i) {
        mstack.push(i);
        list.push_back(i);
    }

    ASSERT_EQ(mstack.size(), list.size());
    EXPECT_TRUE(std::equal(mstack.begin(), mstack.end(), list.begin()));
}
