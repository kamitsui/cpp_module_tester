#include "Cure.hpp"
#include "ICharacter.hpp" // For use() method
#include "Ice.hpp"
#include "gtest/gtest.h"
#include <sstream>

// Mock ICharacter for testing Materia::use
class MockCharacter : public ICharacter {
  private:
    std::string name;

  public:
    MockCharacter(const std::string &name) : name(name) {}
    ~MockCharacter() override {}
    std::string const &getName() const override { return name; }
    void equip(AMateria *m) override { (void)m; }
    void unequip(int idx) override { (void)idx; }
    void use(int idx, ICharacter &target) override {
        (void)idx;
        (void)target;
    }
};

class IceCureMateriaTest : public ::testing::Test {
  protected:
    MockCharacter *target;
    void SetUp() override { target = new MockCharacter("Bob"); }
    void TearDown() override { delete target; }
};

TEST_F(IceCureMateriaTest, IceConstructorType) {
    Ice ice;
    ASSERT_EQ("ice", ice.getType());
}

TEST_F(IceCureMateriaTest, IceClone) {
    Ice originalIce;
    AMateria *clonedIce = originalIce.clone();
    ASSERT_NE(nullptr, clonedIce);
    ASSERT_EQ("ice", clonedIce->getType());
    ASSERT_NE(&originalIce, clonedIce);
    delete clonedIce;
}

TEST_F(IceCureMateriaTest, IceUseOutput) {
    Ice ice;
    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    ice.use(*target);

    std::cout.rdbuf(oldCout);
    ASSERT_EQ("* shoots an ice bolt at Bob *\n", ss.str());
}

TEST_F(IceCureMateriaTest, CureConstructorType) {
    Cure cure;
    ASSERT_EQ("cure", cure.getType());
}

TEST_F(IceCureMateriaTest, CureClone) {
    Cure originalCure;
    AMateria *clonedCure = originalCure.clone();
    ASSERT_NE(nullptr, clonedCure);
    ASSERT_EQ("cure", clonedCure->getType());
    ASSERT_NE(&originalCure, clonedCure);
    delete clonedCure;
}

TEST_F(IceCureMateriaTest, CureUseOutput) {
    Cure cure;
    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    cure.use(*target);

    std::cout.rdbuf(oldCout);
    ASSERT_EQ("* heals Bob’s wounds *\n", ss.str());
}

TEST_F(IceCureMateriaTest, PolymorphicMateriaUse) {
    AMateria *icePtr = new Ice();
    AMateria *curePtr = new Cure();

    std::stringstream ss_ice;
    std::stringstream ss_cure;
    std::streambuf *oldCout = std::cout.rdbuf();

    std::cout.rdbuf(ss_ice.rdbuf());
    icePtr->use(*target);
    std::cout.rdbuf(oldCout);
    ASSERT_EQ("* shoots an ice bolt at Bob *\n", ss_ice.str());

    std::cout.rdbuf(ss_cure.rdbuf());
    curePtr->use(*target);
    std::cout.rdbuf(oldCout);
    ASSERT_EQ("* heals Bob’s wounds *\n", ss_cure.str());

    delete icePtr;
    delete curePtr;
}
