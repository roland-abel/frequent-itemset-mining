/// @file fp_growth.h
/// @brief
///
/// @author Roland Abel
/// @date August 10, 2024
///
/// Copyright (c) 2024 Roland Abel
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

#pragma once

#include <memory>
#include <algorithm>
#include <utility>
#include <ranges>
#include <optional>
#include "dtypes.h"
#include "fp_tree.h"

namespace rules::fp_growth {

    using namespace rules::fp_tree;

    using item_t = rules::item_t;
    using itemset_t = rules::itemset_t;
    using itemsets_t = rules::itemsets_t;
    using database_t = rules::database_t;

    /// Compare function for item
    using itemset_compare_fn = std::function<bool(const item_t &x, const item_t &y)>;

    ///
    /// @param item_counts
    /// @return
    auto item_compare(const item_counts_t &item_counts) -> itemset_compare_fn;

    // Filter function
    using itemset_filter_fn = std::function<items_t(const itemset_t &)>;

    ///
    /// @param item_counts
    /// @param min_support
    /// @return
    auto filter_items(const item_counts_t &item_counts, size_t min_support) -> itemset_filter_fn;

    ///
    /// @param database
    /// @return
    auto get_item_counts(const database_t &database) -> item_counts_t;

    /// Gets a sorted list of frequent items from the given transactions.
    /// @param item_counts
    /// @param min_support The minimum support threshold for items to be considered frequent.
    /// @return A list of items that exceed the minimum support threshold, sorted in order of frequency.
    auto get_frequent_items(const item_counts_t &item_counts, size_t min_support) -> std::pair<items_t, item_counts_t>;

    /// Creates the power set of the given set of items.
    /// @param items The input itemset for which the power set is to created.
    /// @return The set of all subsets of the given items.
    auto power_set(const itemset_t &items, bool include_empty_set = true) -> itemsets_t;

    /// Insert a specified item into each subset of the given itemsets.
    /// @param itemsets The set of itemsets to be expanded.
    /// @param item The item to be added to each subset within the itemsets.
    /// @return A new set of itemsets where each original subset has been expanded with the given item.
    auto insert_into_each_itemsets(const itemsets_t &itemsets, item_t item) -> itemsets_t;

    /// Sorts the items in the x according to the order defined by the frequent items list
    /// and removes any items not in the frequent items list.
    /// @param itemset The original set of items to be filtered.
    /// @param frequent_items A list of items considered frequent.
    /// @return A list of items from the x that are also in the frequent items list, maintaining their order.
    auto filter_and_sort_items(const itemset_t &itemset, const items_t &frequent_items) -> items_t;

    //
    using filter_and_sort_fn = std::function<items_t(const itemset_t &itemset)>;

    ///
    /// @param item_counts
    /// @param min_support
    /// @return
    auto filter_and_sort_items_(const item_counts_t &item_counts, size_t min_support) -> filter_and_sort_fn;

    /// Builds a FP-tree from the given database based on frequent items and minimum support.
    /// @param database A collection of transactions, where each transaction is a set of items.
    /// @param filter_and_sort
    /// @return A shared pointer to the root node of the constructed FP-tree.
    auto build_fp_tree(const database_t &database, const filter_and_sort_fn &filter_and_sort) -> node_ptr;

    /// Builds a FP-tree from the given database based on the minimum support.
    /// @param database A collection of transactions, where each transaction is a set of items.
    /// @param min_support The minimum support threshold for an item to be considered frequent.
    /// @return A shared pointer to the root node of the constructed FP-tree.
    auto build_fp_tree(const database_t &database, size_t min_support) -> node_ptr;

    /// Generates the conditional transaction from the given FP-Tree.
    /// @param node The current node in the FP-Tree being processed.
    /// @param item  The item for which conditional database_ are being generated.
    /// @return The database_ that will be populated with the conditional database_.
    auto conditional_transactions(const node_ptr &node, item_t item) -> database_t;

    /// Applies the FP-Growth algorithm to find frequent itemsets from the given database_.
    /// @param database A collection of transactions, where each transaction is a set of items.
    /// @param min_support The minimum support threshold for an itemset to be considered frequent.
    /// @return A set of frequent itemsets, where each itemset is a collection of items that meet
    /// the minimum support threshold.
    auto fp_growth_algorithm(const database_t &database, size_t min_support) -> itemsets_t;
}