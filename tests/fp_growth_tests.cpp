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
        } | transform([](itemset_t &x) { return x.sort(); }) | std::ranges::to<database_t>();
    }
};

TEST_F(FPGrowthTests, FpGrowthAlgorithmTest) {
    auto sort_each_itemset = [](itemset_t &x) -> itemset_t { return x.sort(); };

    const auto min_support = 4;
    const auto &db = get_database();

    const auto &frequent_itemsets = fp_growth_algorithm(db, min_support)
                                    | transform(sort_each_itemset)
                                    | std::ranges::to<itemsets_t>();

    EXPECT_EQ(frequent_itemsets.size(), 35);

    // 1-itemsets
    EXPECT_TRUE(frequent_itemsets.contains({Milk}));
    EXPECT_TRUE(frequent_itemsets.contains({Bread}));
    EXPECT_TRUE(frequent_itemsets.contains({Cheese}));
    EXPECT_TRUE(frequent_itemsets.contains({Butter}));
    EXPECT_TRUE(frequent_itemsets.contains({Coffee}));
    EXPECT_TRUE(frequent_itemsets.contains({Sugar}));
    EXPECT_TRUE(frequent_itemsets.contains({Flour}));

    // 2-itemsets
    EXPECT_TRUE(frequent_itemsets.contains({Milk, Cheese}));
    EXPECT_TRUE(frequent_itemsets.contains({Milk, Butter}));
    EXPECT_TRUE(frequent_itemsets.contains({Milk, Coffee}));
    EXPECT_TRUE(frequent_itemsets.contains({Milk, Sugar}));
    EXPECT_TRUE(frequent_itemsets.contains({Milk, Flour}));
    EXPECT_TRUE(frequent_itemsets.contains({Bread, Cheese}));
    EXPECT_TRUE(frequent_itemsets.contains({Bread, Sugar}));
    EXPECT_TRUE(frequent_itemsets.contains({Bread, Flour}));
    EXPECT_TRUE(frequent_itemsets.contains({Cheese, Butter}));
    EXPECT_TRUE(frequent_itemsets.contains({Cheese, Sugar}));
    EXPECT_TRUE(frequent_itemsets.contains({Cheese, Flour}));
    EXPECT_TRUE(frequent_itemsets.contains({Butter, Coffee}));
    EXPECT_TRUE(frequent_itemsets.contains({Butter, Sugar}));
    EXPECT_TRUE(frequent_itemsets.contains({Butter, Flour}));
    EXPECT_TRUE(frequent_itemsets.contains({Coffee, Sugar}));
    EXPECT_TRUE(frequent_itemsets.contains({Coffee, Flour}));
    EXPECT_TRUE(frequent_itemsets.contains({Sugar, Flour}));

    // 3-itemsets
    EXPECT_TRUE(frequent_itemsets.contains({Milk, Cheese, Sugar}));
    EXPECT_TRUE(frequent_itemsets.contains({Milk, Butter, Sugar}));
    EXPECT_TRUE(frequent_itemsets.contains({Milk, Sugar, Flour}));
    EXPECT_TRUE(frequent_itemsets.contains({Bread, Cheese, Sugar}));
    EXPECT_TRUE(frequent_itemsets.contains({Bread, Cheese, Flour}));
    EXPECT_TRUE(frequent_itemsets.contains({Bread, Sugar, Flour}));
    EXPECT_TRUE(frequent_itemsets.contains({Cheese, Butter, Sugar}));
    EXPECT_TRUE(frequent_itemsets.contains({Cheese, Sugar, Flour}));
    EXPECT_TRUE(frequent_itemsets.contains({Butter, Sugar, Flour}));
    EXPECT_TRUE(frequent_itemsets.contains({Coffee, Sugar, Flour}));

    // 4-itemsets
    EXPECT_TRUE(frequent_itemsets.contains({Bread, Cheese, Sugar, Flour}));

    EXPECT_FALSE(frequent_itemsets.contains({Milk, Cheese, Sugar, Flour}));
    EXPECT_FALSE(frequent_itemsets.contains({Milk, Butter, Sugar, Flour}));
}