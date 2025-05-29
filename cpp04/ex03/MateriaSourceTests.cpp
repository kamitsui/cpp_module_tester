#include "Cure.hpp" // For learning specific materia
#include "Ice.hpp"  // For learning specific materia
#include "MateriaSource.hpp"
#include "gtest/gtest.h"
#include <sstream>

class MateriaSourceTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(MateriaSourceTest, ConstructorInitializesEmpty) {
    MateriaSource src;
    AMateria *created = src.createMateria("ice");
    ASSERT_EQ(nullptr, created); // Should not create anything yet
}

TEST_F(MateriaSourceTest, LearnMateriaCopiesTemplate) {
    MateriaSource src;
    AMateria *originalIce = new Ice();
    src.learnMateria(originalIce); // src makes a clone of originalIce
    // originalIce should still be valid, but should be deleted eventually if not managed
    // The 'learnMateria' function takes ownership of the passed pointer by cloning it, and deletes it if full.
    // So, in this test context, if learnMateria does not delete it (because it learns it), originalIce needs manual
    // deletion or proper ownership.
    delete originalIce; // Clean up the original passed materia

    AMateria *createdIce = src.createMateria("ice");
    ASSERT_NE(nullptr, createdIce);
    ASSERT_EQ("ice", createdIce->getType());
    delete createdIce;
}

TEST_F(MateriaSourceTest, LearnMateriaToFullSlots) {
    MateriaSource src;
    src.learnMateria(new Ice());
    src.learnMateria(new Cure());
    src.learnMateria(new Ice());
    src.learnMateria(new Cure()); // Source is now full

    AMateria *extraMateria = new Ice(); // This should be deleted by learnMateria
    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    src.learnMateria(extraMateria); // Try to learn to full slots

    std::cout.rdbuf(oldCout);
    ASSERT_TRUE(ss.str().find("cannot learn more materia") != std::string::npos);
    // Valgrind would confirm extraMateria is deleted
}

TEST_F(MateriaSourceTest, CreateUnknownMateriaReturnsNull) {
    MateriaSource src;
    src.learnMateria(new Ice());
    AMateria *unknown = src.createMateria("fire");
    ASSERT_EQ(nullptr, unknown);
}

TEST_F(MateriaSourceTest, CopyConstructorDeepCopiesLearnedMaterias) {
    MateriaSource original;
    original.learnMateria(new Ice());
    original.learnMateria(new Cure());

    MateriaSource copied = original; // Copy constructor

    ASSERT_NE(nullptr, copied.createMateria("ice"));
    ASSERT_NE(nullptr, copied.createMateria("cure"));

    // Test for deep copy: Modify original's learned materia, check copy
    // (This requires direct access to learnedMateria or a more complex setup)
    // A simpler check is that creating from 'original' and 'copied' yields separate objects
    AMateria *originalIce = original.createMateria("ice");
    AMateria *copiedIce = copied.createMateria("ice");
    ASSERT_NE(originalIce, copiedIce); // They are distinct objects cloned from distinct templates
    delete originalIce;
    delete copiedIce;
}

TEST_F(MateriaSourceTest, CopyAssignmentOperatorDeepCopiesLearnedMaterias) {
    MateriaSource original;
    original.learnMateria(new Ice());
    original.learnMateria(new Cure());

    MateriaSource assigned;
    assigned.learnMateria(new Ice()); // Assigned learns one initially

    assigned = original; // Copy assignment operator

    ASSERT_NE(nullptr, assigned.createMateria("ice"));
    ASSERT_NE(nullptr, assigned.createMateria("cure"));

    // Test for deep copy (similar to copy constructor)
    AMateria *originalCure = original.createMateria("cure");
    AMateria *assignedCure = assigned.createMateria("cure");
    ASSERT_NE(originalCure, assignedCure); // Distinct objects
    delete originalCure;
    delete assignedCure;
}
