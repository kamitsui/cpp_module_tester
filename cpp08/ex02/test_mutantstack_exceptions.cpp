#include "MutantStack.hpp"
#include "gtest/gtest.h"

// --- Exception and Undefined Behavior Tests ---

// 未定義動作：環境依存によるため、このテストは不要です。

// 注意: 空のstd::stackに対してtop()やpop()を呼び出すことは「未定義の動作」です。
// クラッシュや例外など、実際の結果はコンパイラやライブラリの実装に依存します。
// ここでは、致命的なエラーでプログラムが終了することを期待するASSERT_DEATHを使用します。

// TEST(MutantStackExceptionTest, TopOnEmptyStackCausesFatalError) {
//     MutantStack<int> mstack;
//     // 空のスタックに対してtop()を呼び出すと、プログラムが終了することを期待します。
//     // 第2引数はstderrに出力されるエラーメッセージの正規表現ですが、環境に依存するため
//     // 空文字列 "" を指定して任意のエラーメッセージにマッチさせます。
//     ASSERT_DEATH({ mstack.top(); }, "");
// }
//
// TEST(MutantStackExceptionTest, PopOnEmptyStackCausesFatalError) {
//     MutantStack<int> mstack;
//     // 空のスタックに対してpop()を呼び出すと、プログラムが終了することを期待します。
//     ASSERT_DEATH({ mstack.pop(); }, "");
// }

// このテストは、スタックの「正しい」使い方を示すものです。
TEST(MutantStackExceptionTest, CorrectUsageDoesNotCauseError) {
    MutantStack<int> mstack;

    // アクセスする前に、必ずスタックが空でないことを確認します。
    ASSERT_NO_THROW({
        if (!mstack.empty()) {
            (void)mstack.top();
        }
    });
    ASSERT_NO_THROW({
        if (!mstack.empty()) {
            mstack.pop();
        }
    });

    mstack.push(42);
    ASSERT_NO_THROW({
        if (!mstack.empty()) {
            EXPECT_EQ(mstack.top(), 42);
            mstack.pop();
        }
    });
    // 正しい使い方では、エラーやクラッシュは発生しません。
}
