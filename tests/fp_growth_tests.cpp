/// @file fp_growth_tests.cpp
/// @brief Unit test for the FP-Growth algorithm.
///
/// @author Roland Abel
/// @date August 10, 2024
///
/// Copyright (c) 2023 Roland Abel
///
/// This software is released under the MIT License.
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.

#include <gtest/gtest.h>
#include <ranges>
#include "fp_growth.h"

using namespace fim;
using namespace fim::fp_growth;

using std::views::transform;

class FPGrowthTests : public ::testing::Test {
protected:
    enum Items {
        Milk = 1,
        Bread,
        Cheese,
        Butter,
        Coffee,
        Sugar,
        Flour,
        Cream
    };

    static database_t get_database() {
        return database_t{
                {Milk,   Cheese, Butter, Bread,  Sugar,  Flour, Cream},
                {Cheese, Butter, Bread,  Coffee, Sugar,  Flour},
                {Milk,   Butter, Coffee, Sugar,  Flour},
                {Milk,   Butter},
                {Milk,   Butter, Coffee},
                {Milk,   Flour},
                {Milk,   Cheese, Butter, Bread,  Coffee, Sugar, Flour},
                {Cream},
                {Milk,   Cheese, Butter, Sugar},
                {Milk,   Cheese, Bread,  Coffee, Sugar,  Flour}
        }.sort_database();
    }
};

TEST_F(FPGrowthTests, FpGrowthAlgorithmTest) {
    const auto min_support = 4;
    const auto &db = get_database();

    const auto &itemsets = fp_growth_algorithm(db, min_support).sort_each_itemset();
    EXPECT_EQ(itemsets.size(), 35);

    // 1-suffix
    EXPECT_TRUE(itemsets.contains({Milk}));
    EXPECT_TRUE(itemsets.contains({Bread}));
    EXPECT_TRUE(itemsets.contains({Cheese}));
    EXPECT_TRUE(itemsets.contains({Butter}));
    EXPECT_TRUE(itemsets.contains({Coffee}));
    EXPECT_TRUE(itemsets.contains({Sugar}));
    EXPECT_TRUE(itemsets.contains({Flour}));

    // 2-suffix
    EXPECT_TRUE(itemsets.contains({Milk, Cheese}));
    EXPECT_TRUE(itemsets.contains({Milk, Butter}));
    EXPECT_TRUE(itemsets.contains({Milk, Coffee}));
    EXPECT_TRUE(itemsets.contains({Milk, Sugar}));
    EXPECT_TRUE(itemsets.contains({Milk, Flour}));
    EXPECT_TRUE(itemsets.contains({Bread, Cheese}));
    EXPECT_TRUE(itemsets.contains({Bread, Sugar}));
    EXPECT_TRUE(itemsets.contains({Bread, Flour}));
    EXPECT_TRUE(itemsets.contains({Cheese, Butter}));
    EXPECT_TRUE(itemsets.contains({Cheese, Sugar}));
    EXPECT_TRUE(itemsets.contains({Cheese, Flour}));
    EXPECT_TRUE(itemsets.contains({Butter, Coffee}));
    EXPECT_TRUE(itemsets.contains({Butter, Sugar}));
    EXPECT_TRUE(itemsets.contains({Butter, Flour}));
    EXPECT_TRUE(itemsets.contains({Coffee, Sugar}));
    EXPECT_TRUE(itemsets.contains({Coffee, Flour}));
    EXPECT_TRUE(itemsets.contains({Sugar, Flour}));

    // 3-suffix
    EXPECT_TRUE(itemsets.contains({Milk, Cheese, Sugar}));
    EXPECT_TRUE(itemsets.contains({Milk, Butter, Sugar}));
    EXPECT_TRUE(itemsets.contains({Milk, Sugar, Flour}));
    EXPECT_TRUE(itemsets.contains({Bread, Cheese, Sugar}));
    EXPECT_TRUE(itemsets.contains({Bread, Cheese, Flour}));
    EXPECT_TRUE(itemsets.contains({Bread, Sugar, Flour}));
    EXPECT_TRUE(itemsets.contains({Cheese, Butter, Sugar}));
    EXPECT_TRUE(itemsets.contains({Cheese, Sugar, Flour}));
    EXPECT_TRUE(itemsets.contains({Butter, Sugar, Flour}));
    EXPECT_TRUE(itemsets.contains({Coffee, Sugar, Flour}));

    // 4-suffix
    EXPECT_TRUE(itemsets.contains({Bread, Cheese, Sugar, Flour}));

    EXPECT_FALSE(itemsets.contains({Milk, Cheese, Sugar, Flour}));
    EXPECT_FALSE(itemsets.contains({Milk, Butter, Sugar, Flour}));
}