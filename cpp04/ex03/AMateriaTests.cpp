#include "AMateria.hpp"
#include "ICharacter.hpp" // Required for AMateria::use signature
#include "gtest/gtest.h"
#include <sstream>

// Mock ICharacter for testing AMateria::use
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

// AMateria is abstract, so we need a concrete class to test its common parts
class ConcreteMateria : public AMateria {
  public:
    ConcreteMateria(const std::string &type) : AMateria(type) {}
    AMateria *clone() const override { return new ConcreteMateria(getType()); }
    void use(ICharacter &target) override {
        std::cout << "* " << getType() << " used on " << target.getName() << " *" << std::endl;
    }
};

class AMateriaTest : public ::testing::Test {
  protected:
    MockCharacter *target;
    void SetUp() override { target = new MockCharacter("TestTarget"); }
    void TearDown() override { delete target; }
};

TEST_F(AMateriaTest, ConstructorInitializesType) {
    ConcreteMateria materia("test_type");
    ASSERT_EQ("test_type", materia.getType());
}

TEST_F(AMateriaTest, CopyConstructor) {
    ConcreteMateria original("original_type");
    ConcreteMateria copied = original;
    ASSERT_EQ(original.getType(), copied.getType());
}

TEST_F(AMateriaTest, CopyAssignmentOperator) {
    ConcreteMateria original("original_type");
    ConcreteMateria assigned("another_type");
    assigned = original;
    ASSERT_EQ(original.getType(), assigned.getType());
}

TEST_F(AMateriaTest, UseMethodOutput) {
    ConcreteMateria materia("generic_materia");
    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    materia.use(*target);

    std::cout.rdbuf(oldCout);
    ASSERT_EQ("* generic_materia used on TestTarget *\n", ss.str());
}

TEST_F(AMateriaTest, CloneReturnsCorrectType) {
    ConcreteMateria original("clonable_type");
    AMateria *clonedMateria = original.clone();
    ASSERT_NE(nullptr, clonedMateria);
    ASSERT_EQ(original.getType(), clonedMateria->getType());
    ASSERT_NE(&original, clonedMateria); // Ensure it's a new object
    delete clonedMateria;
}
