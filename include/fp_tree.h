/// @file fp_tree.h
/// @brief Implementation of an FP-Tree data structure used for the FP-Growth algorithm.
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

#pragma once

#include "itemset.h"
#include "database.h"

namespace fim::fp_tree {
    struct node_t;

    using namespace fim;
    using items_t = std::vector<item_t>;

    using node_ptr = std::shared_ptr<node_t>;
    using children_t = std::vector<node_ptr>;

    /// @brief Represents a node in an FP-tree.
    struct node_t : std::enable_shared_from_this<node_t> {
        using parent_ptr = std::weak_ptr<node_t>;

        item_t item{}; ///< The item stored in this node.
        size_t frequency{}; ///< The frequency of the item in the database.
        parent_ptr parent; ///< A weak pointer to the parent node.
        children_t children{}; ///< A list of child nodes.

        /// @brief Constructs a node with the given item, frequency, and optional parent.
        /// @param item The item to be stored in the node.
        /// @param frequency The frequency of the item.
        /// @param parent An optional shared pointer to the parent node.
        node_t(item_t item, size_t frequency, const std::shared_ptr<node_t> &parent = nullptr);

        /// @brief Default constructor for node_t.
        node_t() = default;

        /// @brief Checks if this node is the root node of the FP-tree.
        /// @return True if this node is the root, otherwise false.
        bool is_root() const;

        /// @brief Creates and returns the root node for an FP-tree.
        /// @return A shared pointer to the newly created root node.
        static node_ptr create_root();

        /// @brief Adds a child node with the specified item and frequency to this node.
        /// @param child_item The item for the new child node.
        /// @param child_frequency The frequency of the child item.
        /// @return A shared pointer to the newly added child node.
        node_ptr add_child(const item_t &child_item, size_t child_frequency);

        /// @brief Finds the child node corresponding to the specified item.
        /// @param child_item The item to find in the list of children.
        /// @return An optional shared pointer to the child node if found; otherwise, std::nullopt.
        std::optional<node_ptr> find_child_item(const item_t &child_item) const;

        /// @brief Checks if a path matching the given items and their frequencies exist in the tree.
        /// @param items A vector of items to be matched in the path.
        /// @param frequencies A vector of frequencies corresponding to the items.
        /// @return True if a path matching all items and frequencies is found, otherwise false.
        bool has_path_with_frequencies(const items_t &items, const std::vector<size_t> &frequencies) const;
    };

    /// @brief Gets the frequency of the given item in the FP-tree.
    /// @param root The root node of the FP-tree.
    /// @param item The item whose frequency is to be retrieved.
    /// @return The frequency of the specified item.
    auto get_item_frequency(const node_ptr &root, item_t item) -> size_t;

    /// @brief Checks if a given FP-tree is a single path from the root to a leaf node.
    /// @param root A pointer to the root node of the FP-tree.
    /// @return A boolean value indicating whether the tree is a single path.
    /// If true, the return value will contain an itemset representing the sequence of items along that path.
    auto tree_is_single_path(const node_ptr &root) -> std::optional<itemset_t>;

    /// @brief Creates the power set of the given set of items (all subsets of the items).
    /// @param items The input itemset for which the power set is to be created.
    /// @param include_empty_set If true, the empty set will be included in the power set.
    /// @return The power set of the given items as a collection of itemsets.
    auto power_set(const itemset_t &items, bool include_empty_set = true) -> itemsets_t;

    /// @brief Inserts a specified item into each subset of the given itemsets.
    /// @param itemsets A collection of itemsets.
    /// @param item The item to be inserted into each subset.
    /// @return A new collection of itemsets where the specified item has been added to each original subset.
    auto insert_into_each_itemsets(const itemsets_t &itemsets, item_t item) -> itemsets_t;

    /// @brief Filters and sorts the items in the input itemset according to the order of frequent items.
    /// @param itemset The original itemset to be filtered and sorted.
    /// @param freq_items A list of frequent items, used to determine the order.
    /// @return Sorted list of items, sorted by their frequency.
    auto filter_and_sort_items(const itemset_t &itemset, const items_t &freq_items) -> items_t;

    /// @brief Builds an FP-tree from the given transaction database using the frequent items list.
    /// @param database The transaction database containing the items and their frequencies.
    /// @param freq_items The list of frequent items used to build the FP-tree.
    /// @return A shared pointer to the root node of the newly constructed FP-tree.
    auto build_fp_tree(const database_t &database, const items_t &freq_items) -> node_ptr;
}
