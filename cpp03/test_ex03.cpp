/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ex03.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamitsui <kamitsui@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 22:33:58 by kamitsui          #+#    #+#             */
/*   Updated: 2025/05/15 14:33:05 by kamitsui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DiamondTrap.hpp"
#include "gtest/gtest.h"
#include <sstream>
#include <string>

// Helper function to capture stdout output
std::string getStdout(std::function<void()> func) {
    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());
    func();
    std::cout.rdbuf(oldCout);
    return ss.str();
}

// デフォルトコンストラクター
TEST(DiamondTrapTest, DefaultConstructor) {
    DiamondTrap unit;
    ASSERT_EQ(unit.getName(), "Default");
    ASSERT_EQ(unit.getHitPoints(), 100u);   // Inherited from FragTrap
    ASSERT_EQ(unit.getEnergyPoints(), 50u); // Inherited from ScavTrap
    ASSERT_EQ(unit.getAttackDamage(), 30u); // Inherited from FragTrap
    // std::string expectedOutput = "ClapTrap Default constructor called\nFragTrap Default constructor called\nScavTrap
    // "
    //                              "Default constructor called\nDiamondTrap Default constructor called\n";
    // ASSERT_EQ(getStdout([&]() { DiamondTrap temp_unit; }), expectedOutput);
}

// Stringコンストラクター
TEST(DiamondTrapTest, StringConstructor) {
    DiamondTrap unit("TestDiamond");
    ASSERT_EQ(unit.getName(), "TestDiamond");
    ASSERT_EQ(unit.getHitPoints(), 100u);
    ASSERT_EQ(unit.getEnergyPoints(), 50u);
    ASSERT_EQ(unit.getAttackDamage(), 30u);

    // std::string expectedOutput = "ClapTrap String constructor called for TestDiamond_clap_name\nFragTrap String "
    //                              "constructor called for TestDiamond\nScavTrap String constructor called for "
    //                              "TestDiamond\nDiamondTrap String constructor called for TestDiamond\n"
    //                              "DiamondTrap Destructor called for TestDiamond\n";
    // ASSERT_EQ(getStdout([&]() { DiamondTrap temp_unit("TestDiamond"); }), expectedOutput);
}

// コピーコンストラクター
TEST(DiamondTrapTest, CopyConstructor) {
    DiamondTrap original("OriginalDiamond"); // hit 100, energy 50, attack 30
    // original.setHitPoints(70);
    // original.setEnergyPoints(40);
    // original.setAttackDamage(35);

    DiamondTrap copy(original);

    ASSERT_EQ(copy.getName(), "OriginalDiamond");
    ASSERT_EQ(copy.getHitPoints(), 100u);
    ASSERT_EQ(copy.getEnergyPoints(), 50u);
    ASSERT_EQ(copy.getAttackDamage(), 30u);

    // original.setName("ModifiedOriginalDiamond");
    // original.setHitPoints(95);
    // ASSERT_NE(copy.getName(), "ModifiedOriginalDiamond");
    // ASSERT_NE(copy.getHitPoints(), 95);

    // std::string expectedOutput =
    //     "ClapTrap String constructor called for OriginalDiamond_clap_name\nFragTrap String constructor called for "
    //     "OriginalDiamond\nScavTrap String constructor called for OriginalDiamond\nDiamondTrap String constructor "
    //     "called for OriginalDiamond\nClapTrap Copy constructor called for OriginalDiamond\nFragTrap Copy constructor
    //     " "called for OriginalDiamond\nScavTrap Copy constructor called for OriginalDiamond\nDiamondTrap Copy "
    //     "constructor called for OriginalDiamond\n";
    // ASSERT_EQ(getStdout([&]() {
    //               DiamondTrap original_temp("OriginalDiamond");
    //               DiamondTrap copy_temp = original_temp;
    //           }),
    //           expectedOutput);
}

// コピー代入演算子
TEST(DiamondTrapTest, CopyAssignmentOperator) {
    DiamondTrap original("OriginalDiamond"); // hit 100, energy 50, attack 30
    // original.setHitPoints(70);
    // original.setEnergyPoints(40);
    // original.setAttackDamage(35);

    DiamondTrap assigned("AssignedDiamond");
    assigned = original;

    ASSERT_EQ(assigned.getName(), "OriginalDiamond");
    ASSERT_EQ(assigned.getHitPoints(), 100u);
    ASSERT_EQ(assigned.getEnergyPoints(), 50u);
    ASSERT_EQ(assigned.getAttackDamage(), 30u);

    // original.setName("ModifiedOriginalDiamond");
    // original.setHitPoints(95);
    // ASSERT_NE(assigned.getName(), "ModifiedOriginalDiamond");
    // ASSERT_NE(assigned.getHitPoints(), 95);

    // std::string expectedOutput =
    //     "ClapTrap String constructor called for OriginalDiamond_clap_name\nFragTrap String constructor called for "
    //     "OriginalDiamond\nScavTrap String constructor called for OriginalDiamond\nDiamondTrap String constructor "
    //     "called for OriginalDiamond\nClapTrap String constructor called for AssignedDiamond_clap_name\nFragTrap
    //     String " "constructor called for AssignedDiamond\nScavTrap String constructor called for
    //     AssignedDiamond\nDiamondTrap " "String constructor called for AssignedDiamond\nClapTrap Assignation operator
    //     called for " "AssignedDiamond\nDiamondTrap Assignation operator called for AssignedDiamond\n";
    // ASSERT_EQ(getStdout([&]() {
    //               DiamondTrap original_temp("OriginalDiamond");
    //               DiamondTrap assigned_temp("AssignedDiamond");
    //               assigned_temp = original_temp;
    //           }),
    //           expectedOutput);
}

// 攻撃
TEST(DiamondTrapTest, Attack) {
    DiamondTrap unit("AttackerDiamond");
    std::string target = "Target";

    // Virtual Inherited from ScavTrap : Only one SubObject(Clap)
    std::string expectedOutput = "ScavTrap AttackerDiamond_clap_name attacks Target, causing 30 points of damage!\n";

    // this case without Virtual Inherited : SubObject(Scav, Frag, Clap)
    //"ScavTrap AttackerDiamond attacks Target, causing 20 points of damage!\n";

    ASSERT_EQ(getStdout([&]() { unit.attack(target); }), expectedOutput);
    ASSERT_EQ(unit.getEnergyPoints(), 49u); // Energy points should decrease (ScavTrap)
}

// ダメージを受ける
TEST(DiamondTrapTest, TakeDamage) {
    DiamondTrap unit("VictimDiamond");
    unsigned int damageAmount = 30;

    // Virtual Inherited from ClapTrap : Only one SubObject(Clap)
    std::string expectedOutput =
        "ClapTrap VictimDiamond_clap_name takes 30 points of damage!\nClapTrap VictimDiamond_clap_name "
        "has 70 hit points remaining.\n";

    // Inherited from ClapTrap (via FragTrap or ScavTrap)
    // std::string expectedOutput =
    //    "ClapTrap VictimDiamond takes 30 points of damage!\nClapTrap VictimDiamond has 70 hit points remaining.\n";

    ASSERT_EQ(getStdout([&]() { unit.takeDamage(damageAmount); }), expectedOutput);
    ASSERT_EQ(unit.getHitPoints(), 70u);
}

// 自己回復する
TEST(DiamondTrapTest, BeRepaired) {
    DiamondTrap unit("HealerDiamond");
    unit.takeDamage(50);
    unsigned int repairAmount = 20;

    // Virtual Inherited from ClapTrap : Only one SubObject(Clap)
    std::string expectedOutput = "ClapTrap HealerDiamond_clap_name repairs itself, regaining 20 hit points!\nClapTrap "
                                 "HealerDiamond_clap_name now has 70 hit points "
                                 "and 49 energy points.\n";

    // Inherited from ClapTrap (via FragTrap or ScavTrap)
    // std::string expectedOutput =
    //    "ClapTrap HealerDiamond repairs itself, regaining 20 hit points!\nClapTrap HealerDiamond now has 70 hit points
    //    " "and 49 energy points.\n";

    ASSERT_EQ(getStdout([&]() { unit.beRepaired(repairAmount); }), expectedOutput);
    ASSERT_EQ(unit.getHitPoints(), 70u);
    ASSERT_EQ(unit.getEnergyPoints(), 49u); // Energy points should decrease (ClapTrap)
}

// 自分（派生オブジェクトDiamondTrap）と継承元（サブオブジェクトClapTrap）の名前
TEST(DiamondTrapTest, WhoAmI) {
    DiamondTrap unit("IdentityDiamond");
    std::string expectedOutput = "DiamondTrap name: IdentityDiamond, ClapTrap name: IdentityDiamond_clap_name\n";

    ASSERT_EQ(getStdout([&]() { unit.whoAmI(); }), expectedOutput);
}

// ハイタッチ
TEST(DiamondTrapTest, HighFivesGuys) {
    DiamondTrap unit("HiFiveDiamond");

    // Virtual Inherited from ClapTrap : Only one SubObject(Clap)
    std::string expectedOutput = "FragTrap HiFiveDiamond_clap_name says: \"High fives, anyone?\"\n";

    // Inherited from ClapTrap (via FragTrap)
    // std::string expectedOutput = "FragTrap HiFiveDiamond says: \"High fives, anyone?\"\n";

    ASSERT_EQ(getStdout([&]() { unit.highFivesGuys(); }), expectedOutput);
}

// ガードモード
TEST(DiamondTrapTest, GuardGate) {
    DiamondTrap unit("GuardianDiamond");

    // Virtual Inherited from ClapTrap : Only one SubObject(Clap)
    std::string expectedOutput = "ScavTrap GuardianDiamond_clap_name has entered Gate keeper mode.\n";

    // Inherited from ClapTrap (via ScavTrap)
    // std::string expectedOutput = "ScavTrap GuardianDiamond has entered Gate keeper mode.\n";

    ASSERT_EQ(getStdout([&]() { unit.guardGate(); }), expectedOutput);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
