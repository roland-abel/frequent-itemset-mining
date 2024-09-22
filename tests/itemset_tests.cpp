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
#include "itemset.h"

using std::views::transform;
using namespace fim::itemset;

class ItemsetTests : public ::testing::Test {
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

TEST_F(ItemsetTests, SortTest) {
    auto x = itemset_t{Sugar, Cheese, Cream, Bread};

    EXPECT_TRUE(itemset_t({Milk, Butter, Sugar}).contains(Butter));
    EXPECT_TRUE(itemset_t({Milk, Coffee, Flour}).contains(Coffee));

    EXPECT_FALSE(itemset_t({}).contains(Coffee));
    EXPECT_FALSE(itemset_t({Milk, Cheese, Flour}).contains(Coffee));
}

TEST_F(ItemsetTests, ContainsTest) {
    EXPECT_TRUE(itemset_t({Milk, Butter, Sugar}).contains(Butter));
    EXPECT_TRUE(itemset_t({Milk, Coffee, Flour}).contains(Coffee));

    EXPECT_FALSE(itemset_t({}).contains(Coffee));
    EXPECT_FALSE(itemset_t({Milk, Cheese, Flour}).contains(Coffee));
}

TEST_F(ItemsetTests, IsSubsetTest) {
    EXPECT_TRUE(is_subset({}, {}));
    EXPECT_TRUE(is_subset({Sugar, Flour}, {Butter, Sugar, Flour}));
    EXPECT_TRUE(is_subset({Sugar, Flour}, {Sugar, Coffee, Flour, Butter}));

    EXPECT_FALSE(is_subset({Milk, Butter}, {}));
    EXPECT_FALSE(is_subset({Flour, Sugar}, {Sugar, Flour}));
    EXPECT_FALSE(is_subset({Milk, Flour}, {}));
    EXPECT_FALSE(is_subset({Flour, Milk}, {Butter, Sugar, Flour}));
    EXPECT_FALSE(is_subset({Flour, Milk}, {Milk, Coffee, Sugar}));
}

TEST_F(ItemsetTests, SetDifferenceTest) {
    EXPECT_EQ(set_difference({Milk, Coffee, Bread}, {Milk, Coffee, Bread}), itemset_t{});
    EXPECT_EQ(set_difference({Milk, Coffee, Bread}, {Coffee}), itemset_t({Milk, Bread}));
    EXPECT_EQ(set_difference({Coffee, Bread, Sugar}, {Coffee, Sugar, Milk}), itemset_t({Bread}));
}

TEST_F(ItemsetTests, SetUnionTest) {
    EXPECT_EQ(set_union({Milk, Coffee, Bread}, {}), itemset_t({Milk, Coffee, Bread}));
    EXPECT_EQ(set_union({Milk, Coffee, Bread}, {Coffee}), itemset_t({Milk, Coffee, Bread}));
    EXPECT_EQ(set_union({Coffee, Bread, Sugar}, {Coffee, Sugar, Milk}), itemset_t({Coffee, Bread, Sugar, Milk}));
}

TEST_F(ItemsetTests, HashCodeTest) {
    auto hash = itemset_hash();

    const auto code1 = hash(itemset_t{Coffee, Milk, Bread}.sort());
    const auto code2 = hash(itemset_t{Milk, Coffee, Bread}.sort());

    const auto code3 = hash(itemset_t{Coffee, Milk, Bread});
    const auto code4 = hash(itemset_t{Coffee, Milk, Sugar});

    EXPECT_EQ(code1, code2);
    EXPECT_NE(code3, code4);
}

TEST_F(ItemsetTests, GetItemCountsTest) {
    const auto &database = get_database();
    const auto &counts = item_count_t::get_item_count(database);

    ASSERT_EQ(counts.size(), 8);

    EXPECT_EQ(counts.at(Milk), 8);
    EXPECT_EQ(counts.at(Butter), 7);
    EXPECT_EQ(counts.at(Sugar), 6);
    EXPECT_EQ(counts.at(Flour), 6);
    EXPECT_EQ(counts.at(Cheese), 5);
    EXPECT_EQ(counts.at(Coffee), 5);
    EXPECT_EQ(counts.at(Bread), 4);
    EXPECT_EQ(counts.at(Cream), 2);
}

TEST_F(ItemsetTests, GetFrequentItemsTest) {
    const auto min_support = 4;
    const auto &database = get_database();

    const auto &counts = item_count_t::get_item_count(database);
    const itemset_t &items = counts.get_frequent_items(min_support);

    ASSERT_EQ(items.size(), 7);

    EXPECT_EQ(items[0], Milk);
    EXPECT_EQ(items[1], Butter);
    EXPECT_EQ(items[2], Sugar);
    EXPECT_EQ(items[3], Flour);
    EXPECT_EQ(items[4], Cheese);
    EXPECT_EQ(items[5], Coffee);
    EXPECT_EQ(items[6], Bread);
}