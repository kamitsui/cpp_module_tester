#include "Animal.hpp" // For polymorphic tests
#include "Brain.hpp"  // To access Brain content
#include "Cat.hpp"
#include "gtest/gtest.h"
#include <sstream>

class CatTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(CatTest, DefaultConstructorInitializesBrain) {
    Cat cat;
    ASSERT_NE(nullptr, cat.getBrain()); // Ensure Brain pointer is not null
    ASSERT_EQ("Cat", cat.getType());
    ASSERT_EQ("Thinking...", cat.getBrain()->getIdea(0)); // Check Brain default init
}

TEST_F(CatTest, CopyConstructorDeepCopyBrain) {
    Cat originalCat;
    originalCat.getBrain()->setIdea(0, "Cat original idea");

    Cat copiedCat = originalCat; // Call copy constructor

    // Ensure the Brain pointers are different (deep copy)
    ASSERT_NE(originalCat.getBrain(), copiedCat.getBrain());

    // Ensure contents are initially the same
    ASSERT_EQ(originalCat.getBrain()->getIdea(0), copiedCat.getBrain()->getIdea(0));

    // Modify the copied Brain and check if original's Brain remains unchanged
    copiedCat.getBrain()->setIdea(0, "Cat copied idea modified");
    ASSERT_EQ("Cat original idea", originalCat.getBrain()->getIdea(0));
    ASSERT_EQ("Cat copied idea modified", copiedCat.getBrain()->getIdea(0));
}

TEST_F(CatTest, CopyAssignmentOperatorDeepCopyBrain) {
    Cat originalCat;
    originalCat.getBrain()->setIdea(0, "Cat original idea assign");

    Cat assignedCat; // Has its own brain initially
    assignedCat.getBrain()->setIdea(0, "Cat assigned initial idea");

    assignedCat = originalCat; // Call copy assignment operator

    // Ensure the Brain pointers are different (deep copy)
    ASSERT_NE(originalCat.getBrain(), assignedCat.getBrain());

    // Ensure contents are now the same
    ASSERT_EQ(originalCat.getBrain()->getIdea(0), assignedCat.getBrain()->getIdea(0));

    // Modify the assigned Brain and check if original's Brain remains unchanged
    assignedCat.getBrain()->setIdea(0, "Cat assigned idea modified");
    ASSERT_EQ("Cat original idea assign", originalCat.getBrain()->getIdea(0));
    ASSERT_EQ("Cat assigned idea modified", assignedCat.getBrain()->getIdea(0));
}

TEST_F(CatTest, MakeSoundOutput) {
    Cat cat;
    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    cat.makeSound();

    std::cout.rdbuf(oldCout);

    ASSERT_EQ("Meow!\n", ss.str());
}

TEST_F(CatTest, PolymorphicMakeSound) {
    Animal *catPtr = new Cat();
    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    catPtr->makeSound();

    std::cout.rdbuf(oldCout);
    delete catPtr;

    ASSERT_EQ("Meow!\n", ss.str());
}
