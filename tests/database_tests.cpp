/// @file database_tests.cpp
/// @brief Unit test class.
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

class DatabaseTests : public testing::Test {
protected:
    static database_t get_database() {
        return database_t{
            {3, 4, 2, 5, 6, 7},
            {1, 3, 4, 6},
            {5, 1, 4, 6, 8, 7},
            {1, 4},
            {1, 4, 5},
            {7, 1},
            {2, 3, 4, 1, 5, 6, 7},
            {8},
            {1, 3, 2, 5, 6, 7},
            {8, 3, 4, 6, 2, 7, 1}
        };
    }
};

TEST_F(DatabaseTests, TransactionReductionDatabaseTest) {
    constexpr auto min_support = 4;
    const auto &[db, counts] = get_database().reduce_database(min_support);

    ASSERT_EQ(counts.size(), 7);

    // high support -> low support
    // items: 1 > 4 > 6 >= 7 > 3 >= 5 > 2 > 8
    EXPECT_EQ(counts.at(1), 8);
    EXPECT_EQ(counts.at(4), 7);
    EXPECT_EQ(counts.at(6), 6);
    EXPECT_EQ(counts.at(7), 6);
    EXPECT_EQ(counts.at(3), 5);
    EXPECT_EQ(counts.at(5), 5);
    EXPECT_EQ(counts.at(2), 4);

    ASSERT_EQ(db.size(), 9);

    EXPECT_EQ(db[0], itemset_t({2, 3, 5, 6, 7, 4, 1}));
    EXPECT_EQ(db[1], itemset_t({2, 3, 5, 6, 7, 4}));
    EXPECT_EQ(db[2], itemset_t({2, 3, 5, 6, 7, 1}));
    EXPECT_EQ(db[3], itemset_t({2, 3, 6, 7, 4, 1}));
    EXPECT_EQ(db[4], itemset_t({3, 6, 4, 1}));
    EXPECT_EQ(db[5], itemset_t({5, 6, 7, 4, 1}));
    EXPECT_EQ(db[6], itemset_t({5, 4, 1}));
    EXPECT_EQ(db[7], itemset_t({7, 1}));
    EXPECT_EQ(db[8], itemset_t({4, 1}));
}

TEST_F(DatabaseTests, GetFrequentItemsTest) {
    constexpr auto min_support = 4;
    const auto &[db, counts] = get_database().reduce_database(min_support);

    const auto freq_items = counts
            .get_frequent_items(min_support)
            .sort_itemset(counts.get_item_compare());

    EXPECT_EQ(freq_items, itemset_t({2, 3, 5, 6, 7, 4, 1}));
}
