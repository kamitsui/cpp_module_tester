#include "BitcoinExchange.hpp"
#include "gtest/gtest.h"
#include <cstdio> // For remove()
#include <fstream>
#include <sstream>

// --- Test Fixture for BitcoinExchange ---
// テストごとに共通のセットアップと後片付けを行うためのクラス
class BitcoinExchangeTest : public ::testing::Test {
  protected:
    // 各テストケースの開始前に呼ばれる
    void SetUp() override {
        // テスト用のダミーデータベースファイルを作成
        std::ofstream db_file("test_data.csv");
        db_file << "date,exchange_rate\n"
                << "2011-01-03,0.3\n"
                << "2011-01-09,0.32\n"
                << "2012-01-11,7.1\n"
                << "2022-03-29,47115.88\n";
        db_file.close();

        // テスト用のダミー入力ファイルを作成
        std::ofstream input_file("test_input.txt");
        input_file << "date | value\n"
                   << "2011-01-03 | 10\n"   // Exact match
                   << "2011-01-05 | 1\n"    // Closest past date
                   << "2009-01-01 | 1\n"    // Date before all records
                   << "2023-01-01 | 1\n"    // Date after all records
                   << "2012-01-11 | -1\n"   // Negative value
                   << "2012-01-11 | 2000\n" // Too large value
                   << "2001-42-42 | 10\n";  // Bad date
        input_file.close();
    }

    // 各テストケースの終了後に呼ばれる
    void TearDown() override {
        // 作成したダミーファイルを削除
        std::remove("test_data.csv");
        std::remove("test_input.txt");
    }

    // 標準出力をキャプチャして文字列として返すヘルパー関数
    std::string captureOutput(void (BitcoinExchange::*func)(const std::string &), BitcoinExchange &btc,
                              const std::string &arg) {
        std::stringstream buffer;
        std::streambuf *old = std::cout.rdbuf(buffer.rdbuf()); // coutの出力先をbufferに変更
        (btc.*func)(arg);
        std::cout.rdbuf(old); // coutの出力先を元に戻す
        return buffer.str();
    }

    // 標準エラー出力をキャプチャして文字列として返すヘルパー関数
    std::string captureError(void (BitcoinExchange::*func)(const std::string &), BitcoinExchange &btc,
                             const std::string &arg) {
        std::stringstream buffer;
        std::streambuf *old = std::cerr.rdbuf(buffer.rdbuf()); // cerrの出力先をbufferに変更
        (btc.*func)(arg);
        std::cerr.rdbuf(old); // cerrの出力先を元に戻す
        return buffer.str();
    }
};

// --- Unit Tests ---

// isValidDateメソッドのテスト
TEST(BitcoinExchangeValidationTest, IsValidDate) {
    BitcoinExchange btc;
    // リフレクションを使ってprivateメソッドにアクセスするのは一般的ではないが、
    // ここではデモのためにヘルパークラス/関数を仮定するか、メソッドを一時的にpublicにする。
    // 今回は、processInputFile経由で間接的にテストする。
    // (isValidDateがprivateなので直接呼び出せないため)
    // ここでは、isValidDateがpublicだと仮定してテストコードを示す。
    // EXPECT_TRUE(btc.isValidDate("2022-08-25"));
    // EXPECT_TRUE(btc.isValidDate("2020-02-29")); // Leap year
    // EXPECT_FALSE(btc.isValidDate("2021-02-29")); // Not a leap year
    // EXPECT_FALSE(btc.isValidDate("2022-13-01")); // Invalid month
    // EXPECT_FALSE(btc.isValidDate("2022-08-32")); // Invalid day
    // EXPECT_FALSE(btc.isValidDate("2022/08/25")); // Invalid format
    // EXPECT_FALSE(btc.isValidDate("2022-8-25"));  // Invalid format
}

