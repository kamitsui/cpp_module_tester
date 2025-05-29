#include "Character.hpp"
#include "Cure.hpp" // For equipping specific materia
#include "Ice.hpp"  // For equipping specific materia
#include "gtest/gtest.h"
#include <sstream>

// Mock ICharacter for testing Character::use target
class DummyTarget : public ICharacter {
  private:
    std::string name;

  public:
    DummyTarget(const std::string &name) : name(name) {}
    ~DummyTarget() override {}
    std::string const &getName() const override { return name; }
    void equip(AMateria *m) override { (void)m; }
    void unequip(int idx) override { (void)idx; }
    void use(int idx, ICharacter &target) override {
        (void)idx;
        (void)target;
    }
};

class CharacterTest : public ::testing::Test {
  protected:
    DummyTarget *target;
    void SetUp() override { target = new DummyTarget("Dummy"); }
    void TearDown() override { delete target; }
};

TEST_F(CharacterTest, ConstructorInitializesNameAndEmptyInventory) {
    Character charlie("Charlie");
    ASSERT_EQ("Charlie", charlie.getName());
    for (int i = 0; i < 4; ++i) {
        ASSERT_FALSE(charlie.isMateriaEquipped(i));
    }
}

TEST_F(CharacterTest, EquipMateriaToFirstEmptySlot) {
    Character charlie("Charlie");
    AMateria *ice = new Ice();
    charlie.equip(ice);
    ASSERT_TRUE(charlie.isMateriaEquipped(0));
    // ASSERT_EQ("ice", getMateriaTypeFromCharacter(charlie, 0));
    ASSERT_EQ("ice", charlie.getMateriaAtSlot(0)->getType());
    // ASSERT_EQ("ice", charlie.getName()); // This is a trick to get type from character's perspective, fix later
    //  Better way to check type: require Character to have a getMateriaAtSlot() or similar
}

// Helper function to get materia type for testing
// std::string getMateriaTypeFromCharacter(Character &character, int slot) {
//    // This is a common test helper. In a real project, Character might expose a way to get materia.
//    // For this test, we must assume isMateriaEquipped(slot) is true first.
//    // Or we rely on the specific message output from equip/use.
//    // For simplicity, let's assume the order of equipping is always 0, 1, 2, 3.
//    // If you need direct access to the materia pointer, you'd need a public getter in Character.
//    // Given the constraints, we'll verify via use() output or rely on equip/unequip being correct.
//
//    // As per previous discussion, Character::use() outputs the materia type.
//    // We can use that for validation.
//    std::stringstream ss;
//    std::streambuf *oldCout = std::cout.rdbuf();
//    std::cout.rdbuf(ss.rdbuf());
//
//    // character.use(slot, *character.getName() == "Alice" ? *new DummyTarget("temp") : *target); // Use a dummy
//    character.use(slot, character.getName() == "Alice" ? *new DummyTarget("temp") : *target); // Use a dummy
//
//    std::cout.rdbuf(oldCout);
//    std::string output = ss.str();
//
//    // Check if it's an ice or cure string in the output
//    if (output.find("ice bolt") != std::string::npos)
//        return "ice";
//    if (output.find("heals") != std::string::npos)
//        return "cure";
//    return ""; // No known materia used
//}

TEST_F(CharacterTest, EquipMultipleMaterias) {
    Character alice("Alice");
    alice.equip(new Ice());
    alice.equip(new Cure());
    alice.equip(new Ice());
    alice.equip(new Cure());

    ASSERT_TRUE(alice.isMateriaEquipped(0));
    ASSERT_TRUE(alice.isMateriaEquipped(1));
    ASSERT_TRUE(alice.isMateriaEquipped(2));
    ASSERT_TRUE(alice.isMateriaEquipped(3));
}

TEST_F(CharacterTest, EquipToFullInventoryDeletesMateria) {
    Character charlie("Charlie");
    charlie.equip(new Ice());
    charlie.equip(new Cure());
    charlie.equip(new Ice());
    charlie.equip(new Cure()); // Inventory now full

    AMateria *extraMateria = new Ice(); // This should be deleted by equip
    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    charlie.equip(extraMateria); // Try to equip to full inventory

    std::cout.rdbuf(oldCout);
    ASSERT_TRUE(ss.str().find("inventory is full") != std::string::npos); // Verify message
    // Manual check for no leak: Valgrind would confirm extraMateria deleted
    // No direct way to assert 'extraMateria' pointer is invalid/deleted within gtest
}

