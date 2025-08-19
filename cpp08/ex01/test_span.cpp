#include "Span.hpp"
#include "gtest/gtest.h"
#include <cstdlib> // For rand()
#include <ctime>   // For time()
#include <vector>

// --- Test Fixture ---
// 各テストで共通して使うオブジェクトを準備
class SpanTest : public ::testing::Test {
  protected:
    Span sp5;  // Capacity 5
    Span sp10; // Capacity 10

    SpanTest() : sp5(5), sp10(10) {}

    void SetUp() override {
        // sp5に初期データを設定
        sp5.addNumber(6);
        sp5.addNumber(3);
        sp5.addNumber(17);
        sp5.addNumber(9);
        sp5.addNumber(11);
    }
};

// --- Basic Functionality Tests ---

TEST_F(SpanTest, ShortestSpanNormal) {
    // 11 - 9 = 2
    ASSERT_EQ(sp5.shortestSpan(), 2);
}

TEST_F(SpanTest, LongestSpanNormal) {
    // 17 - 3 = 14
    ASSERT_EQ(sp5.longestSpan(), 14);
}

TEST_F(SpanTest, ShortestSpanWithDuplicates) {
    Span sp(5);
    sp.addNumber(10);
    sp.addNumber(20);
    sp.addNumber(10); // Duplicate
    sp.addNumber(30);
    // 10 - 10 = 0
    ASSERT_EQ(sp.shortestSpan(), 0);
}

TEST_F(SpanTest, LongestSpanWithNegativeNumbers) {
    Span sp(4);
    sp.addNumber(-10);
    sp.addNumber(5);
    sp.addNumber(-1);
    sp.addNumber(20);
    // 20 - (-10) = 30
    ASSERT_EQ(sp.longestSpan(), 30);
}

// --- Exception Tests ---

TEST_F(SpanTest, ThrowWhenAddingNumberToFullSpan) {
    // sp5 is already full
    ASSERT_THROW(sp5.addNumber(100), Span::SpanFullException);
}

TEST_F(SpanTest, ThrowWhenFindingSpanWithNoNumbers) {
    Span sp(5);
    ASSERT_THROW(sp.shortestSpan(), Span::NotEnoughNumbersException);
    ASSERT_THROW(sp.longestSpan(), Span::NotEnoughNumbersException);
}

TEST_F(SpanTest, ThrowWhenFindingSpanWithOneNumber) {
    Span sp(5);
    sp.addNumber(42);
    ASSERT_THROW(sp.shortestSpan(), Span::NotEnoughNumbersException);
    ASSERT_THROW(sp.longestSpan(), Span::NotEnoughNumbersException);
}

// --- Range AddNumber Tests ---

TEST_F(SpanTest, AddNumberRangeSuccessfully) {
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    sp10.addNumber(v.begin(), v.end()); // Add 3 numbers
    sp10.addNumber(100);
    sp10.addNumber(200);

    // 2 - 1 = 1
    ASSERT_EQ(sp10.shortestSpan(), 1);
    // 200 - 1 = 199
    ASSERT_EQ(sp10.longestSpan(), 199);
}

TEST_F(SpanTest, ThrowWhenAddingRangeThatExceedsCapacity) {
    Span sp(5);
    sp.addNumber(1);
    sp.addNumber(2);
    sp.addNumber(3); // 3 elements stored, 2 remaining capacity

    std::vector<int> v;
    v.push_back(4);
    v.push_back(5);
    v.push_back(6); // This range has 3 elements, which is too many

    ASSERT_THROW(sp.addNumber(v.begin(), v.end()), Span::SpanFullException);
}

// --- Large Scale Test ---

TEST(SpanLargeScaleTest, HandlesLargeNumberOfElements) {
    const unsigned int numElements = 20000;
    Span sp(numElements);

    std::vector<int> largeVector;
    largeVector.reserve(numElements);
    srand(time(0));
    for (unsigned int i = 0; i < numElements; ++i) {
        largeVector.push_back(rand());
    }

    // No exception should be thrown here
    ASSERT_NO_THROW(sp.addNumber(largeVector.begin(), largeVector.end()));

    // Check if span functions work without crashing (values will vary)
    ASSERT_NO_THROW(sp.shortestSpan());
    ASSERT_NO_THROW(sp.longestSpan());
}

// --- Copy Semantics Test ---

TEST_F(SpanTest, CopyConstructor) {
    Span sp_copy(sp5);

    // Both should have the same spans
    ASSERT_EQ(sp_copy.shortestSpan(), sp5.shortestSpan());
    ASSERT_EQ(sp_copy.longestSpan(), sp5.longestSpan());

    // Modifying the copy should not affect the original
    // sp_copy.addNumber(100); // This should throw as it's full
    ASSERT_THROW(sp_copy.addNumber(100), Span::SpanFullException);

    // The original should still be fine
    ASSERT_EQ(sp5.shortestSpan(), 2);
}

TEST_F(SpanTest, AssignmentOperator) {
    Span sp_assigned(2); // Different capacity
    sp_assigned = sp5;

    ASSERT_EQ(sp_assigned.shortestSpan(), sp5.shortestSpan());
    ASSERT_EQ(sp_assigned.longestSpan(), sp5.longestSpan());

    // Self-assignment test
    sp_assigned = sp_assigned;
    ASSERT_EQ(sp_assigned.shortestSpan(), 2);
}
