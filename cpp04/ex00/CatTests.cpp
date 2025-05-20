#include "Cat.hpp"
#include "gtest/gtest.h"
#include <sstream>

class CatTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(CatTest, DefaultConstructor) {
    Cat cat;
    ASSERT_EQ("Cat", cat.getType());
}

TEST_F(CatTest, CopyConstructor) {
    Cat original;
    Cat copied = original;
    ASSERT_EQ(original.getType(), copied.getType());
}

TEST_F(CatTest, CopyAssignmentOperator) {
    Cat original;
    Cat assigned;
    assigned = original;
    ASSERT_EQ(original.getType(), assigned.getType());
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
    Animal *catPtr = new Cat(); // AnimalポインタでCatオブジェクトを指す
    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    catPtr->makeSound(); // ポリモーフィック呼び出し

    std::cout.rdbuf(oldCout);
    delete catPtr; // メモリリーク防止

    ASSERT_EQ("Meow!\n", ss.str());
}
