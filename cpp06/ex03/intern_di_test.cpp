#include "ControllableFailingFactory.hpp" // 分離したテスト用Factoryをインクルード
#include "Intern.hpp"
#include "gtest/gtest.h"

TEST(InternDependencyInjectionTest, HandlesAllocationFailure) {
    ControllableFailingFactory factory;
    Intern intern(&factory);

    // 0回目の呼び出し（つまり、次）で失敗するように設定
    ControllableFailingFactory::setFailAfter(0);

    EXPECT_THROW(intern.makeForm("robotomy request", "Bender"), std::bad_alloc);
}

TEST(InternDependencyInjectionTest, FailsOnSecondAllocation) {
    ControllableFailingFactory factory;
    Intern intern(&factory);

    // 1回成功した後、2回目で失敗するように設定
    ControllableFailingFactory::setFailAfter(1);

    AForm *form1 = NULL;
    ASSERT_NO_THROW(form1 = intern.makeForm("shrubbery creation", "garden"));
    ASSERT_NE(form1, (AForm *)NULL);

    EXPECT_THROW(intern.makeForm("robotomy request", "Bender"), std::bad_alloc);

    delete form1;
}
