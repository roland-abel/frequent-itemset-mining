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
using namespace fim::algorithm::fp_growth;

using std::views::transform;

class FPGrowthTests : public ::testing::Test {
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

TEST_F(FPGrowthTests, ConditionalTransactions1Test) {
    const auto [db, item_counts] = get_database().transaction_reduction(min_support());
    const auto &freq_items = item_counts.get_frequent_items(min_support());

    const auto& root = build_fp_tree(db, freq_items);
    const auto& trans = conditional_transactions(root, 7);

    ASSERT_EQ(trans.size(), 6);
    EXPECT_EQ(trans[0], (itemset_t{1, 4, 6}));
    EXPECT_EQ(trans[1], (itemset_t{1, 4, 6}));
    EXPECT_EQ(trans[2], (itemset_t{1, 4, 6}));
    EXPECT_EQ(trans[3], (itemset_t{{1, 6}}));
    EXPECT_EQ(trans[4], itemset_t{1});
    EXPECT_EQ(trans[5], (itemset_t{{4, 6}}));
}

TEST_F(FPGrowthTests, ConditionalTransactions2Test) {
    const auto [db, item_counts] = get_database().transaction_reduction(min_support());
    const auto &freq_items = item_counts.get_frequent_items(min_support());

    const auto& root = build_fp_tree(db, freq_items);
    const auto& trans = conditional_transactions(root, 2);

    ASSERT_EQ(trans.size(), 4);
    EXPECT_EQ(trans[0], (itemset_t{1, 3, 4, 5, 6, 7}));
    EXPECT_EQ(trans[1], (itemset_t{1, 3, 4, 6, 7}));
    EXPECT_EQ(trans[2], (itemset_t{1, 3, 5, 6, 7}));
    EXPECT_EQ(trans[3], (itemset_t{3, 4, 5, 6, 7}));
}
