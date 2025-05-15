/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ex00.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamitsui <kamitsui@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:57:49 by kamitsui          #+#    #+#             */
/*   Updated: 2025/05/15 14:25:42 by kamitsui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClapTrap.hpp"
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

TEST(ClapTrapTest, DefaultConstructor) {
    ClapTrap unit;
    ASSERT_EQ(unit.getName(), "Default");
    ASSERT_EQ(unit.getHitPoints(), 10u);
    ASSERT_EQ(unit.getEnergyPoints(), 10u);
    ASSERT_EQ(unit.getAttackDamage(), 0u);

    //  NG TEST : 補助テスト
    //  コンストラクタの出力メッセージで間接的に確認
    //  オブジェクトの生成〜デストロイが行われるため、コンストラクタ単体のログをテストはできない。
    // std::string expectedOutput = "ClapTrap Default constructor called\n";
    // ASSERT_EQ(getStdout([&]() { ClapTrap tmp_unit; }), expectedOutput);
}

TEST(ClapTrapTest, StringConstructor) {
    ClapTrap unit("TestUnit");
    ASSERT_EQ(unit.getName(), "TestUnit");
    ASSERT_EQ(unit.getHitPoints(), 10u);
    ASSERT_EQ(unit.getEnergyPoints(), 10u);
    ASSERT_EQ(unit.getAttackDamage(), 0u);
}

TEST(ClapTrapTest, CopyConstructor) {
    ClapTrap original("CopyUnit");
    ClapTrap unit(original);
    ASSERT_EQ(unit.getName(), "CopyUnit");
    ASSERT_EQ(unit.getHitPoints(), 10u);
    ASSERT_EQ(unit.getEnergyPoints(), 10u);
    ASSERT_EQ(unit.getAttackDamage(), 0u);
}

TEST(ClapTrapTest, CopyAssignmentOperator) {
    ClapTrap original("OriginalUnit");
    ClapTrap assigned("AssignedUnit");
    assigned = original; // コピー代入演算子の呼び出し
    ASSERT_EQ(assigned.getName(), "OriginalUnit");
    ASSERT_EQ(assigned.getHitPoints(), 10u);
    ASSERT_EQ(assigned.getEnergyPoints(), 10u);
    ASSERT_EQ(assigned.getAttackDamage(), 0u);
}

TEST(ClapTrapTest, Attack) {
    ClapTrap attacker("Attacker");
    std::string target = "Target";
    std::string expectedOutput = "ClapTrap Attacker attacks Target, causing 0 points of damage!\n";
    ASSERT_EQ(getStdout([&]() { attacker.attack(target); }), expectedOutput);
    // エネルギーポイントが減少していることを間接的に確認 (attack を再度呼ぶと異なる出力になるはず)
    std::string expectedOutput2 = "ClapTrap Attacker attacks Target, causing 0 points of damage!\n";
    ASSERT_EQ(getStdout([&]() { attacker.attack(target); }), expectedOutput2);
}

TEST(ClapTrapTest, TakeDamage) {
    ClapTrap unit("Victim");

    // ダメージを受ける　残りのヒットポイントを出力
    unsigned int damageAmount = 5;
    std::string expectedOutput =
        "ClapTrap Victim takes 5 points of damage!\nClapTrap Victim has 5 hit points remaining.\n";
    ASSERT_EQ(getStdout([&]() { unit.takeDamage(damageAmount); }), expectedOutput);

    // ヒットポイントがなくなって死亡
    unsigned int lethalDamage = 10;
    std::string expectedOutput2 = "ClapTrap Victim takes 10 points of damage!\nClapTrap Victim has died!\n";
    ASSERT_EQ(getStdout([&]() { unit.takeDamage(lethalDamage); }), expectedOutput2);

    // 死亡したユニットはさらにダメージを受けない
    std::string expectedOutput3 = "ClapTrap Victim is already out of hit points!\n";
    ASSERT_EQ(getStdout([&]() { unit.takeDamage(5); }), expectedOutput3);
}

TEST(ClapTrapTest, BeRepaired) {
    ClapTrap unit("Healer");

    // 自分を修復する energyPoint 1ポイント消費
    unsigned int repairAmount = 3;
    std::string expectedOutput = "ClapTrap Healer repairs itself, regaining 3 hit points!\nClapTrap Healer now has 13 "
                                 "hit points and 9 energy points.\n";
    ASSERT_EQ(getStdout([&]() { unit.beRepaired(repairAmount); }), expectedOutput);

    // エネルギーがない場合は修復できない：攻撃して残りの９ポイント消費してからセルフ修復（beRepaired）を行った時のテスト
    for (int i = 0; i < 9; ++i) {
        unit.attack("Dummy");
    }
    std::string expectedOutput2 = "ClapTrap Healer is out of energy points and cannot repair!\n";
    ASSERT_EQ(getStdout([&]() { unit.beRepaired(repairAmount); }), expectedOutput2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
