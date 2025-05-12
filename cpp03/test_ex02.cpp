/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ex02.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamitsui <kamitsui@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 14:13:49 by kamitsui          #+#    #+#             */
/*   Updated: 2025/05/12 16:35:02 by kamitsui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FragTrap.hpp"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

// ヘルパー関数: std::cout の出力をキャプチャする
std::string getStdout(std::function<void()> func) {
    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());
    func();
    std::cout.rdbuf(oldCout);
    return ss.str();
}

// デフォルトコンストラクター
TEST(FragTrapTest, DefaultConstructor) {
    FragTrap unit;
    ASSERT_EQ(unit.getName(), "DefaultFrag");
    ASSERT_EQ(unit.getHitPoints(), 100);
    ASSERT_EQ(unit.getEnergyPoints(), 100);
    ASSERT_EQ(unit.getAttackDamage(), 30);
    // std::string expectedOutput = "ClapTrap Default constructor called\nFragTrap Default constructor called\n";
    // ASSERT_EQ(getStdout([&]() { FragTrap temp_unit; }), expectedOutput);
}

// 文字列パラメーター付きコンストラクター
TEST(FragTrapTest, StringConstructor) {
    FragTrap unit("TestFrag");
    ASSERT_EQ(unit.getName(), "TestFrag");
    ASSERT_EQ(unit.getHitPoints(), 100);
    ASSERT_EQ(unit.getEnergyPoints(), 100);
    ASSERT_EQ(unit.getAttackDamage(), 30);
    // std::string expectedOutput =
    //     "ClapTrap String constructor called for TestFrag\nFragTrap String constructor called for TestFrag\n";
    // ASSERT_EQ(getStdout([&]() { FragTrap temp_unit("TestFrag"); }), expectedOutput);
}

// コピーコンストラクター
TEST(FragTrapTest, CopyConstructor) {
    FragTrap original("OriginalFrag");
    // original.setHitPoints(70);
    // original.setEnergyPoints(80);
    // original.setAttackDamage(35);

    FragTrap copy(original);

    ASSERT_EQ(copy.getName(), "OriginalFrag");
    ASSERT_EQ(copy.getHitPoints(), 100);
    ASSERT_EQ(copy.getEnergyPoints(), 100);
    ASSERT_EQ(copy.getAttackDamage(), 30);

    FragTrap copy2 = original;

    ASSERT_EQ(copy2.getName(), "OriginalFrag");
    ASSERT_EQ(copy2.getHitPoints(), 100);
    ASSERT_EQ(copy2.getEnergyPoints(), 100);
    ASSERT_EQ(copy2.getAttackDamage(), 30);

    // original.setName("ModifiedOriginalFrag");
    // original.setHitPoints(95);
    // ASSERT_NE(copy.getName(), "ModifiedOriginalFrag");
    // ASSERT_NE(copy.getHitPoints(), 95);

    // std::string expectedOutput = "ClapTrap String constructor called for OriginalFrag\nFragTrap String constructor "
    //                              "called for OriginalFrag\nClapTrap Copy constructor called for
    //                              OriginalFrag\nFragTrap " "Copy constructor called for OriginalFrag\n";
    // ASSERT_EQ(getStdout([&]() {
    //               FragTrap original_temp("OriginalFrag");
    //               FragTrap copy_temp = original_temp;
    //           }),
    //           expectedOutput);
}

// コピー代入演算子
TEST(FragTrapTest, CopyAssignmentOperator) {
    FragTrap original("OriginalFrag");
    // original.setHitPoints(70);
    // original.setEnergyPoints(80);
    // original.setAttackDamage(35);

    FragTrap assigned("AssignedFrag");
    assigned = original;

    ASSERT_EQ(assigned.getName(), "OriginalFrag");
    ASSERT_EQ(assigned.getHitPoints(), 100);
    ASSERT_EQ(assigned.getEnergyPoints(), 100);
    ASSERT_EQ(assigned.getAttackDamage(), 30);

    // original.setName("ModifiedOriginalFrag");
    // original.setHitPoints(95);
    // ASSERT_NE(assigned.getName(), "ModifiedOriginalFrag");
    // ASSERT_NE(assigned.getHitPoints(), 95);

    // std::string expectedOutput =
    //     "ClapTrap String constructor called for OriginalFrag\nFragTrap String constructor called for "
    //     "OriginalFrag\nClapTrap String constructor called for AssignedFrag\nFragTrap String constructor called for "
    //     "AssignedFrag\nClapTrap Assignation operator called for AssignedFrag\nFragTrap Assignation operator called
    //     for " "AssignedFrag\n";
    // ASSERT_EQ(getStdout([&]() {
    //               FragTrap original_temp("OriginalFrag");
    //               FragTrap assigned_temp("AssignedFrag");
    //               assigned_temp = original_temp;
    //           }),
    //           expectedOutput);
}

// Attack メソッド
TEST(FragTrapTest, Attack) {
    FragTrap attacker("AttackerFrag");
    std::string target = "Target";
    std::string expectedOutput = "FragTrap AttackerFrag attacks Target, causing 30 points of damage!\n";

    // ターゲットに30pointsのダメージ
    ASSERT_EQ(getStdout([&]() { attacker.attack(target); }), expectedOutput);

    // Energy Points の残量確認
    ASSERT_EQ(attacker.getEnergyPoints(), 99); // Energy points should decrease
}

// ダメージを受ける
TEST(FragTrapTest, TakeDamage) {
    FragTrap unit("VictimFrag");

    // hitPoint 100 - 40 -> 60
    unsigned int damageAmount = 40;
    std::string expectedOutput =
        "ClapTrap VictimFrag takes 40 points of damage!\nClapTrap VictimFrag has 60 hit points remaining.\n";
    ASSERT_EQ(getStdout([&]() { unit.takeDamage(damageAmount); }), expectedOutput);
    ASSERT_EQ(unit.getHitPoints(), 60);

    // hitPoint 60 < 110 (lethalDamage)
    unsigned int lethalDamage = 110;
    std::string expectedOutput2 = "ClapTrap VictimFrag takes 110 points of damage!\nClapTrap VictimFrag has died!\n";
    ASSERT_EQ(getStdout([&]() { unit.takeDamage(lethalDamage); }), expectedOutput2);
    ASSERT_EQ(unit.getHitPoints(), 0);
}

// 自己修復
TEST(FragTrapTest, BeRepaired) {
    FragTrap unit("HealerFrag");
    unit.takeDamage(60); // 100 - 60 = 40

    // hitPoints( 40 -> 70 ), energyPoints( 100 -> 99 )
    unsigned int repairAmount = 30;
    std::string expectedOutput = "ClapTrap HealerFrag repairs itself, regaining 30 hit points!\nClapTrap HealerFrag "
                                 "now has 70 hit points and 99 energy points.\n";
    ASSERT_EQ(getStdout([&]() { unit.beRepaired(repairAmount); }), expectedOutput);
    ASSERT_EQ(unit.getHitPoints(), 70);

    // EnergyPoints の残量確認
    ASSERT_EQ(unit.getEnergyPoints(), 99); // Energy points should decrease
}

// ハイタッチ要求のメッセージ出力
TEST(FragTrapTest, HighFivesGuys) {
    FragTrap unit("HappyFrag");
    std::string expectedOutput = "FragTrap HappyFrag says: \"High fives, anyone?\"\n";
    ASSERT_EQ(getStdout([&]() { unit.highFivesGuys(); }), expectedOutput);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
