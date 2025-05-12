/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ex01.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamitsui <kamitsui@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:00:27 by kamitsui          #+#    #+#             */
/*   Updated: 2025/05/09 18:45:04 by kamitsui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScavTrap.hpp"
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
TEST(ScavTrapTest, DefaultConstructor) {
    ScavTrap unit;
    ASSERT_EQ(unit.getName(), "DefaultScav");
    ASSERT_EQ(unit.getHitPoints(), 100);
    ASSERT_EQ(unit.getEnergyPoints(), 50);
    ASSERT_EQ(unit.getAttackDamage(), 20);
}

// 文字列パラメーター付きコンストラクター
TEST(ScavTrapTest, StringConstructor) {
    ScavTrap unit("TestScav");
    ASSERT_EQ(unit.getName(), "TestScav");
    ASSERT_EQ(unit.getHitPoints(), 100);
    ASSERT_EQ(unit.getEnergyPoints(), 50);
    ASSERT_EQ(unit.getAttackDamage(), 20);
}

// コピーコンストラクター
TEST(ScavTrapTest, CopyConstructor) {
    ScavTrap original("OriginalScav");
    // default hitPoints(100), energyPoints(50), attack(20)

    // Nothing Setter method
    // original.setHitPoints(75);
    // original.setEnergyPoints(40);
    // original.setAttackDamage(25);

    ScavTrap copy(original);

    ASSERT_EQ(copy.getName(), "OriginalScav");
    ASSERT_EQ(copy.getHitPoints(), 100);
    ASSERT_EQ(copy.getEnergyPoints(), 50);
    ASSERT_EQ(copy.getAttackDamage(), 20);

    // Nothing Setter method
    // original.setName("ModifiedOriginalScav");
    // original.setHitPoints(90);
    // ASSERT_NE(copy.getName(), "ModifiedOriginalScav");
    // ASSERT_NE(copy.getHitPoints(), 90);
}

// コピー代入演算子
TEST(ScavTrapTest, CopyAssignmentOperator) {
    ScavTrap original("OriginalScav");

    ScavTrap assigned("AssignedScav");
    assigned = original;

    ASSERT_EQ(assigned.getName(), "OriginalScav");
    ASSERT_EQ(assigned.getHitPoints(), 100);
    ASSERT_EQ(assigned.getEnergyPoints(), 50);
    ASSERT_EQ(assigned.getAttackDamage(), 20);
}

// Attack メソッド
TEST(ScavTrapTest, Attack) {
    ScavTrap attacker("AttackerScav");
    std::string target = "Target";

    // ターゲットに 20points のダメージ
    std::string expectedOutput = "ScavTrap AttackerScav attacks Target, causing 20 points of damage!\n";
    ASSERT_EQ(getStdout([&]() { attacker.attack(target); }), expectedOutput);

    // EnergyPoints の残量確認
    ASSERT_EQ(attacker.getEnergyPoints(), 49); // Energy points should decrease
}

// ダメージを受ける
TEST(ScavTrapTest, TakeDamage) {
    ScavTrap unit("VictimScav");

    // hitPoint 100 - 30 -> 70
    unsigned int damageAmount = 30;
    std::string expectedOutput =
        "ClapTrap VictimScav takes 30 points of damage!\nClapTrap VictimScav has 70 hit points remaining.\n";
    ASSERT_EQ(getStdout([&]() { unit.takeDamage(damageAmount); }), expectedOutput);
    ASSERT_EQ(unit.getHitPoints(), 70);

    // hitPoint 70 < 100 (lethalDamage)
    unsigned int lethalDamage = 100;
    std::string expectedOutput2 = "ClapTrap VictimScav takes 100 points of damage!\nClapTrap VictimScav has died!\n";
    ASSERT_EQ(getStdout([&]() { unit.takeDamage(lethalDamage); }), expectedOutput2);
    ASSERT_EQ(unit.getHitPoints(), 0); // remaining 0 : already out.
}

// 自己修復
TEST(ScavTrapTest, BeRepaired) {
    ScavTrap unit("HealerScav");
    unit.takeDamage(50);
    unsigned int repairAmount = 20;
    std::string expectedOutput = "ClapTrap HealerScav repairs itself, regaining 20 hit points!\nClapTrap HealerScav "
                                 "now has 70 hit points and 49 energy points.\n";
    ASSERT_EQ(getStdout([&]() { unit.beRepaired(repairAmount); }), expectedOutput);
    ASSERT_EQ(unit.getHitPoints(), 70);
    ASSERT_EQ(unit.getEnergyPoints(), 49); // Energy points should decrease
}

TEST(ScavTrapTest, GuardGate) {
    ScavTrap unit("GuardianScav");
    std::string expectedOutput = "ScavTrap GuardianScav has entered Gate keeper mode.\n";
    ASSERT_EQ(getStdout([&]() { unit.guardGate(); }), expectedOutput);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
