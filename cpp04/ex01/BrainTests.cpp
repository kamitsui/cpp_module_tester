#include "Brain.hpp"
#include "gtest/gtest.h"
#include <sstream> // For capturing cout output

class BrainTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // No specific setup needed for most Brain tests
    }
    void TearDown() override {
        // No specific teardown needed
    }
};

TEST_F(BrainTest, DefaultConstructorInitializesIdeas) {
    Brain brain;
    for (int i = 0; i < 100; ++i) {
        ASSERT_EQ("Thinking...", brain.getIdea(i));
    }
}

TEST_F(BrainTest, SetAndGetIdea) {
    Brain brain;
    brain.setIdea(0, "First idea");
    brain.setIdea(50, "Middle idea");
    brain.setIdea(99, "Last idea");

    ASSERT_EQ("First idea", brain.getIdea(0));
    ASSERT_EQ("Middle idea", brain.getIdea(50));
    ASSERT_EQ("Last idea", brain.getIdea(99));
}

TEST_F(BrainTest, SetIdeaOutOfBounds) {
    Brain brain;
    brain.setIdea(-1, "Invalid");
    brain.setIdea(100, "Invalid");

    // Ensure setting out of bounds doesn't change anything or crash
    ASSERT_EQ("", brain.getIdea(-1));           // Should return empty string or default
    ASSERT_EQ("", brain.getIdea(100));          // Should return empty string or default
    ASSERT_EQ("Thinking...", brain.getIdea(0)); // Should remain default
}

TEST_F(BrainTest, CopyConstructorDeepCopy) {
    Brain original;
    original.setIdea(0, "Original idea 1");
    original.setIdea(1, "Original idea 2");

    Brain copied = original; // Call copy constructor

    // Verify initial copy is correct
    ASSERT_EQ(original.getIdea(0), copied.getIdea(0));
    ASSERT_EQ(original.getIdea(1), copied.getIdea(1));

    // Modify the copy and ensure original remains unchanged (deep copy)
    copied.setIdea(0, "Modified idea 1");
    copied.setIdea(1, "Modified idea 2");

    ASSERT_EQ("Original idea 1", original.getIdea(0));
    ASSERT_EQ("Original idea 2", original.getIdea(1));
    ASSERT_EQ("Modified idea 1", copied.getIdea(0));
    ASSERT_EQ("Modified idea 2", copied.getIdea(1));
}

TEST_F(BrainTest, CopyAssignmentOperatorDeepCopy) {
    Brain original;
    original.setIdea(0, "Original idea A");
    original.setIdea(1, "Original idea B");

    Brain assigned;
    assigned.setIdea(0, "Initial idea C"); // Different initial state
    assigned = original;                   // Call copy assignment operator

    // Verify initial assignment is correct
    ASSERT_EQ(original.getIdea(0), assigned.getIdea(0));
    ASSERT_EQ(original.getIdea(1), assigned.getIdea(1));

    // Modify the assigned object and ensure original remains unchanged (deep copy)
    assigned.setIdea(0, "Modified idea A");
    assigned.setIdea(1, "Modified idea B");

    ASSERT_EQ("Original idea A", original.getIdea(0));
    ASSERT_EQ("Original idea B", original.getIdea(1));
    ASSERT_EQ("Modified idea A", assigned.getIdea(0));
    ASSERT_EQ("Modified idea B", assigned.getIdea(1));
}