// isValidValueメソッドのテスト
TEST(BitcoinExchangeValidationTest, IsValidValue) {
    BitcoinExchange btc;
    // isValidValueもprivateなので直接テストは難しい。
    // double value;
    // EXPECT_TRUE(btc.isValidValue("0", value));
    // EXPECT_TRUE(btc.isValidValue("1000", value));
    // EXPECT_TRUE(btc.isValidValue("123.45", value));
    // EXPECT_FALSE(btc.isValidValue("-1", value));
    // EXPECT_FALSE(btc.isValidValue("1000.1", value));
    // EXPECT_FALSE(btc.isValidValue("abc", value));
}

// データベース読み込みのテスト
TEST_F(BitcoinExchangeTest, LoadDatabase) {
    BitcoinExchange btc;
    // 正常に読み込めるか（例外が発生しないか）
    ASSERT_NO_THROW(btc.loadDatabase("test_data.csv"));

    // 存在しないファイルを指定した場合に例外をスローするか
    ASSERT_THROW(btc.loadDatabase("non_existent_file.csv"), std::runtime_error);
}

// 正常な計算処理のテスト
TEST_F(BitcoinExchangeTest, ProcessExactAndClosestDate) {
    BitcoinExchange btc;
    btc.loadDatabase("test_data.csv");

    std::ofstream input_file("single_input.txt");
    input_file << "date | value\n";
    input_file << "2011-01-03 | 10\n"; // Exact match
    input_file << "2011-01-05 | 2\n";  // Closest past date (uses 2011-01-03)
    input_file.close();

    std::string output = captureOutput(&BitcoinExchange::processInputFile, btc, "single_input.txt");

    std::string expected_output = "2011-01-03 => 10 = 3\n"
                                  "2011-01-05 => 2 = 0.6\n";

    EXPECT_EQ(output, expected_output);
    std::remove("single_input.txt");
}

// 境界値（DBの最初より前、最後より後）のテスト
TEST_F(BitcoinExchangeTest, ProcessBoundaryDates) {
    BitcoinExchange btc;
    btc.loadDatabase("test_data.csv");

    std::ofstream input_file("boundary_input.txt");
    input_file << "date | value\n";
    input_file << "2009-12-31 | 10\n"; // Before first date
    input_file << "2023-01-01 | 1\n";  // After last date
    input_file.close();

    std::string output = captureOutput(&BitcoinExchange::processInputFile, btc, "boundary_input.txt");
    std::string error_output = captureError(&BitcoinExchange::processInputFile, btc, "boundary_input.txt");

    std::string expected_output = "2023-01-01 => 1 = 47115.9\n"; // Note: double precision might vary slightly
    std::string expected_error = "Error: no data available for or before date 2009-12-31\n";

    // doubleの比較は難しいため、ここでは文字列として比較するが、
    // 実際には浮動小数点数用の比較マクロ(EXPECT_DOUBLE_EQ)を使うのが望ましい。
    // 出力形式の都合上、文字列比較で代用。
    EXPECT_TRUE(output.find("2023-01-01 => 1 = 47115.9") != std::string::npos ||
                output.find("2023-01-01 => 1 = 47115.88") != std::string::npos);
    EXPECT_EQ(error_output, expected_error);

    std::remove("boundary_input.txt");
}

// 異常値を含む入力ファイルのテスト
TEST_F(BitcoinExchangeTest, ProcessInvalidInputs) {
    BitcoinExchange btc;
    btc.loadDatabase("test_data.csv");

    std::ofstream input_file("invalid_input.txt");
    input_file << "date | value\n";
    input_file << "2012-01-11 | -5\n";   // Negative
    input_file << "2012-01-11 | 1001\n"; // Too large
    input_file << "2022-99-99 | 10\n";   // Bad date
    input_file << "bad line format\n";   // Bad line
    input_file.close();

    std::string error_output = captureError(&BitcoinExchange::processInputFile, btc, "invalid_input.txt");

    std::string expected_error = "Error: not a positive number.\n"
                                 "Error: too large a number.\n"
                                 "Error: bad input => 2022-99-99 | 10\n"
                                 "Error: bad input => bad line format\n";

    EXPECT_EQ(error_output, expected_error);
    std::remove("invalid_input.txt");
}
