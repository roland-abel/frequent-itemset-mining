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
#include "test_data.h"

using namespace rules;
using namespace rules::apriori;
using namespace rules::fp_growth;
using namespace rules::tests;

class FPGrowthTests : public ::testing::Test {
protected:
};

TEST_F(FPGrowthTests, NodeIsRootTest) {
    EXPECT_TRUE(node_t::create_root()->is_root());
}

TEST_F(FPGrowthTests, NodeIsNotRootTest) {
    const auto &root = node_t::create_root();
    const auto &child = root->add_child(Milk, 1);

    EXPECT_FALSE(child->is_root());
}

TEST_F(FPGrowthTests, RootNodeIsValidTest) {
    const auto &root = node_t::create_root();

    EXPECT_EQ(root->item, 0);
    EXPECT_EQ(root->frequency, 0);
    EXPECT_TRUE(root->children.empty());
}

TEST_F(FPGrowthTests, AddChildTest) {
    const auto &root = node_t::create_root();

    const auto &node1 = root->add_child(Milk, 3);
    const auto &node2 = root->add_child(Sugar, 2);

    EXPECT_EQ(node1->item, Milk);
    EXPECT_EQ(node1->frequency, 3);
    EXPECT_TRUE(node1->children.empty());

    EXPECT_EQ(node2->item, Sugar);
    EXPECT_EQ(node2->frequency, 2);
    EXPECT_TRUE(node2->children.empty());

    EXPECT_EQ(root->children.size(), 2);
}

TEST_F(FPGrowthTests, FindChildItemTest) {
    const auto &root = node_t::create_root();
    root->add_child(Milk, 3);
    root->add_child(Sugar, 2);
    root->add_child(Coffee, 2);

    auto child = root->find_child_item(Sugar);

    EXPECT_TRUE(child.has_value());
    EXPECT_EQ(child.value()->frequency, 2);
}

TEST_F(FPGrowthTests, ChildNotFoundTest) {
    const auto &root = node_t::create_root();
    root->add_child(Milk, 3);
    root->add_child(Sugar, 2);
    root->add_child(Coffee, 2);

    EXPECT_FALSE(root->find_child_item(Flour).has_value());
}

TEST_F(FPGrowthTests, TreeHasSinglePathTest) {
    const auto &root = node_t::create_root();
    root->add_child(Milk, 5)->add_child(Cheese, 4)->add_child(Coffee, 2);

    const auto &[has_single_path, items_along_path] = tree_has_single_path(root);
    EXPECT_TRUE(has_single_path);

    EXPECT_FALSE(items_along_path.contains(root->item));
    EXPECT_TRUE(items_along_path.contains(Milk));
    EXPECT_TRUE(items_along_path.contains(Cheese));
    EXPECT_TRUE(items_along_path.contains(Coffee));
}

TEST_F(FPGrowthTests, TreeHasNoSinglePathTest) {
    const auto &root = node_t::create_root();

    const auto &child = root->add_child(Milk, 8)->add_child(Cheese, 5);
    child->add_child(Butter, 4)->add_child(Flour, 1);
    child->add_child(Coffee, 3)->add_child(Cheese, 1);

    const auto &[has_single_path, items_along_path] = tree_has_single_path(root);
    EXPECT_FALSE(has_single_path);
    EXPECT_TRUE(items_along_path.empty());
}

TEST_F(FPGrowthTests, PowerSetTest) {
    const auto &items = itemset_t{Milk, Butter};
    const auto &p = power_set(items, false);

    EXPECT_EQ(p.size(), 3);
    EXPECT_TRUE(p.contains({Milk}));
    EXPECT_TRUE(p.contains({Butter}));
    EXPECT_TRUE(p.contains({Milk, Butter}));
}

TEST_F(FPGrowthTests, PowerSetExcludeEmptySetTest) {
    const auto &items = itemset_t{Milk, Butter, Coffee};
    const auto &p = power_set(items, true);

    EXPECT_EQ(p.size(), 8);
    EXPECT_TRUE(p.contains({}));
    EXPECT_TRUE(p.contains({Milk}));
    EXPECT_TRUE(p.contains({Butter}));
    EXPECT_TRUE(p.contains({Coffee}));
    EXPECT_TRUE(p.contains({Milk, Butter}));
    EXPECT_TRUE(p.contains({Milk, Coffee}));
    EXPECT_TRUE(p.contains({Butter, Coffee}));
    EXPECT_TRUE(p.contains({Milk, Butter, Coffee}));
}

