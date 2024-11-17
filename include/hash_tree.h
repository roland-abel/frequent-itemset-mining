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
#include <stack>
#include <memory>
#include <iterator>
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


    class hash_tree_itemset_iterator {
    public:
        using value_type = itemset_t; // Typ der einzelnen Itemsets
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type *;
        using reference = const value_type &;
        using iterator_category = std::forward_iterator_tag;

        // Constructor: Start from the root
        explicit hash_tree_itemset_iterator(std::shared_ptr<hash_tree_node> root) {
            if (root) {
                stack_.push(root);
                advance_to_next_itemset();
            }
        }

        // End iterator constructor
        hash_tree_itemset_iterator() = default;

        // Dereference to get the current itemset
        auto operator*() const -> reference {
            return *current_itemset_;
        }

        // Move to the next itemset
        auto operator++() -> hash_tree_itemset_iterator & {
            ++current_itemset_index_;
            advance_to_next_itemset();
            return *this;
        }

        // Postfix increment
        auto operator++(int) -> hash_tree_itemset_iterator {
            auto temp = *this;
            ++(*this);
            return temp;
        }

        // Comparison operators
        auto operator==(const hash_tree_itemset_iterator &other) const -> bool {
            return current_itemset_ == other.current_itemset_ &&
                   current_itemset_index_ == other.current_itemset_index_ &&
                   stack_ == other.stack_;
        }

        auto operator!=(const hash_tree_itemset_iterator &other) const -> bool {
            return !(*this == other);
        }

    private:
        std::stack<std::shared_ptr<hash_tree_node>> stack_;
        std::shared_ptr<leaf_node> current_leaf_ = nullptr;
        const itemsets_t *current_itemsets_ = nullptr;
        size_t current_itemset_index_ = 0;
        pointer current_itemset_ = nullptr;

        // Advance to the next itemset
        void advance_to_next_itemset() {
            while (true) {
                // Check if we are inside a valid leaf's itemsets
                if (current_itemsets_ && current_itemset_index_ < current_itemsets_->size()) {
                    current_itemset_ = &(*current_itemsets_)[current_itemset_index_];
                    return;
                }

                // Move to the next leaf node
                current_leaf_ = nullptr;
                current_itemsets_ = nullptr;
                current_itemset_index_ = 0;

                // If the stack is empty, we're done
                if (stack_.empty()) {
                    current_itemset_ = nullptr;
                    return;
                }

                auto node = stack_.top();
                stack_.pop();

                if (node->is_leaf()) {
                    current_leaf_ = std::static_pointer_cast<leaf_node>(node);
                    current_itemsets_ = &current_leaf_->itemsets();
                    current_itemset_index_ = 0;
                } else {
                    auto inner = std::dynamic_pointer_cast<inner_node>(node);
                    if (inner) {
                        for (const auto &[_, child] : inner->children()) {
                            stack_.push(child);
                        }
                    }
                }
            }
        }
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

        auto begin() -> hash_tree_itemset_iterator {
            return hash_tree_itemset_iterator(root_);
        }

        auto end() -> hash_tree_itemset_iterator {
            return hash_tree_itemset_iterator();
        }

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
