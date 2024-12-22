/// @file itemset_tests.cpp
/// @brief Unit test for the Apriori algorithm.
///
/// @author Roland Abel
/// @date September 22, 2024
///
/// Copyright (c) 2023 Roland Abel
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

class ItemsetTests : public testing::Test {
protected:
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

TEST_F(ItemsetTests, SortTest) {
    auto x = itemset_t{6, 3, 8, 2};

    EXPECT_TRUE(itemset_t({1, 4, 6}).contains(4));
    EXPECT_TRUE(itemset_t({1, 5, 7}).contains(5));

    EXPECT_FALSE(itemset_t({}).contains(5));
    EXPECT_FALSE(itemset_t({1, 3, 7}).contains(5));
}

TEST_F(ItemsetTests, ContainsTest) {
    EXPECT_TRUE(itemset_t({1, 4, 6}).contains(4));
    EXPECT_TRUE(itemset_t({1, 5, 7}).contains(5));

    EXPECT_FALSE(itemset_t({}).contains(5));
    EXPECT_FALSE(itemset_t({1, 3, 7}).contains(5));
}

TEST_F(ItemsetTests, IsSubsetTest) {
    EXPECT_TRUE(is_subset({}, {}));
    EXPECT_TRUE(is_subset({94}, {94}));
    EXPECT_TRUE(is_subset({6, 7}, {4, 6, 7}));
    EXPECT_TRUE(is_subset({6, 7}, {6, 5, 7, 4}));

    EXPECT_FALSE(is_subset({4}, {}));
    EXPECT_FALSE(is_subset({1, 4}, {}));
    EXPECT_FALSE(is_subset({7, 6}, {6, 7}));
    EXPECT_FALSE(is_subset({1, 7}, {}));
    EXPECT_FALSE(is_subset({7, 1}, {4, 6, 7}));
    EXPECT_FALSE(is_subset({7, 1}, {1, 5, 6}));
}

TEST_F(ItemsetTests, IsSubsetMemberTest) {
    EXPECT_TRUE(itemset_t{}.is_subset({}));
    EXPECT_TRUE(itemset_t({6, 7}).is_subset({4, 6, 7}));
    EXPECT_TRUE(itemset_t({6, 7}).is_subset({6, 5, 7, 4}));

    EXPECT_FALSE(itemset_t({1, 4}).is_subset({}));
    EXPECT_FALSE(itemset_t({7, 6}).is_subset({6, 7}));
    EXPECT_FALSE(itemset_t({1, 7}).is_subset({}));
    EXPECT_FALSE(itemset_t({7, 1}).is_subset({4, 6, 7}));
    EXPECT_FALSE(itemset_t({7, 1}).is_subset({1, 5, 6}));
}

TEST_F(ItemsetTests, SetDifferenceTest) {
    EXPECT_EQ(set_difference({1, 5, 2}, {1, 5, 2}), itemset_t{});
    EXPECT_EQ(set_difference({1, 5, 2}, {5}), itemset_t({1, 2}));
    EXPECT_EQ(set_difference({5, 2, 6}, {5, 6, 1}), itemset_t({2}));
}

TEST_F(ItemsetTests, SetUnionTest) {
    EXPECT_EQ(set_union({1, 5, 2}, {}), itemset_t({1, 5, 2}));
    EXPECT_EQ(set_union({1, 5, 2}, {5}), itemset_t({1, 5, 2}));
    EXPECT_EQ(set_union({5, 2, 6}, {5, 6, 1}), itemset_t({5, 2, 6, 1}));
}

TEST_F(ItemsetTests, HashCodeTest) {
    constexpr auto hash = itemset_hash();

    const auto code1 = hash(itemset_t{5, 1, 2}.sort_itemset(default_item_compare));
    const auto code2 = hash(itemset_t{1, 5, 2}.sort_itemset(default_item_compare));

    const auto code3 = hash(itemset_t{5, 1, 2});
    const auto code4 = hash(itemset_t{5, 1, 6});

    EXPECT_EQ(code1, code2);
    EXPECT_NE(code3, code4);
}
