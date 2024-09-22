/// @file apriori_tests.cpp
/// @brief Unit test for the Apriori algorithm.
///
/// @author Roland Abel
/// @date December 8, 2023
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
#include "apriori.h"

using namespace fim::itemset;
using namespace fim::algorithms::apriori;

using std::views::transform;

class AprioriTests : public ::testing::Test {
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

    static size_t min_support() { return 4; }

    static database_t get_database() {
        return database_t{
                {Milk,   Cheese, Butter, Bread,  Sugar,  Flour, Cream},
                {Cheese, Butter, Bread,  Coffee, Sugar,  Flour},
                {Milk,   Butter, Coffee, Sugar,  Flour},
                {Milk,   Cream,  Butter},
                {Milk,   Butter, Coffee},
                {Milk,   Flour},
                {Milk,   Cheese, Butter, Bread,  Coffee, Sugar, Flour},
                {Cream},
                {Milk,   Cheese, Butter, Sugar},
                {Milk,   Cheese, Bread,  Coffee, Sugar,  Flour}
        }.sort_database();
    }
};

TEST_F(AprioriTests, AllFrequentOneItemsetsTest) {
    const auto &db = get_database();
    const auto &itemsets = all_frequent_1_itemsets(db, min_support());

    // 1-itemsets
    ASSERT_EQ(itemsets.size(), 7);

    EXPECT_TRUE(itemsets.contains(Milk));
    EXPECT_TRUE(itemsets.contains(Bread));
    EXPECT_TRUE(itemsets.contains(Cheese));
    EXPECT_TRUE(itemsets.contains(Butter));
    EXPECT_TRUE(itemsets.contains(Coffee));
    EXPECT_TRUE(itemsets.contains(Sugar));
    EXPECT_TRUE(itemsets.contains(Flour));

    EXPECT_FALSE(itemsets.contains(Cream));
}

TEST_F(AprioriTests, GenerateCandidatesTest) {
    const itemsets_t itemsets = {
            {Milk,   Cheese},
            {Milk,   Butter},
            {Milk,   Coffee},
            {Milk,   Sugar},
            {Milk,   Flour},
            {Bread,  Cheese},
            {Bread,  Sugar},
            {Bread,  Flour},
            {Cheese, Butter},
            {Cheese, Sugar},
            {Cheese, Flour},
            {Butter, Coffee}
    };

    auto candidates = generate_candidates(itemsets, 3);

    ASSERT_EQ(candidates.size(), 6);
    EXPECT_TRUE(candidates.contains({Milk, Cheese, Butter}));
    EXPECT_TRUE(candidates.contains({Milk, Butter, Coffee}));
    EXPECT_TRUE(candidates.contains({Milk, Cheese, Sugar}));
    EXPECT_TRUE(candidates.contains({Milk, Butter, Coffee}));
    EXPECT_TRUE(candidates.contains({Bread, Cheese, Sugar}));
    EXPECT_TRUE(candidates.contains({Bread, Cheese, Flour}));
}

TEST_F(AprioriTests, AprioriAlgorithmTest) {
    const auto &db = get_database();
    const auto &itemsets = apriori_algorithm(db, min_support()).sort_each_itemset();

    ASSERT_EQ(itemsets.size(), 35);

    // 1-itemsets
    EXPECT_TRUE(itemsets.contains({Milk}));
    EXPECT_TRUE(itemsets.contains({Bread}));
    EXPECT_TRUE(itemsets.contains({Cheese}));
    EXPECT_TRUE(itemsets.contains({Butter}));
    EXPECT_TRUE(itemsets.contains({Coffee}));
    EXPECT_TRUE(itemsets.contains({Sugar}));
    EXPECT_TRUE(itemsets.contains({Flour}));

    // 2-itemsets
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

    // 3-itemsets
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

    // 4-itemsets
    EXPECT_TRUE(itemsets.contains({Bread, Cheese, Sugar, Flour}));
}
