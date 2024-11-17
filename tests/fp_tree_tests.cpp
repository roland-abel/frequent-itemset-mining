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

class FPTreeTests : public ::testing::Test {
protected:
    static size_t min_support() { return 4; }

    static database_t get_database() {
        itemsets_t database = {
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

        return database
               | transform([](itemset_t& x) { return x.sort_itemset(); })
               | std::ranges::to<database_t>();
    }
};


TEST_F(FPTreeTests, NodeIsRootTest) {
    EXPECT_TRUE(node_t::create_root()->is_root());
}

TEST_F(FPTreeTests, NodeIsNotRootTest) {
    const auto& root = node_t::create_root();
    const auto& child = root->add_child(1, 1);

    EXPECT_FALSE(child->is_root());
}

TEST_F(FPTreeTests, RootNodeIsValidTest) {
    const auto& root = node_t::create_root();

    EXPECT_EQ(root->item, 0);
    EXPECT_EQ(root->frequency, 0);
    EXPECT_TRUE(root->children.empty());
}

TEST_F(FPTreeTests, AddChildTest) {
    const auto& root = node_t::create_root();

    const auto& node1 = root->add_child(1, 3);
    const auto& node2 = root->add_child(6, 2);

    EXPECT_EQ(node1->item, 1);
    EXPECT_EQ(node1->frequency, 3);
    EXPECT_TRUE(node1->children.empty());

    EXPECT_EQ(node2->item, 6);
    EXPECT_EQ(node2->frequency, 2);
    EXPECT_TRUE(node2->children.empty());

    EXPECT_EQ(root->children.size(), 2);
}

TEST_F(FPTreeTests, FindChildItemTest) {
    const auto& root = node_t::create_root();
    root->add_child(1, 3);
    root->add_child(6, 2);
    root->add_child(5, 2);

    auto child = root->find_child_item(6);

    EXPECT_TRUE(child.has_value());
    EXPECT_EQ(child.value()->frequency, 2);
}

TEST_F(FPTreeTests, ChildNotFoundTest) {
    const auto& root = node_t::create_root();
    root->add_child(1, 3);
    root->add_child(6, 2);
    root->add_child(5, 2);

    EXPECT_FALSE(root->find_child_item(7).has_value());
}

TEST_F(FPTreeTests, TreeHasSinglePathTest) {
    const auto& root = node_t::create_root();
    root->add_child(1, 5)->add_child(3, 4)->add_child(5, 2);

    const auto& items_along_path = tree_is_single_path(root);
    EXPECT_TRUE(items_along_path.has_value());

    const auto& path = items_along_path.value();
    EXPECT_FALSE(path.contains(root->item));
    EXPECT_TRUE(path.contains(1));
    EXPECT_TRUE(path.contains(3));
    EXPECT_TRUE(path.contains(5));
}

TEST_F(FPTreeTests, TreeHasNoSinglePathTest) {
    const auto& root = node_t::create_root();

    const auto& child = root->add_child(1, 8)->add_child(3, 5);
    child->add_child(4, 4)->add_child(7, 1);
    child->add_child(5, 3)->add_child(3, 1);

    const auto& items_along_path = tree_is_single_path(root);
    EXPECT_FALSE(items_along_path.has_value());
}

TEST_F(FPTreeTests, GetItemFrequencyTest) {
    const auto [db, item_counts] = get_database().transaction_reduction(min_support());
    const auto &freq_items = item_counts.get_frequent_items(min_support());

    const auto& root = build_fp_tree(get_database(), freq_items);

    EXPECT_EQ(get_item_frequency(root, 1), 8);
    EXPECT_EQ(get_item_frequency(root, 4), 7);
    EXPECT_EQ(get_item_frequency(root, 6), 6);
    EXPECT_EQ(get_item_frequency(root, 7), 6);
    EXPECT_EQ(get_item_frequency(root, 3), 5);
    EXPECT_EQ(get_item_frequency(root, 5), 5);
    EXPECT_EQ(get_item_frequency(root, 2), 4);
    EXPECT_EQ(get_item_frequency(root, 8), 0);
}

TEST_F(FPTreeTests, PowerSetTest) {
    const auto& items = itemset_t{1, 4};
    const auto& p = power_set(items, false);

    EXPECT_EQ(p.size(), 3);
    EXPECT_TRUE(p.contains({1}));
    EXPECT_TRUE(p.contains({4}));
    EXPECT_TRUE(p.contains({1, 4}));
}

TEST_F(FPTreeTests, PowerSetExcludeEmptySetTest) {
    const auto& items = itemset_t{1, 4, 5};
    const auto& p = power_set(items, true);

    EXPECT_EQ(p.size(), 8);
    EXPECT_TRUE(p.contains(itemset_t{}));
    EXPECT_TRUE(p.contains({1}));
    EXPECT_TRUE(p.contains({4}));
    EXPECT_TRUE(p.contains({5}));
    EXPECT_TRUE(p.contains({1, 4}));
    EXPECT_TRUE(p.contains({1, 5}));
    EXPECT_TRUE(p.contains({4, 5}));
    EXPECT_TRUE(p.contains({1, 4, 5}));
}

TEST_F(FPTreeTests, InsertIntoAllItemsetsTest) {
    const auto& items = itemset_t{1, 4, 5};
    const auto& result = insert_into_each_itemsets(power_set(items), 6);

    EXPECT_EQ(result.size(), 8);
    EXPECT_TRUE(result.contains({6}));
    EXPECT_TRUE(result.contains({1, 6}));
    EXPECT_TRUE(result.contains({4, 6}));
    EXPECT_TRUE(result.contains({5, 6}));
    EXPECT_TRUE(result.contains({1, 4, 6}));
    EXPECT_TRUE(result.contains({1, 5, 6}));
    EXPECT_TRUE(result.contains({4, 5, 6}));
    EXPECT_TRUE(result.contains({1, 4, 5, 6}));
}

//TEST_F(FPTreeTests, GetFrequentItemsTest) {
//    const auto [db, item_counts] = get_database().transaction_reduction(min_support());
//    const auto &freq_items = item_counts.get_frequent_items(min_support());
//
//    const auto& item_counts = get_item_counts(get_database());
//    const auto& [items, frequencies] = get_ordered_frequent_items(item_counts, min_support);
//
//    ASSERT_EQ(items.size(), 7);
//    EXPECT_EQ(items[0], 1);
//    EXPECT_EQ(items[1], 4);
//    EXPECT_EQ(items[2], 6);
//    EXPECT_EQ(items[3], 7);
//    EXPECT_EQ(items[4], 3);
//    EXPECT_EQ(items[5], 5);
//    EXPECT_EQ(items[6], 2);
//
//    ASSERT_EQ(frequencies.size(), 8);
//    EXPECT_EQ(frequencies.at(1), 8);
//    EXPECT_EQ(frequencies.at(4), 7);
//    EXPECT_EQ(frequencies.at(6), 6);
//    EXPECT_EQ(frequencies.at(7), 6);
//    EXPECT_EQ(frequencies.at(3), 5);
//    EXPECT_EQ(frequencies.at(5), 5);
//    EXPECT_EQ(frequencies.at(2), 4);
//    EXPECT_EQ(frequencies.at(8), 2);
//}

//TEST_F(FPTreeTests, SortAndFilterItems1Test) {
//    const auto min_support = 4;
//
//    const auto& item_counts = get_item_counts(get_database());
//    const auto& [frequent_items, _] = get_ordered_frequent_items(item_counts, min_support);
//    const itemset_t& itemset = {1, 2, 3, 4, 5, 6, 7, 8};
//
//    const auto& items = filter_and_sort_items(itemset, frequent_items);
//
//    ASSERT_EQ(items.size(), 7);
//    EXPECT_EQ(items[0], 1);
//    EXPECT_EQ(items[1], 4);
//    EXPECT_EQ(items[2], 6);
//    EXPECT_EQ(items[3], 7);
//    EXPECT_EQ(items[4], 3);
//    EXPECT_EQ(items[5], 5);
//    EXPECT_EQ(items[6], 2);
//}
//
//TEST_F(FPTreeTests, SortAndFilterItems2Test) {
//    const auto& frequent_items = items_t{1, 4, 6, 7, 3, 5, 2};
//    const itemset_t& itemset = {5, 8, 4, 3};
//
//    const auto& items = filter_and_sort_items(itemset, frequent_items);
//
//    EXPECT_EQ(items.size(), 3);
//    EXPECT_EQ(items[0], 4);
//    EXPECT_EQ(items[1], 3);
//    EXPECT_EQ(items[2], 5);
//}

TEST_F(FPTreeTests, BuildFpTreeFormEmptyTransactionsTest) {
    const auto [db, item_counts] = get_database().transaction_reduction(min_support());
    const auto &freq_items = item_counts.get_frequent_items(min_support());

    const auto& root = build_fp_tree(database_t{}, freq_items);

    EXPECT_EQ(root->children.size(), 0);
    EXPECT_EQ(root->item, 0);
    EXPECT_EQ(root->frequency, 0);
    EXPECT_EQ(root->parent.lock(), nullptr);
}

TEST_F(FPTreeTests, BuildFpTreeTest) {
    const auto [db, item_counts] = get_database().transaction_reduction(min_support());
    const auto &freq_items = item_counts.get_frequent_items(min_support());

    const auto& root = build_fp_tree(get_database(), freq_items);
    ASSERT_EQ(root->children.size(), 2);

    EXPECT_TRUE(root->has_path_with_frequencies({1, 7}, {8, 1}));
    EXPECT_TRUE(root->has_path_with_frequencies({1, 6, 7, 3, 5, 2}, {8, 1, 1, 1, 1, 1}));
    EXPECT_TRUE(root->has_path_with_frequencies({1, 4, 5}, {8, 6, 1}));
    EXPECT_TRUE(root->has_path_with_frequencies({1, 4, 6, 3}, {8, 6, 4, 1}));
    EXPECT_TRUE(root->has_path_with_frequencies({1, 4, 6, 7, 3, 5, 2}, {8, 6, 4, 3, 2, 1, 1}));
    EXPECT_TRUE(root->has_path_with_frequencies({1, 4, 6, 7, 5}, {8, 6, 4, 3, 1}));
    EXPECT_TRUE(root->has_path_with_frequencies({4, 6, 7, 3, 5, 2}, {1, 1, 1, 1, 1, 1}));
    EXPECT_TRUE(root->has_path_with_frequencies({1, 4, 6, 7, 3, 2}, {8, 6, 4, 3, 2, 1}));
}
