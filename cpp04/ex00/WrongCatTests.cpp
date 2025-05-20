#include "WrongCat.hpp"
#include "gtest/gtest.h"
#include <sstream>

class WrongCatTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(WrongCatTest, DefaultConstructor) {
    WrongCat cat;
    ASSERT_EQ("WrongCat", cat.getType());
}

TEST_F(WrongCatTest, CopyConstructor) {
    WrongCat original;
    WrongCat copied = original;
    ASSERT_EQ(original.getType(), copied.getType());
}

TEST_F(WrongCatTest, CopyAssignmentOperator) {
    WrongCat original;
    WrongCat assigned;
    assigned = original;
    ASSERT_EQ(original.getType(), assigned.getType());
}

// 直接WrongCatオブジェクトに対してmakeSound()を呼び出す場合
TEST_F(WrongCatTest, MakeSoundOutputDirectCall) {
    WrongCat cat;
    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    cat.makeSound(); // 直接呼び出す場合はWrongCatのmakeSound()が呼ばれる

    std::cout.rdbuf(oldCout);

    ASSERT_EQ("Wrong meow!\n", ss.str());
}
