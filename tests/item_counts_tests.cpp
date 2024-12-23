/// @file item_counts_tests.cpp
/// @brief
///
/// @author Roland Abel
/// @date December 11, 2024
///
/// Copyright (c) 2024 Roland Abel
///
/// This software is released under the MIT License.
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// with the Software without restriction, including without limitation the rights
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
#include "itemset.h"
#include "database.h"

using std::views::transform;
using namespace fim;

class ItemsetCountsTests : public testing::Test {
protected:
    static size_t min_support() { return 4; }

    static database_t get_database() {
        return database_t{
            {1, 3, 4, 2, 6, 7, 8},
            {3, 4, 2, 5, 6, 7},
            {1, 4, 5, 6, 7},
            {1, 4},
            {1, 4, 5},
            {1, 7},
            {1, 3, 4, 2, 5, 6, 7},
            {8},
            {1, 3, 4, 6},
            {1, 3, 2, 5, 6, 7}
        };
    }
};

TEST_F(ItemsetCountsTests, GetItemCountsTest) {
    const auto &db = get_database();
    const auto &counts = db.get_item_counts();

    ASSERT_EQ(counts.size(), 8);

    EXPECT_EQ(counts.at(1), 8);
    EXPECT_EQ(counts.at(4), 7);
    EXPECT_EQ(counts.at(6), 6);
    EXPECT_EQ(counts.at(7), 6);
    EXPECT_EQ(counts.at(3), 5);
    EXPECT_EQ(counts.at(5), 5);
    EXPECT_EQ(counts.at(2), 4);
    EXPECT_EQ(counts.at(8), 2);
}

TEST_F(ItemsetCountsTests, GetFrequentItemsTest) {
    const auto &db = get_database();

    const auto &counts = db.get_item_counts();
    const itemset_t &items = counts.get_frequent_items(min_support());

    ASSERT_EQ(items.size(), 7);

    EXPECT_EQ(items[0], 1);
    EXPECT_EQ(items[1], 4);
    EXPECT_EQ(items[2], 6);
    EXPECT_EQ(items[3], 7);
    EXPECT_EQ(items[4], 3);
    EXPECT_EQ(items[5], 5);
    EXPECT_EQ(items[6], 2);
}

TEST_F(ItemsetCountsTests, GetCountTest) {
    const auto &[db, item_counts] = get_database().transaction_reduction(min_support());
    const auto compare = item_counts.get_item_compare();

    itemsets_t candidates{
        {4, 3, 1},
        {1, 4, 5},
        {1, 3, 6},
        {3, 2, 7}
    };

    candidates.sort_each_itemset(compare);
    const auto &counts = itemset_counts_t::create_itemset_counts(db, candidates, compare);

    auto contains = [&](const itemset_t &itemset) -> bool {
        return counts.contains(itemset.sort_itemset(compare));
    };

    auto get_count = [&](const itemset_t &itemset) -> size_t {
        return counts.get_count(itemset.sort_itemset(compare));
    };

    ASSERT_EQ(counts.size(), 4);

    EXPECT_TRUE(contains({2, 3, 7}));
    EXPECT_EQ(get_count({2, 3, 7}), 4);

    EXPECT_TRUE(contains({3, 4, 1}));
    EXPECT_EQ(get_count({3, 4, 1}), 3);

    EXPECT_TRUE(contains({3, 6, 1}));
    EXPECT_EQ(get_count({3, 6, 1}), 4);

    EXPECT_TRUE(contains({5, 4, 1}));
    EXPECT_EQ(get_count({5, 4, 1}), 3);
}

TEST_F(ItemsetCountsTests, GetSupportTest) {
    constexpr auto eps = 0.1f;
    const auto &[db, item_counts] = get_database().transaction_reduction(min_support());
    const auto compare = item_counts.get_item_compare();

    itemsets_t candidates{
        {4, 3, 1},
        {1, 4, 5},
        {1, 3, 6},
        {3, 2, 7}
    };

    candidates.sort_each_itemset(compare);
    const auto &counts = itemset_counts_t::create_itemset_counts(db, candidates, compare);

    auto get_support = [&](const itemset_t &itemset) -> float {
        return counts.get_support(itemset.sort_itemset(compare), db.size());
    };

    ASSERT_EQ(counts.size(), 4);

    EXPECT_FLOAT_EQ(get_support({2, 3, 7}), 0.44444445f);
    EXPECT_FLOAT_EQ(get_support({3, 4, 1}), 0.33333334f);
    EXPECT_FLOAT_EQ(get_support({3, 6, 1}), 0.44444445f);
    EXPECT_FLOAT_EQ(get_support({5, 4, 1}), 0.33333334f);
}

TEST_F(ItemsetCountsTests, ItemCompareTest) {
    const auto &db = get_database();
    const auto &counts = db.get_item_counts();
    const auto compare = counts.get_item_compare();

    const itemset_t &items = counts.get_frequent_items(min_support());

    //  1 > 4 > 7 > 6 > 5 > 3 > 2 > 8 (ascending order)
    EXPECT_FALSE(compare(1, 4));
    EXPECT_TRUE(compare(4, 1));
    EXPECT_FALSE(compare(7, 6));
    EXPECT_TRUE(compare(6, 7));
    EXPECT_FALSE(compare(5, 3));
    EXPECT_TRUE(compare(3, 5));
    EXPECT_FALSE(compare(2, 8));
    EXPECT_TRUE(compare(8, 2));
}

TEST_F(ItemsetCountsTests, ItemCompareReverseTest) {
    const auto &db = get_database();
    const auto &counts = db.get_item_counts();
    const auto compare = counts.get_item_reverse_compare();

    //  8 > 2 > 3 > 5 > 6 > 7 > 4 > 1 (descending order)
    EXPECT_TRUE(compare(1, 4));
    EXPECT_FALSE(compare(4, 1));
    EXPECT_TRUE(compare(7, 6));
    EXPECT_FALSE(compare(6, 7));
    EXPECT_TRUE(compare(5, 3));
    EXPECT_FALSE(compare(3, 5));
    EXPECT_TRUE(compare(2, 8));
    EXPECT_FALSE(compare(8, 2));
}
