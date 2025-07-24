#include "Base.hpp"
#include "gtest/gtest.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

/*
 * NOTE: The functions to be tested (generate, identify) are included here
 * for the purpose of creating a self-contained test file. In a larger
 * project, they would typically be in their own .cpp file and linked.
 */

// --- Functions to be Tested ---

Base *generate(void) {
    int choice = rand() % 3;
    if (choice == 0)
        return new A();
    if (choice == 1)
        return new B();
    return new C();
}

void identify(Base *p) {
    std::cout << "identify(pointer): ";
    if (dynamic_cast<A *>(p))
        std::cout << "A";
    else if (dynamic_cast<B *>(p))
        std::cout << "B";
    else if (dynamic_cast<C *>(p))
        std::cout << "C";
    else
        std::cout << "Unknown";
    std::cout << std::endl;
}

// Assumed implementation based on the exercise requirements
void identify(Base &p) {
    std::cout << "identify(reference): ";
    if (dynamic_cast<A *>(&p)) // This is a common workaround for the no-typeinfo rule
        std::cout << "A";
    else if (dynamic_cast<B *>(&p))
        std::cout << "B";
    else if (dynamic_cast<C *>(&p))
        std::cout << "C";
    else
        std::cout << "Unknown";
    std::cout << std::endl;
}

// --- Test Fixture for Capturing std::cout ---

class IdentifyTest : public ::testing::Test {
  protected:
    void SetUp() override {
        sbuf_ = std::cout.rdbuf();
        std::cout.rdbuf(buffer_.rdbuf());
    }

    void TearDown() override { std::cout.rdbuf(sbuf_); }

    std::string getOutput() { return buffer_.str(); }

  private:
    std::stringstream buffer_;
    std::streambuf *sbuf_;
};

// --- Test Cases ---

// Test group for identify(Base* p)
TEST_F(IdentifyTest, IdentifyPointerA) {
    A a;
    identify(&a);
    ASSERT_EQ(getOutput(), "identify(pointer): A\n");
}

TEST_F(IdentifyTest, IdentifyPointerB) {
    B b;
    identify(&b);
    ASSERT_EQ(getOutput(), "identify(pointer): B\n");
}

TEST_F(IdentifyTest, IdentifyPointerC) {
    C c;
    identify(&c);
    ASSERT_EQ(getOutput(), "identify(pointer): C\n");
}

// Test group for identify(Base& p)
TEST_F(IdentifyTest, IdentifyReferenceA) {
    A a;
    identify(a);
    ASSERT_EQ(getOutput(), "identify(reference): A\n");
}

TEST_F(IdentifyTest, IdentifyReferenceB) {
    B b;
    identify(b);
    ASSERT_EQ(getOutput(), "identify(reference): B\n");
}

TEST_F(IdentifyTest, IdentifyReferenceC) {
    C c;
    identify(c);
    ASSERT_EQ(getOutput(), "identify(reference): C\n");
}

// Test for the generate() function
TEST(GenerateTest, ReturnsValidPointer) {
    // Seed the random number generator to ensure different results per run
    srand(time(NULL));

    Base *p = generate();
    // The most basic test is to ensure it doesn't return null.
    ASSERT_NE(p, (void *)NULL);

    // Clean up memory
    delete p;
}