TEST_F(FPGrowthTests, InsertIntoAllItemsetsTest) {
    const auto &items = itemset_t{Milk, Butter, Coffee};
    const auto &result = insert_into_all_itemsets(power_set(items), Sugar);

    EXPECT_EQ(result.size(), 8);
    EXPECT_TRUE(result.contains({Sugar}));
    EXPECT_TRUE(result.contains({Sugar, Milk}));
    EXPECT_TRUE(result.contains({Sugar, Butter}));
    EXPECT_TRUE(result.contains({Sugar, Coffee}));
    EXPECT_TRUE(result.contains({Sugar, Milk, Butter}));
    EXPECT_TRUE(result.contains({Sugar, Milk, Coffee}));
    EXPECT_TRUE(result.contains({Sugar, Butter, Coffee}));
    EXPECT_TRUE(result.contains({Sugar, Milk, Butter, Coffee}));
}

TEST_F(FPGrowthTests, FindFrequentItemsTest) {
    const auto min_support_abs = 4;
    const auto &[items, frequencies] = find_frequent_items(get_transactions(), min_support_abs);

    ASSERT_EQ(items.size(), 7);
    EXPECT_EQ(items[0], Milk);
    EXPECT_EQ(items[1], Butter);
    EXPECT_EQ(items[2], Sugar);
    EXPECT_EQ(items[3], Flour);
    EXPECT_EQ(items[4], Cheese);
    EXPECT_EQ(items[5], Coffee);
    EXPECT_EQ(items[6], Bread);

    ASSERT_EQ(frequencies.size(), 8);
    EXPECT_EQ(frequencies.at(Milk), 8);
    EXPECT_EQ(frequencies.at(Butter), 7);
    EXPECT_EQ(frequencies.at(Sugar), 6);
    EXPECT_EQ(frequencies.at(Flour), 6);
    EXPECT_EQ(frequencies.at(Cheese), 5);
    EXPECT_EQ(frequencies.at(Coffee), 5);
    EXPECT_EQ(frequencies.at(Bread), 4);
    EXPECT_EQ(frequencies.at(Cream), 2);
}

TEST_F(FPGrowthTests, SortAndFilterItems1Test) {
    const auto min_support_abs = 4;

    const auto &[frequent_items, _] = find_frequent_items(get_transactions(), min_support_abs);
    const itemset_t &itemset = {Milk, Bread, Cheese, Butter, Coffee, Sugar, Flour, Cream};

    const auto &items = filter_and_sort_items(itemset, frequent_items);

    ASSERT_EQ(items.size(), 7);
    EXPECT_EQ(items[0], Milk);
    EXPECT_EQ(items[1], Butter);
    EXPECT_EQ(items[2], Sugar);
    EXPECT_EQ(items[3], Flour);
    EXPECT_EQ(items[4], Cheese);
    EXPECT_EQ(items[5], Coffee);
    EXPECT_EQ(items[6], Bread);
}

TEST_F(FPGrowthTests, SortAndFilterItems2Test) {
    const auto &frequent_items = items_t{Milk, Butter, Sugar, Flour, Cheese, Coffee, Bread};
    const itemset_t &itemset = {Coffee, Cream, Butter, Cheese};

    const auto &items = filter_and_sort_items(itemset, frequent_items);

    EXPECT_EQ(items.size(), 3);
    EXPECT_EQ(items[0], Butter);
    EXPECT_EQ(items[1], Cheese);
    EXPECT_EQ(items[2], Coffee);
}

TEST_F(FPGrowthTests, GetFrequencyTest) {
    const auto min_support_abs = 4;
    const auto root = build_fp_tree(get_transactions(), min_support_abs);

    EXPECT_EQ(get_item_frequency(root, Milk), 8);
    EXPECT_EQ(get_item_frequency(root, Butter), 7);
    EXPECT_EQ(get_item_frequency(root, Sugar), 6);
    EXPECT_EQ(get_item_frequency(root, Flour), 6);
    EXPECT_EQ(get_item_frequency(root, Cheese), 5);
    EXPECT_EQ(get_item_frequency(root, Coffee), 5);
    EXPECT_EQ(get_item_frequency(root, Bread), 4);
    EXPECT_EQ(get_item_frequency(root, Cream), 0);
}

