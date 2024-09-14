/// @file fp_tree.h
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
#include <optional>
#include <functional>
#include "dtypes.h"

namespace rules::fp_tree {
    struct node_t;

    using item_t = rules::item_t;

    using items_t = std::vector<item_t>;
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

        /// Finds the child node with is associated with the the specified item.
        /// @param child_item The item to find in the children.
        /// @return An optional shared pointer to the child node if found; otherwise, std::nullopt
        std::optional<node_ptr> find_child_item(const item_t &child_item) const;

        /// Checks if there exists a path in the tree matching the given items and frequencies.
        /// @param items A vector of items to be matched in the tree path.
        /// @param frequencies A vector of frequencies corresponding to the items.
        /// @return True if a path matching all items and frequencies is found; otherwise, false.
        bool has_path_with_frequencies(const items_t &items, const std::vector<size_t> &frequencies) const;
    };

    /// Gets the frequency of the given item.
    /// @param root The
    /// @param item
    /// @return
    auto get_item_frequency(const node_ptr &root, item_t item) -> size_t;

    /// Checks whether a given FP-tree root has a single path from the root to a leaf.
    /// @param root A pointer to the current root in the FP-tree.
    /// @return A pair where the first element is a boolean indicating whether the tree has a single path,
    /// and the second element is an itemset representing the sequence of items along that path.
    auto tree_has_single_path(const node_ptr &root) -> std::optional<itemset_t>;
}