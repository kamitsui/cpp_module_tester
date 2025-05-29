#include "Animal.hpp" // Abstract Animal class
#include "Cat.hpp"    // Concrete derived class
#include "Dog.hpp"    // Concrete derived class
#include "gtest/gtest.h"
#include <sstream> // For capturing cout output

// テストフィクスチャ: 抽象クラスを直接テストできないため、主に具象派生クラスを介してテストします。
class AbstractAnimalTest : public ::testing::Test {
  protected:
    // 各テストの前に実行されるセットアップ
    void SetUp() override {}

    // 各テストの後に実行されるクリーンアップ
    void TearDown() override {}
};

// -----------------------------------------------------------
// 抽象クラスの特性のテスト
// -----------------------------------------------------------

// コンパイル時エラーとして抽象クラスの直接インスタンス化はGoogle Testではテストできない
// しかし、概念として重要なのでコメントで示します。
/*
TEST_F(AbstractAnimalTest, CannotInstantiateAbstractAnimal) {
    // 以下の行はコンパイルエラーになることを期待します。
    Animal myAbstractAnimal; // error: cannot declare variable 'myAbstractAnimal' to be of abstract type 'Animal'
    Animal *ptrAbstractAnimal = new Animal(); // error: invalid new-expression of abstract class type 'Animal'

    // Runtime test can't assert compile-time error. This is just for demonstration.
    // If you uncomment these lines, the test compilation will fail.
}
*/

// 抽象クラスの仮想デストラクタが適切に動作することの再確認
TEST_F(AbstractAnimalTest, PolymorphicDeletionCallsCorrectDestructors) {
    // std::cout の出力を捕捉し、デストラクタの呼び出し順序を確認します。
    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf(); // cout のrdbufを保存
    std::cout.rdbuf(ss.rdbuf());                 // cout をstringstreamにリダイレクト

    Animal *animalPtrDog = new Dog(); // DogオブジェクトをAnimalポインタで
    Animal *animalPtrCat = new Cat(); // CatオブジェクトをAnimalポインタで

    delete animalPtrDog; // 仮想デストラクタによりDog::~Dog() -> Animal::~Animal() が呼ばれる
    delete animalPtrCat; // 仮想デストラクタによりCat::~Cat() -> Animal::~Animal() が呼ばれる

    std::cout.rdbuf(oldCout); // cout を元に戻す

    // デストラクタメッセージはEX01でBrainが導入されているため、より多くの行が出力されます
    // ここでは、特定のメッセージが含まれていることを確認します。
    std::string output = ss.str();
    ASSERT_TRUE(output.find("Dog destructor called") != std::string::npos);
    ASSERT_TRUE(output.find("Cat destructor called") != std::string::npos);
    ASSERT_TRUE(output.find("Animal destructor called") != std::string::npos);
    ASSERT_TRUE(output.find("Brain destructor called") != std::string::npos); // EX01の機能
}

// -----------------------------------------------------------
// 具象クラスを介した抽象インターフェースのテスト
// -----------------------------------------------------------

// 具象クラスが getType() を正しく継承しているか
TEST_F(AbstractAnimalTest, GetTypeWorksThroughDerivedClasses) {
    Dog dog;
    Cat cat;
    ASSERT_EQ("Dog", dog.getType());
    ASSERT_EQ("Cat", cat.getType());
}

// 具象クラスが makeSound() を正しく実装しているか（ポリモーフィックな呼び出し）
TEST_F(AbstractAnimalTest, MakeSoundWorksPolymorphically) {
    Animal *dogPtr = new Dog();
    Animal *catPtr = new Cat();

    std::stringstream ss_dog;
    std::streambuf *oldCout = std::cout.rdbuf(); // cout のrdbufを保存
    std::cout.rdbuf(ss_dog.rdbuf());             // cout をstringstreamにリダイレクト

    dogPtr->makeSound(); // Dog::makeSound() が呼ばれるはず

    std::cout.rdbuf(oldCout); // cout を元に戻す
    ASSERT_EQ("Woof!\n", ss_dog.str());

    std::stringstream ss_cat;
    std::cout.rdbuf(ss_cat.rdbuf()); // cout をstringstreamにリダイレクト

    catPtr->makeSound(); // Cat::makeSound() が呼ばれるはず

    std::cout.rdbuf(oldCout); // cout を元に戻す
    ASSERT_EQ("Meow!\n", ss_cat.str());

    delete dogPtr;
    delete catPtr;
}

// EX01の機能も維持されているか（Brainの存在確認）
TEST_F(AbstractAnimalTest, DerivedClassesHaveBrain) {
    Dog dog;
    Cat cat;
    ASSERT_NE(nullptr, dog.getBrain());
    ASSERT_NE(nullptr, cat.getBrain());
}
