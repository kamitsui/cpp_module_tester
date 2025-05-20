#include "Dog.hpp"
#include "gtest/gtest.h"
#include <sstream>

class DogTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(DogTest, DefaultConstructor) {
    Dog dog;
    ASSERT_EQ("Dog", dog.getType());
}

TEST_F(DogTest, CopyConstructor) {
    Dog original;
    Dog copied = original;
    ASSERT_EQ(original.getType(), copied.getType());
}

TEST_F(DogTest, CopyAssignmentOperator) {
    Dog original;
    Dog assigned;
    assigned = original;
    ASSERT_EQ(original.getType(), assigned.getType());
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
    Animal *dogPtr = new Dog(); // AnimalポインタでDogオブジェクトを指す
    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    dogPtr->makeSound(); // ポリモーフィック呼び出し

    std::cout.rdbuf(oldCout);
    delete dogPtr; // メモリリーク防止

    ASSERT_EQ("Woof!\n", ss.str());
}
