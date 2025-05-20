#include "Animal.hpp"
#include "gtest/gtest.h"
#include <sstream> // For capturing cout output

// テストフィクスチャ: テスト間で共通のセットアップが必要な場合に使用
class AnimalTest : public ::testing::Test {
  protected:
    // 各テストの前に実行されるセットアップ
    void SetUp() override {
        // コンストラクタ/デストラクタメッセージを捕捉するためのストリーム
        // 出力はテストの成功/失敗には影響しないため、ここでは捕捉しない
        // もしメッセージ内容をテストしたい場合は、stringstreamを使う
    }

    // 各テストの後に実行されるクリーンアップ
    void TearDown() override {}
};

TEST_F(AnimalTest, DefaultConstructor) {
    Animal animal;
    ASSERT_EQ("Generic Animal", animal.getType());
}

TEST_F(AnimalTest, CopyConstructor) {
    Animal original;
    Animal copied = original;
    ASSERT_EQ(original.getType(), copied.getType());
    // 深いコピーではないため、ポインタ比較は意味がない
    // 今回は文字列なので値が同じかを確認
}

TEST_F(AnimalTest, CopyAssignmentOperator) {
    Animal original;
    Animal assigned;
    assigned = original;
    ASSERT_EQ(original.getType(), assigned.getType());
}

// makeSound() の出力テスト (コンソール出力を捕捉)
TEST_F(AnimalTest, MakeSoundOutput) {
    Animal animal;
    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf(); // cout のrdbufを保存
    std::cout.rdbuf(ss.rdbuf());                 // cout をstringstreamにリダイレクト

    animal.makeSound();

    std::cout.rdbuf(oldCout); // cout を元に戻す

    ASSERT_EQ("Generic animal sound\n", ss.str());
}

TEST_F(AnimalTest, GetType) {
    Animal animal;
    ASSERT_EQ("Generic Animal", animal.getType());
}
