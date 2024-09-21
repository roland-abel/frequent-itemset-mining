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
#include "apriori_2.h"
//#include "test_data.h"

using namespace fim::apriori_2;

class Apriori2Tests : public ::testing::Test {
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

    fim::apriori_2::database_t get_database() {
        itemsets_t database = {
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

        auto sort_itemset = [](itemset_t &x) -> itemset_t {
            std::ranges::sort(x);
            return x;
        };

        return database
               | std::views::transform(sort_itemset)
               | std::ranges::to<itemsets_t>();
    }
};

TEST_F(Apriori2Tests, IsSubsetTest) {
    EXPECT_TRUE(is_subset({}, {}));
    EXPECT_TRUE(is_subset({Sugar, Flour}, {Butter, Sugar, Flour}));
    EXPECT_TRUE(is_subset({Sugar, Flour}, {Sugar, Coffee, Flour, Butter}));

    EXPECT_FALSE(is_subset({Milk, Butter}, {}));
    EXPECT_FALSE(is_subset({Flour, Sugar}, {Sugar, Flour}));
    EXPECT_FALSE(is_subset({Milk, Flour}, {}));
    EXPECT_FALSE(is_subset({Flour, Milk}, {Butter, Sugar, Flour}));
    EXPECT_FALSE(is_subset({Flour, Milk}, {Milk, Coffee, Sugar}));
}

TEST_F(Apriori2Tests, AllFrequentOneItemsetsTest) {
    const auto min_support = 4;
    const auto &db = get_database();

    const auto &itemsets = all_frequent_1_itemsets(db, min_support);
    auto contains = [&](const item_t &x) {
        return std::ranges::contains(itemsets, itemset_t{x});
    };

    // 1-itemsets
    ASSERT_EQ(itemsets.size(), 7);

    EXPECT_TRUE(contains(Milk));
    EXPECT_TRUE(contains(Bread));
    EXPECT_TRUE(contains(Cheese));
    EXPECT_TRUE(contains(Butter));
    EXPECT_TRUE(contains(Coffee));
    EXPECT_TRUE(contains(Sugar));
    EXPECT_TRUE(contains(Flour));

    EXPECT_FALSE(contains(Cream));
}

TEST_F(Apriori2Tests, GenerateCandidatesTest) {
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
    auto contains = [&](const itemset_t &x) {
        return std::ranges::contains(candidates, x);
    };

    ASSERT_EQ(candidates.size(), 6);
    EXPECT_TRUE(contains({Milk, Cheese, Butter}));
    EXPECT_TRUE(contains({Milk, Butter, Coffee}));
    EXPECT_TRUE(contains({Milk, Cheese, Sugar}));
    EXPECT_TRUE(contains({Milk, Butter, Coffee}));
    EXPECT_TRUE(contains({Bread, Cheese, Sugar}));
    EXPECT_TRUE(contains({Bread, Cheese, Flour}));
}

TEST_F(Apriori2Tests, Apriori2AlgorithmTest) {
    const auto min_support = 4;
    const auto &db = get_database();

    const auto &frequent_itemsets = fim::apriori_2::apriori(db, min_support);
    ASSERT_EQ(frequent_itemsets.size(), 35);

    auto contains = [&](const itemset_t &x) {
        return std::ranges::contains(frequent_itemsets, x);
    };

    // 1-itemsets
    EXPECT_TRUE(contains({Milk}));
    EXPECT_TRUE(contains({Bread}));
    EXPECT_TRUE(contains({Cheese}));
    EXPECT_TRUE(contains({Butter}));
    EXPECT_TRUE(contains({Coffee}));
    EXPECT_TRUE(contains({Sugar}));
    EXPECT_TRUE(contains({Flour}));

    // 2-itemsets
    EXPECT_TRUE(contains({Milk, Cheese}));
    EXPECT_TRUE(contains({Milk, Butter}));
    EXPECT_TRUE(contains({Milk, Coffee}));
    EXPECT_TRUE(contains({Milk, Sugar}));
    EXPECT_TRUE(contains({Milk, Flour}));
    EXPECT_TRUE(contains({Bread, Cheese}));
    EXPECT_TRUE(contains({Bread, Sugar}));
    EXPECT_TRUE(contains({Bread, Flour}));
    EXPECT_TRUE(contains({Cheese, Butter}));
    EXPECT_TRUE(contains({Cheese, Sugar}));
    EXPECT_TRUE(contains({Cheese, Flour}));
    EXPECT_TRUE(contains({Butter, Coffee}));
    EXPECT_TRUE(contains({Butter, Sugar}));
    EXPECT_TRUE(contains({Butter, Flour}));
    EXPECT_TRUE(contains({Coffee, Sugar}));
    EXPECT_TRUE(contains({Coffee, Flour}));
    EXPECT_TRUE(contains({Sugar, Flour}));

    // 3-itemsets
    EXPECT_TRUE(contains({Milk, Cheese, Sugar}));
    EXPECT_TRUE(contains({Milk, Butter, Sugar}));
    EXPECT_TRUE(contains({Milk, Sugar, Flour}));
    EXPECT_TRUE(contains({Bread, Cheese, Sugar}));
    EXPECT_TRUE(contains({Bread, Cheese, Flour}));
    EXPECT_TRUE(contains({Bread, Sugar, Flour}));
    EXPECT_TRUE(contains({Cheese, Butter, Sugar}));
    EXPECT_TRUE(contains({Cheese, Sugar, Flour}));
    EXPECT_TRUE(contains({Butter, Sugar, Flour}));
    EXPECT_TRUE(contains({Coffee, Sugar, Flour}));

    // 4-itemsets
    EXPECT_TRUE(contains({Bread, Cheese, Sugar, Flour}));
}
