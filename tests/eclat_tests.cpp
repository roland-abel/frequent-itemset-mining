/// @file eclat_tests.cpp
/// @brief Unit test for the Eclat algorithm.
///
/// @author Roland Abel
/// @date September 8, 2024
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
#include "eclat.h"

using namespace fim;
using namespace fim::algorithms::eclat;

using std::views::transform;

class EclatTests : public ::testing::Test {
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
        };
    }
};

TEST_F(EclatTests, ToVerticalTransactionsTest) {
    const auto &trans = get_database();
    const auto &vertical_trans = to_vertical_database(trans);

    ASSERT_EQ(trans.size(), 10);
    ASSERT_EQ(vertical_trans.size(), 8);

    EXPECT_EQ(vertical_trans.at(Milk), (tidset_t{0, 2, 3, 4, 5, 6, 8, 9}));
    EXPECT_EQ(vertical_trans.at(Bread), (tidset_t{0, 1, 6, 9}));
    EXPECT_EQ(vertical_trans.at(Cheese), (tidset_t{0, 1, 6, 8, 9}));
    EXPECT_EQ(vertical_trans.at(Butter), (tidset_t{0, 1, 2, 3, 4, 6, 8}));
    EXPECT_EQ(vertical_trans.at(Coffee), (tidset_t{1, 2, 4, 6, 9}));
    EXPECT_EQ(vertical_trans.at(Sugar), (tidset_t{0, 1, 2, 6, 8, 9}));
    EXPECT_EQ(vertical_trans.at(Flour), (tidset_t{0, 1, 2, 5, 6, 9}));
    EXPECT_EQ(vertical_trans.at(Cream), (tidset_t{0, 7}));
}

TEST_F(EclatTests, SetIntersectionOfTidsTest) {
    EXPECT_EQ(set_intersection({}, {}), (tidset_t{}));
    EXPECT_EQ(set_intersection({}, {0, 1, 2, 3, 6}), (tidset_t{}));
    EXPECT_EQ(set_intersection({0, 1, 2, 3}, {3, 4, 5, 6}), (tidset_t{3}));
    EXPECT_EQ(set_intersection({0, 3, 2, 3, 6, 6}, {0, 3, 2, 3, 6}), (tidset_t{0, 3, 2, 6}));
    EXPECT_EQ(set_intersection({0, 1, 3, 5, 9}, {0, 1, 2, 3, 6, 8, 9}), (tidset_t{0, 1, 3, 9}));
}

TEST_F(EclatTests, EclatAlgorithmTest) {
    const auto min_support = 4;
    const auto &database = get_database();

    const auto &itemsets = eclat_algorithm(database, min_support).sort_each_itemset();
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
}