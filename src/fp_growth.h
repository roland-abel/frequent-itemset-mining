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

namespace rules::fp_growth {

    struct node_t;

    using item_t = rules::item_t;
    using itemset_t = rules::itemset_t;
    using itemsets_t = rules::itemsets_t;
    using transactions_t = rules::transactions_t;

    using items_t = std::vector<item_t>;
    using frequencies_t = std::unordered_map<item_t, size_t>;
    using node_ptr = std::shared_ptr<node_t>;
    using children_t = std::vector<node_ptr>;

    /// Represents a node in a FP-tree tree
    struct node_t : public std::enable_shared_from_this<node_t> {
        using parent_ptr = std::weak_ptr<node_t>;

        item_t item{};
        size_t frequency{};
        parent_ptr parent;
        children_t children{};

        /// Constructs a node with the given item, frequency, and optional parent.
        /// @param item The item to be stored in the node.
        /// @param frequency The frequency of the item.
        /// @param parent An optional shared pointer to the parent node.
        node_t(item_t item, size_t frequency, const std::shared_ptr<node_t> &parent = nullptr);

        /// Default constructor.
        node_t() = default;

        /// Checks if this node is the root node.
        /// @return True if this node is the root, otherwise false.
        bool is_root() const;

        /// Creates the root node_t for an FP-tree.
        /// @return A shared pointer to the newly created root node_t.
        static node_ptr create_root();

        /// Adds a child node with the specified item and frequency.
        /// @param child_item The item to be added as a child.
        /// @param child_frequency The frequency of the child item.
        /// @return A shared pointer to the newly added child node.
        node_ptr add_child(const item_t &child_item, size_t child_frequency);

        /// Finds a child node with the specified item.
        /// @param child_item The item to find in the children.
        /// @return An optional shared pointer to the child node if found; otherwise, std::nullopt
        std::optional<node_ptr> find_child_item(const item_t &child_item) const;

        /// Checks if there exists a path in the tree matching the given items and frequencies.
        /// @param items A vector of items to be matched in the tree path.
        /// @param frequencies A vector of frequencies corresponding to the items.
        /// @return True if a path matching all items and frequencies is found; otherwise, false.
        bool has_path_with_frequencies(const items_t &items, const std::vector<size_t> &frequencies) const;
    };

    /// Creates the power set of the given set of items.
    /// @param items The input itemset for which the power set is to created.
    /// @return The set of all subsets of the given items.
    auto power_set(const itemset_t &items, bool include_empty_set = true) -> itemsets_t;

    /// Insert a specified item into each subset of the given itemsets.
    /// @param itemsets The set of itemsets to be expanded.
    /// @param item The item to be added to each subset within the itemsets.
    /// @return A new set of itemsets where each original subset has been expanded with the given item.
    auto insert_into_all_itemsets(const itemsets_t &itemsets, item_t item) -> itemsets_t;

    /// Gets a sorted list of frequent items from the given transactions.
    /// @param transactions A collection of transactions where each transaction is a set of items.
    /// @param min_support_abs The minimum support threshold for items to be considered frequent.
    /// @return A list of items that exceed the minimum support threshold, sorted in order of frequency.
    auto find_frequent_items(
            const transactions_t &transactions,
            size_t min_support_abs) -> std::pair<items_t, frequencies_t>;

    /// Sorts the items in the x according to the order defined by the frequent items list
    /// and removes any items not in the frequent items list.
    /// @param itemset The original set of items to be filtered.
    /// @param frequent_items A list of items considered frequent.
    /// @return A list of items from the x that are also in the frequent items list, maintaining their order.
    auto filter_and_sort_items(const itemset_t &itemset, const items_t &frequent_items) -> items_t;

    /// Builds a FP-tree from the given transactions based on frequent items and minimum support.
    /// @param transactions A collection of transactions, where each transaction is a set of items.
    /// @param frequent_items A list of items considered frequent, based on the minimum support.
    /// @return A shared pointer to the root node of the constructed FP-tree.
    auto build_fp_tree(const transactions_t &transactions, const items_t &frequent_items) -> node_ptr;

    /// Builds a FP-tree from the given transactions based on the minimum support.
    /// @param itemsets A collection of transactions, where each transaction is a set of items.
    /// @param min_support_abs The minimum support threshold for an item to be considered frequent.
    /// @return A shared pointer to the root node of the constructed FP-tree.
    auto build_fp_tree(const transactions_t &itemsets, size_t min_support_abs) -> node_ptr;

    /// Checks whether a given FP-tree root has a single path from the root to a leaf.
    /// @param root A pointer to the current root in the FP-tree.
    /// @return A pair where the first element is a boolean indicating whether the tree has a single path,
    /// and the second element is an itemset representing the sequence of items along that path.
    auto tree_has_single_path(const node_ptr &root) -> std::optional<itemset_t>;

    /// Gets the frequency of the given item.
    /// @param root The
    /// @param item
    /// @return
    size_t get_item_frequency(const node_ptr &root, item_t item);

    /// Generates the conditional transaction from the given FP-Tree.
    /// @param node The current node in the FP-Tree being processed.
    /// @param item  The item for which conditional transactions are being generated.
    /// @return The transactions that will be populated with the conditional transactions.
    auto conditional_transactions(const node_ptr &node, item_t item) -> transactions_t;

    /// Applies the FP-Growth algorithm to find frequent itemsets from the given transactions.
    /// @param transactions A collection of transactions, where each transaction is a set of items.
    /// @param min_support_abs The minimum support threshold for an itemset to be considered frequent.
    /// @return A set of frequent itemsets, where each itemset is a collection of items that meet
    /// the minimum support threshold.
    auto fp_growth_algorithm(const transactions_t &transactions, size_t min_support_abs) -> itemsets_t;
}