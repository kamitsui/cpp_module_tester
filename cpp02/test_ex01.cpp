/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamitsui <kamitsui@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 22:26:30 by kamitsui          #+#    #+#             */
/*   Updated: 2025/05/01 16:04:15 by kamitsui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fixed.hpp"
#include <cmath>
#include <iomanip> // std::fixed, std::setprecision
#include <iostream>
#include <vector>

// 整数を受け取るコンストラクタのテストケース構造体
struct IntTestCase {
    int input;
    float expectedFloat;
    int expectedInt;
};

// 浮動小数点数を受け取るコンストラクタのテストケース構造体
struct FloatTestCase {
    float input;
    float expectedFloat;
    int expectedInt;
};

void runIntConstructorTests() {
    std::cout << "\n--- Int Constructor Tests ---" << std::endl;
    std::vector<IntTestCase> intTests = {
        {0, 0.0f, 0},    {10, 10.0f, 10},       {-5, -5.0f, -5},         {1, 1.0f, 1},
        {-1, -1.0f, -1}, {1234, 1234.0f, 1234}, {-5678, -5678.0f, -5678} // テストケースはここに追加・修正できます
    };

    for (const auto &test : intTests) {
        Fixed fixed(test.input);
        std::cout << "Input: " << test.input << std::endl;
        std::cout << "  toFloat(): " << std::fixed << std::setprecision(6) << fixed.toFloat()
                  << " (Expected: " << test.expectedFloat << ")" << std::endl;
        std::cout << "  toInt(): " << fixed.toInt() << " (Expected: " << test.expectedInt << ")" << std::endl;
    }
}

void runFloatConstructorTests() {
    std::cout << "\n--- Float Constructor Tests ---" << std::endl;
    static const float numsF[] = {
        0.0f, 10.5f, -5.2f, 1.99f, 42.42f, -3.14159f, 1234.5678f,
    };
    // 1.99f		->	-1に丸められるケース
    // -1.99f		->	-2に丸められるケース
    // 42.42f		->	浮動小数点数の精度による近似
    // -3.14159f	->	浮動小数点数の精度による近似
    // 1234.5678f	->	浮動小数点数の精度による近似と丸め
    //// テストケースはここに追加・修正できます

    std::vector<FloatTestCase> floatTests = {
        {numsF[0], numsF[0], static_cast<int>(std::floor(numsF[0]))},
        {numsF[1], numsF[1], static_cast<int>(std::floor(numsF[1]))},
        {numsF[2], numsF[2], static_cast<int>(std::floor(numsF[2]))},
        {numsF[3], numsF[3], static_cast<int>(std::floor(numsF[3]))},
        {numsF[4], numsF[4], static_cast<int>(std::floor(numsF[4]))},
        {numsF[5], numsF[5], static_cast<int>(std::floor(numsF[5]))},
        {numsF[6], numsF[6], static_cast<int>(std::floor(numsF[6]))},
        //// テストケースはここに追加・修正できます
    };

    for (const auto &test : floatTests) {
        Fixed fixed(test.input);
        std::cout << "Input: " << test.input << std::endl;
        std::cout << "  toFloat(): " << std::fixed << std::setprecision(6) << fixed.toFloat()
                  << " (Expected: " << test.expectedFloat << ")" << std::endl;
        std::cout << "  toInt(): " << fixed.toInt() << " (Expected: " << test.expectedInt << ")" << std::endl;
    }
}

int main(void) {
    runIntConstructorTests();
    runFloatConstructorTests();

    return 0;
}
