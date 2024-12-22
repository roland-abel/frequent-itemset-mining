/// @file frequent_itemset_tests.cpp
/// @brief Unit test class.
///
/// @author Roland Abel
/// @date December 8, 2024
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
#include <writer.h>
#include "itemset.h"
#include "algorithms.h"

using namespace fim;
using namespace fim::algorithm;

class FrequentItemsetTests : public testing::TestWithParam<algorithm_function_t> {
public:
    FrequentItemsetTests() = default;

protected:
    static algorithm_function_t get_algorthm() { return GetParam(); }

    static size_t min_support() { return 4; }

    static database_t get_database() {
        return database_t{
            {3, 1, 4, 2, 6, 7, 8},
            {3, 4, 2, 5, 6, 7},
            {1, 4, 5, 6, 7},
            {1, 8, 4},
            {1, 4, 5},
            {1, 7},
            {1, 3, 4, 2, 5, 6, 7},
            {8},
            {1, 3, 4, 6},
            {1, 3, 2, 5, 6, 7}
        };
    }
};

TEST_P(FrequentItemsetTests, ApplyAlgorithm) {
    const auto [db, item_counts] = get_database().transaction_reduction(min_support());
    const auto compare = item_counts.get_item_compare();

    const auto &freq_items = get_algorthm()({db, item_counts}, min_support()).sort_each_itemset(compare);
    const auto &counts = itemset_counts_t::create_itemset_counts(db, freq_items, compare);

    auto verify = [&](const itemset_t &itemset, const size_t support = 0) -> bool {
        return freq_items.contains(itemset.sort_itemset(compare)) && counts.get_count(itemset) == support;
    };

    ASSERT_EQ(freq_items.size(), 35);
    ASSERT_EQ(counts.size(), 35);

    // 1-suffix
    EXPECT_TRUE(verify({2}, 4));
    EXPECT_TRUE(verify({3}, 5));
    EXPECT_TRUE(verify({5}, 5));
    EXPECT_TRUE(verify({6}, 6));
    EXPECT_TRUE(verify({7}, 6));
    EXPECT_TRUE(verify({4}, 7));
    EXPECT_TRUE(verify({1}, 8));

    // 2-suffix
    EXPECT_TRUE(verify({2, 3}, 4));
    EXPECT_TRUE(verify({2, 6}, 4));
    EXPECT_TRUE(verify({2, 7}, 4));
    EXPECT_TRUE(verify({3, 6}, 5));
    EXPECT_TRUE(verify({3, 7}, 4));
    EXPECT_TRUE(verify({3, 4}, 4));
    EXPECT_TRUE(verify({3, 1}, 4));
    EXPECT_TRUE(verify({5, 6}, 4));
    EXPECT_TRUE(verify({5, 7}, 4));
    EXPECT_TRUE(verify({5, 4}, 4));
    EXPECT_TRUE(verify({5, 1}, 4));
    EXPECT_TRUE(verify({6, 7}, 5));
    EXPECT_TRUE(verify({6, 4}, 5));
    EXPECT_TRUE(verify({6, 1}, 5));
    EXPECT_TRUE(verify({7, 4}, 4));
    EXPECT_TRUE(verify({7, 1}, 5));
    EXPECT_TRUE(verify({4, 1}, 6));

    // 3-suffix
    EXPECT_TRUE(verify({2, 3, 6}, 4));
    EXPECT_TRUE(verify({2, 3, 7}, 4));
    EXPECT_TRUE(verify({2, 6, 7}, 4));
    EXPECT_TRUE(verify({3, 6, 7}, 4));
    EXPECT_TRUE(verify({3, 6, 4}, 4));
    EXPECT_TRUE(verify({3, 6, 1}, 4));
    EXPECT_TRUE(verify({5, 6, 7}, 4));
    EXPECT_TRUE(verify({6, 7, 4}, 4));
    EXPECT_TRUE(verify({6, 7, 1}, 4));
    EXPECT_TRUE(verify({6, 4, 1}, 4));

    // 4-suffix
    EXPECT_TRUE(verify({2, 3, 6, 7}, 4));
}

INSTANTIATE_TEST_SUITE_P(
    FrequentItemsetTests,
    FrequentItemsetTests,
    ::testing::Values(
        get_algorithm(algorithm_t::APRIORI),
        get_algorithm(algorithm_t::FP_GROWTH),
        get_algorithm(algorithm_t::ECLAT),
        get_algorithm(algorithm_t::RELIM))
);
