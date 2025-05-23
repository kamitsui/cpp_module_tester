#include "Animal.hpp" // For polymorphic tests
#include "Brain.hpp"  // To access Brain content
#include "Dog.hpp"
#include "gtest/gtest.h"
#include <sstream>

class DogTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(DogTest, DefaultConstructorInitializesBrain) {
    Dog dog;
    ASSERT_NE(nullptr, dog.getBrain()); // Ensure Brain pointer is not null
    ASSERT_EQ("Dog", dog.getType());
    ASSERT_EQ("Thinking...", dog.getBrain()->getIdea(0)); // Check Brain default init
}

TEST_F(DogTest, CopyConstructorDeepCopyBrain) {
    Dog originalDog;
    originalDog.getBrain()->setIdea(0, "Dog original idea");

    Dog copiedDog = originalDog; // Call copy constructor

    // Ensure the Brain pointers are different (deep copy)
    ASSERT_NE(originalDog.getBrain(), copiedDog.getBrain());

    // Ensure contents are initially the same
    ASSERT_EQ(originalDog.getBrain()->getIdea(0), copiedDog.getBrain()->getIdea(0));

    // Modify the copied Brain and check if original's Brain remains unchanged
    copiedDog.getBrain()->setIdea(0, "Dog copied idea modified");
    ASSERT_EQ("Dog original idea", originalDog.getBrain()->getIdea(0));
    ASSERT_EQ("Dog copied idea modified", copiedDog.getBrain()->getIdea(0));
}

TEST_F(DogTest, CopyAssignmentOperatorDeepCopyBrain) {
    Dog originalDog;
    originalDog.getBrain()->setIdea(0, "Dog original idea assign");

    Dog assignedDog; // Has its own brain initially
    assignedDog.getBrain()->setIdea(0, "Dog assigned initial idea");

    assignedDog = originalDog; // Call copy assignment operator

    // Ensure the Brain pointers are different (deep copy)
    ASSERT_NE(originalDog.getBrain(), assignedDog.getBrain());

    // Ensure contents are now the same
    ASSERT_EQ(originalDog.getBrain()->getIdea(0), assignedDog.getBrain()->getIdea(0));

    // Modify the assigned Brain and check if original's Brain remains unchanged
    assignedDog.getBrain()->setIdea(0, "Dog assigned idea modified");
    ASSERT_EQ("Dog original idea assign", originalDog.getBrain()->getIdea(0));
    ASSERT_EQ("Dog assigned idea modified", assignedDog.getBrain()->getIdea(0));
}

TEST_F(DogTest, MakeSoundOutput) {
    Dog dog;
    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    dog.makeSound();

    std::cout.rdbuf(oldCout);

    ASSERT_EQ("Woof!\n", ss.str());
}

TEST_F(DogTest, PolymorphicMakeSound) {
    Animal *dogPtr = new Dog();
    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    dogPtr->makeSound();

    std::cout.rdbuf(oldCout);
    delete dogPtr;

    ASSERT_EQ("Woof!\n", ss.str());
}