TEST_F(FPGrowthTests, BuildFpTreeFormEmptyTransactionsTest) {
    const auto min_support_abs = 4;
    const auto root = build_fp_tree(transactions_t{}, min_support_abs);

    EXPECT_EQ(root->children.size(), 0);
    EXPECT_EQ(root->item, 0);
    EXPECT_EQ(root->frequency, 0);
    EXPECT_EQ(root->parent.lock(), nullptr);
}

TEST_F(FPGrowthTests, BuildFpTreeTest) {
    const auto min_support_abs = 4;

    const auto root = build_fp_tree(get_transactions(), min_support_abs);
    ASSERT_EQ(root->children.size(), 2);

    EXPECT_TRUE(root->has_path_with_frequencies({Milk, Flour}, {8, 1}));
    EXPECT_TRUE(root->has_path_with_frequencies({Milk, Sugar, Flour, Cheese, Coffee, Bread}, {8, 1, 1, 1, 1, 1}));
    EXPECT_TRUE(root->has_path_with_frequencies({Milk, Butter, Coffee}, {8, 6, 1}));
    EXPECT_TRUE(root->has_path_with_frequencies({Milk, Butter, Sugar, Cheese}, {8, 6, 4, 1}));
    EXPECT_TRUE(root->has_path_with_frequencies({Milk, Butter, Sugar, Flour, Cheese, Coffee, Bread}, {8, 6, 4, 3, 2, 1, 1}));
    EXPECT_TRUE(root->has_path_with_frequencies({Milk, Butter, Sugar, Flour, Coffee}, {8, 6, 4, 3, 1}));
    EXPECT_TRUE(root->has_path_with_frequencies({Butter, Sugar, Flour, Cheese, Coffee, Bread}, {1, 1, 1, 1, 1, 1}));
    EXPECT_TRUE(root->has_path_with_frequencies({Milk, Butter, Sugar, Flour, Cheese, Bread}, {8, 6, 4, 3, 2, 1}));
}

TEST_F(FPGrowthTests, ConditionalTransactions1Test) {
    const auto min_support_abs = 4;
    const auto &root = build_fp_tree(get_transactions(), min_support_abs);
    const auto &trans = conditional_transactions(root, Flour);

    ASSERT_EQ(trans.size(), 6);
    EXPECT_EQ(trans[0], (itemset_t{Milk, Butter, Sugar}));
    EXPECT_EQ(trans[1], (itemset_t{Milk, Butter, Sugar}));
    EXPECT_EQ(trans[2], (itemset_t{Milk, Butter, Sugar}));
    EXPECT_EQ(trans[3], itemset_t{Milk});
    EXPECT_EQ(trans[4], (itemset_t{{Milk, Sugar}}));
    EXPECT_EQ(trans[5], (itemset_t{{Sugar, Butter}}));
}

TEST_F(FPGrowthTests, ConditionalTransactions2Test) {
    const auto min_support_abs = 4;
    const auto &root = build_fp_tree(get_transactions(), min_support_abs);
    const auto &trans = conditional_transactions(root, Bread);

    ASSERT_EQ(trans.size(), 4);
    EXPECT_EQ(trans[0], (itemset_t{Milk, Butter, Sugar, Flour, Cheese}));
    EXPECT_EQ(trans[1], (itemset_t{Milk, Butter, Sugar, Flour, Coffee, Cheese}));
    EXPECT_EQ(trans[2], (itemset_t{Milk, Sugar, Flour, Coffee, Cheese}));
    EXPECT_EQ(trans[3], (itemset_t{Butter, Sugar, Flour, Coffee, Cheese}));
}

TEST_F(FPGrowthTests, FpGrowthAlgorithmTest) {
    const auto min_support_abs = 4;
    const auto &transactions = get_transactions();

    const auto &itemsets = fp_growth_algorithm(transactions, min_support_abs);
    EXPECT_EQ(itemsets.size(), 35);

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
    EXPECT_TRUE(itemsets.contains({Bread, Cheese, Sugar, Flour})); // 1 2 5 6

    EXPECT_FALSE(itemsets.contains({Milk, Cheese, Sugar, Flour})); // 0 2 5 6
    EXPECT_FALSE(itemsets.contains({Milk, Butter, Sugar, Flour})); // 0 3 5 6
}