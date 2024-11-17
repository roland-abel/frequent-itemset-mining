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

using namespace fim;
using namespace fim::algorithm::apriori;

using std::views::transform;

class AprioriTests : public ::testing::Test {
protected:
    static size_t min_support() { return 4; }

    static database_t get_database() {
        return database_t{
                {3, 4, 2, 5, 6, 7},
                {1, 3, 4, 6},
                {1, 4, 5, 6, 7},
                {1, 4},
                {1, 4, 5},
                {1, 7},
                {1, 3, 4, 2, 5, 6, 7},
                {8},
                {1, 3, 2, 5, 6, 7},
                {8, 3, 4, 6, 2, 7, 1}
        };
    }
};

TEST_F(AprioriTests, AllFrequentOneItemsetsTest) {
    const auto [db, item_counts] = get_database().transaction_reduction(min_support());
    const auto& itemsets = all_frequent_one_itemsets(item_counts, min_support());

    // 1-suffix
    ASSERT_EQ(itemsets.size(), 7);

    EXPECT_TRUE(itemsets.contains(1));
    EXPECT_TRUE(itemsets.contains(2));
    EXPECT_TRUE(itemsets.contains(3));
    EXPECT_TRUE(itemsets.contains(4));
    EXPECT_TRUE(itemsets.contains(5));
    EXPECT_TRUE(itemsets.contains(6));
    EXPECT_TRUE(itemsets.contains(7));

    EXPECT_FALSE(itemsets.contains(8));
}

TEST_F(AprioriTests, GenerateCandidatesTest) {
    const itemsets_t itemsets = {
            {1, 3},
            {1, 4},
            {1, 5},
            {1, 6},
            {1, 7},
            {2, 3},
            {2, 6},
            {2, 7},
            {3, 4},
            {3, 6},
            {3, 7},
            {4, 5}
    };

    auto candidates = generate_candidates(itemsets, 3, default_item_compare);

    ASSERT_EQ(candidates.size(), 6);
    EXPECT_TRUE(candidates.contains({1, 3, 4}));
    EXPECT_TRUE(candidates.contains({1, 4, 5}));
    EXPECT_TRUE(candidates.contains({1, 3, 6}));
    EXPECT_TRUE(candidates.contains({1, 4, 5}));
    EXPECT_TRUE(candidates.contains({2, 3, 6}));
    EXPECT_TRUE(candidates.contains({2, 3, 7}));
}
