/// @file hash_tree.h
/// @brief
/// @see: https://www.macs.hw.ac.uk/~dwcorne/Teaching/agrawal94fast.pdf
///
/// @author Roland Abel
/// @date September 16, 2024
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

#include <functional>
#include <utility>
#include "itemset.h"

namespace fim::hash {

    using hash_function_t = std::function<size_t(const item_t &)>;

    /// Hash tree node type.
    class hash_tree_node {
    public:
        virtual ~hash_tree_node() = default;

        [[nodiscard]] virtual bool is_leaf() const = 0;
    };

    /// Inner node type
    class inner_node : public hash_tree_node {
    public:
        using nodes_t = std::unordered_map<size_t, std::shared_ptr<hash_tree_node>>;

        ///
        /// @param hash_func
        explicit inner_node(hash_function_t hash_func);

        ///
        /// @param item
        /// @return
        [[nodiscard]] auto hash_code(const item_t &item) const -> size_t;

        ///
        /// @return
        [[nodiscard]] auto is_leaf() const -> bool override;

        ///
        /// @return
        auto children() -> nodes_t &;

    private:
        std::unordered_map<size_t, std::shared_ptr<hash_tree_node>> children_{};
        hash_function_t hash_func_;
    };

    /// Leaf node type.
    class leaf_node : public hash_tree_node {
    public:
        ///
        /// @return
        [[nodiscard]] bool is_leaf() const override;

        ///
        /// @return
        [[nodiscard]] const itemsets_t &itemsets() const;

        ///
        /// @return
        itemsets_t &itemsets();

    private:
        itemsets_t itemsets_{};
    };

    /// Hash tree type.
    class hash_tree {
    public:
        /// Constructor.
        /// @param max_leaf_size
        /// @param hash_func
        hash_tree(size_t max_leaf_size, const hash_function_t &hash_func);

        ///
        /// @param itemset
        auto insert(const itemset_t &itemset) -> void;

        ///
        /// @param itemset
        /// @return
        [[nodiscard]] auto search(const itemset_t &itemset) const -> std::optional<itemset_t>;

    private:
        ///
        /// @param leaf
        /// @param depth
        auto split_leaf_node(const std::shared_ptr<leaf_node> &leaf, size_t depth) -> void;

    private:
        size_t max_leaf_size_{};
        hash_function_t hash_func_;
        std::shared_ptr<hash_tree_node> root_;
    };
}
