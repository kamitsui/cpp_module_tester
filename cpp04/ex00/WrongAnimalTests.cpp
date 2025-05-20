#include "WrongAnimal.hpp"
#include "WrongCat.hpp" // WrongAnimalポインタでWrongCatをテストするために必要
#include "gtest/gtest.h"
#include <sstream>

class WrongAnimalTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(WrongAnimalTest, DefaultConstructor) {
    WrongAnimal animal;
    ASSERT_EQ("Generic WrongAnimal", animal.getType());
}

TEST_F(WrongAnimalTest, CopyConstructor) {
    WrongAnimal original;
    WrongAnimal copied = original;
    ASSERT_EQ(original.getType(), copied.getType());
}

TEST_F(WrongAnimalTest, CopyAssignmentOperator) {
    WrongAnimal original;
    WrongAnimal assigned;
    assigned = original;
    ASSERT_EQ(original.getType(), assigned.getType());
}

TEST_F(WrongAnimalTest, MakeSoundOutput) {
    WrongAnimal animal;
    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    animal.makeSound();

    std::cout.rdbuf(oldCout);

    ASSERT_EQ("Wrong generic animal sound\n", ss.str());
}

// 非ポリモーフィックなmakeSound()のテスト
TEST_F(WrongAnimalTest, NonPolymorphicMakeSound) {
    WrongAnimal *wrongCatPtr = new WrongCat(); // WrongAnimalポインタでWrongCatオブジェクトを指す
    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    wrongCatPtr->makeSound(); // ここがポイント！WrongAnimal::makeSound()が呼ばれるはず

    std::cout.rdbuf(oldCout);
    delete wrongCatPtr; // メモリリーク防止

    ASSERT_EQ("Wrong generic animal sound\n", ss.str()); // WrongCatの音ではない
}
