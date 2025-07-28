#include "Array.hpp"
#include <gtest/gtest.h>
#include <string>
#include <vector>

// --- Default Constructor Tests ---
TEST(ArrayTest, DefaultConstructor) {
    Array<int> a;
    EXPECT_EQ(a.size(), 0U);
}

TEST(ArrayTest, DefaultConstructorThrowsOnAccess) {
    Array<double> a;
    EXPECT_THROW(a[0], Array<double>::OutOfBoundsException);
}

// --- Unsigned Int Constructor Tests ---
TEST(ArrayTest, UIntConstructor) {
    Array<int> a(10);
    EXPECT_EQ(a.size(), 10U);

    // Elements should be default-initialized (value-initialized)
    for (unsigned int i = 0; i < a.size(); ++i) {
        EXPECT_EQ(a[i], 0);
    }
}

TEST(ArrayTest, UIntConstructorWithZero) {
    Array<int> a(0);
    EXPECT_EQ(a.size(), 0U);
}

TEST(ArrayTest, UIntConstructorWithString) {
    Array<std::string> a(5);
    EXPECT_EQ(a.size(), 5U);
    for (unsigned int i = 0; i < a.size(); ++i) {
        EXPECT_EQ(a[i], "");
    }
}

// --- Copy Constructor Tests ---
TEST(ArrayTest, CopyConstructor) {
    Array<int> original(5);
    for (unsigned int i = 0; i < original.size(); ++i) {
        original[i] = i * 10;
    }

    Array<int> copy(original);

    // Check size and content
    ASSERT_EQ(copy.size(), original.size());
    for (unsigned int i = 0; i < copy.size(); ++i) {
        EXPECT_EQ(copy[i], original[i]);
    }

    // Check for deep copy
    copy[0] = 999;
    EXPECT_NE(copy[0], original[0]);
    EXPECT_EQ(original[0], 0);
}

TEST(ArrayTest, CopyConstructorFromEmpty) {
    Array<int> empty_original;
    Array<int> copy(empty_original);
    EXPECT_EQ(copy.size(), 0U);
}

// --- Assignment Operator Tests ---
// Using a Test Fixture for more complex setup
class AssignmentTest : public ::testing::Test {
  protected:
    Array<int> a1;         // Empty
    Array<int> a2;         // Has 5 elements
    Array<std::string> s1; // Has 3 string elements

    AssignmentTest() : a2(5), s1(3) {
        for (unsigned int i = 0; i < a2.size(); ++i) {
            a2[i] = i + 1;
        }
        s1[0] = "A";
        s1[1] = "B";
        s1[2] = "C";
    }
};

TEST_F(AssignmentTest, AssignToEmpty) {
    a1 = a2;
    ASSERT_EQ(a1.size(), a2.size());
    for (unsigned int i = 0; i < a1.size(); ++i) {
        EXPECT_EQ(a1[i], a2[i]);
    }

    // Check deep copy
    a1[0] = 100;
    EXPECT_NE(a1[0], a2[0]);
}

TEST_F(AssignmentTest, AssignFromEmpty) {
    a2 = a1;
    EXPECT_EQ(a2.size(), 0U);
}

TEST_F(AssignmentTest, AssignToExisting) {
    Array<int> a3(10);
    a3 = a2; // Assign smaller to larger
    ASSERT_EQ(a3.size(), a2.size());
    for (unsigned int i = 0; i < a3.size(); ++i) {
        EXPECT_EQ(a3[i], a2[i]);
    }
}

TEST_F(AssignmentTest, SelfAssignment) {
    a2 = a2;
    ASSERT_EQ(a2.size(), 5U);
    for (unsigned int i = 0; i < a2.size(); ++i) {
        EXPECT_EQ(a2[i], static_cast<int>(i + 1));
    }
}

TEST_F(AssignmentTest, ChainedAssignment) {
    Array<int> a3, a4;
    a4 = a3 = a2;
    ASSERT_EQ(a3.size(), 5U);
    ASSERT_EQ(a4.size(), 5U);
    EXPECT_EQ(a3[0], 1);
    EXPECT_EQ(a4[0], 1);
}

// --- Subscript Operator and Bounds Tests ---
TEST(ArrayTest, SubscriptOperator) {
    Array<int> a(3);
    a[0] = 10;
    a[1] = 20;
    a[2] = 30;
    EXPECT_EQ(a[0], 10);
    EXPECT_EQ(a[1], 20);
    EXPECT_EQ(a[2], 30);
}

TEST(ArrayTest, SubscriptOperatorConst) {
    Array<int> a(3);
    a[0] = 10;
    a[1] = 20;
    a[2] = 30;

    const Array<int> const_a(a);
    EXPECT_EQ(const_a[0], 10);
    EXPECT_EQ(const_a[1], 20);
    EXPECT_EQ(const_a[2], 30);
    // const_a[0] = 100; // This would fail to compile, which is correct.
}

TEST(ArrayTest, ThrowsOnAccessAboveBounds) {
    Array<int> a(10);
    EXPECT_THROW(a[10] = 1, Array<int>::OutOfBoundsException);
    EXPECT_THROW(a[10], Array<int>::OutOfBoundsException);
}

TEST(ArrayTest, ThrowsOnAccessLargeIndex) {
    Array<int> a(10);
    EXPECT_THROW(a[100] = 1, Array<int>::OutOfBoundsException);
}

TEST(ArrayTest, ThrowsOnNegativeIndex) {
    // Note: -1 will be converted to a large unsigned int
    Array<int> a(10);
    EXPECT_THROW(a[-1] = 1, Array<int>::OutOfBoundsException);
}