TEST_F(CharacterTest, UnequipMateriaSetsSlotToNull) {
    Character charlie("Charlie");
    AMateria *ice = new Ice();
    charlie.equip(ice);
    ASSERT_TRUE(charlie.isMateriaEquipped(0));

    charlie.unequip(0);
    ASSERT_FALSE(charlie.isMateriaEquipped(0));
    // The unequipped materia is NOT deleted by unequip, so 'ice' still points to it
    // You would manage 'ice' manually in main (or in a test case if it was owned)
    delete ice; // Clean up the unequipped materia
}

TEST_F(CharacterTest, UnequipInvalidSlotDoesNothing) {
    Character charlie("Charlie");
    charlie.equip(new Ice()); // Occupy slot 0

    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    charlie.unequip(-1);
    charlie.unequip(4);
    charlie.unequip(1); // Empty slot

    std::cout.rdbuf(oldCout);
    ASSERT_TRUE(ss.str().find("tried to unequip from invalid slot") != std::string::npos);
    ASSERT_TRUE(ss.str().find("tried to unequip from invalid slot 1") !=
                std::string::npos); // Should print even for empty valid slot
}

TEST_F(CharacterTest, UseMateriaOutputCorrectly) {
    Character charlie("Charlie");
    charlie.equip(new Ice());  // Slot 0
    charlie.equip(new Cure()); // Slot 1

    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    charlie.use(0, *target); // Use Ice
    charlie.use(1, *target); // Use Cure

    std::cout.rdbuf(oldCout);
    ASSERT_TRUE(ss.str().find("* shoots an ice bolt at Dummy *") != std::string::npos);
    ASSERT_TRUE(ss.str().find("* heals Dummy’s wounds *") != std::string::npos);
}

TEST_F(CharacterTest, UseInvalidSlotDoesNothing) {
    Character charlie("Charlie"); // Empty inventory

    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    charlie.use(-1, *target);
    charlie.use(0, *target); // Empty slot
    charlie.use(4, *target);

    std::cout.rdbuf(oldCout);
    ASSERT_TRUE(ss.str().find("tried to use invalid slot") != std::string::npos);
}

TEST_F(CharacterTest, CopyConstructorDeepCopiesMaterias) {
    Character original("Original");
    original.equip(new Ice());  // Slot 0
    original.equip(new Cure()); // Slot 1

    Character copied = original; // Copy constructor

    ASSERT_EQ(original.getName(), copied.getName());
    ASSERT_TRUE(copied.isMateriaEquipped(0));
    ASSERT_TRUE(copied.isMateriaEquipped(1));

    // Verify deep copy by unequipping in copy and checking original
    copied.unequip(0);
    ASSERT_FALSE(copied.isMateriaEquipped(0));  // Copied's slot 0 is empty
    ASSERT_TRUE(original.isMateriaEquipped(0)); // Original's slot 0 still has materia (deep copy)

    // Verify content difference after change
    copied.equip(new Cure()); // Fill copied's slot 0 with Cure
    // We cannot directly access materia pointers to compare if they are different objects
    // The previous unequip check is stronger for showing deep copy of pointers
}

TEST_F(CharacterTest, CopyAssignmentOperatorDeepCopiesMaterias) {
    Character original("Original");
    original.equip(new Ice());  // Slot 0
    original.equip(new Cure()); // Slot 1

    Character assigned("Assigned");
    assigned.equip(new Ice()); // Assigned has initial materia in slot 0

    assigned = original; // Copy assignment operator

    ASSERT_EQ(original.getName(), assigned.getName());
    ASSERT_TRUE(assigned.isMateriaEquipped(0));
    ASSERT_TRUE(assigned.isMateriaEquipped(1));

    // Verify deep copy by unequipping in assigned and checking original
    assigned.unequip(0);
    ASSERT_FALSE(assigned.isMateriaEquipped(0)); // Assigned's slot 0 is empty
    ASSERT_TRUE(original.isMateriaEquipped(0));  // Original's slot 0 still has materia (deep copy)
}

// Note: Character::equip handles deleting materia if inventory is full.
// For Character copy constructor/assignment, old materias are deleted then new ones are cloned/equipped.
// Memory leak checks for these complex scenarios are best done with Valgrind.
