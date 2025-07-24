#include "CustomAllocator.hpp" // 作成したカスタムアロケータをインクルード
#include "Intern.hpp"
#include "gtest/gtest.h"

// Internのメモリアロケーション失敗をテストするためのフィクスチャ
class InternAllocationTest : public ::testing::Test {
  protected:
    Intern *intern;

    // 各テストの前に呼ばれる
    void SetUp() override {
        AllocationController::reset(); // アロケータの状態をリセット
        intern = new Intern();
    }

    // 各テストの後に呼ばれる
    void TearDown() override {
        delete intern;
        AllocationController::reset(); // 念のため再度リセット
    }
};

// --- Test Case: Handles New Failure ---
// Intern::makeForm内でnewが失敗した場合に、std::bad_alloc例外が
// 正しくスローされる（=プログラムがクラッシュしない）ことをテストします。
TEST_F(InternAllocationTest, HandlesNewFailure) {
    // 最初のnew (Internのコンストラクタ内ではない、makeForm内のnew) で失敗させる
    // 0を設定すると、次のnewが失敗する
    const std::string &formName = "robotomy request";
    const std::string &formTarget = "Bender";
    std::cout << AllocationController::failIn << "Before" << std::endl;
    AllocationController::setFailAfter(1);
    // std::cout << AllocationController::failIn << std::endl;

    // makeFormを呼び出すと、内部でフォームオブジェクトをnewしようとして失敗し、
    // std::bad_allocがスローされるはず
    // ASSERT_NO_THROW(intern->makeForm("robotomy request", "Bender"));
    EXPECT_THROW(intern->makeForm(formName, formTarget), std::bad_alloc);
}

// --- Test Case: SuccessAfterReset ---
// アロケータがリセットされた後、正常にオブジェクトが作成できることを確認します。
TEST_F(InternAllocationTest, SuccessAfterReset) {
    // 失敗設定
    AllocationController::setFailAfter(0);
    EXPECT_THROW(intern->makeForm("shrubbery creation", "garden"), std::bad_alloc);

    // リセット（デフォルトでは自動でリセットされるが、明示的にテスト）
    AllocationController::reset();

    AForm *form = NULL;
    // リセット後は例外がスローされないはず
    ASSERT_NO_THROW(form = intern->makeForm("presidential pardon", "Ford"));

    // フォームが正しく作成されていることを確認
    ASSERT_NE(form, (AForm *)NULL);
    delete form;
}
