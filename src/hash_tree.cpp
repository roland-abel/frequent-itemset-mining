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

#include "hash_tree.h"

using namespace fim;

namespace fim::hash {

    inner_node::inner_node(hash_function_t hash_func)
            : hash_func_(std::move(hash_func)) {
    }

    auto inner_node::hash_code(const item_t &item) const -> size_t {
        return hash_func_(item);
    }

    auto inner_node::is_leaf() const -> bool {
        return false;
    }

    auto inner_node::children() -> inner_node::nodes_t & {
        return children_;
    }

    bool leaf_node::is_leaf() const {
        return true;
    }

    const itemsets_t &leaf_node::itemsets() const {
        return itemsets_;
    }

    itemsets_t &leaf_node::itemsets() {
        return itemsets_;
    }

    hash_tree::hash_tree(size_t max_leaf_size, const hash_function_t &hash_func)
            : max_leaf_size_(max_leaf_size),
              hash_func_(hash_func),
              root_(std::make_shared<inner_node>(hash_func)) {
    }

    auto hash_tree::insert(const itemset_t &itemset) -> void {
        using func_t = std::function<void(std::shared_ptr<hash_tree_node>, size_t)>;
        func_t insert_ = [&](const auto &node, size_t depth) {
            if (node->is_leaf()) {
                auto leaf = std::static_pointer_cast<leaf_node>(node);
                leaf->itemsets().emplace_back(itemset);

                if (leaf->itemsets().size() > max_leaf_size_) {
                    split_leaf_node(leaf, depth);
                }

            } else {
                auto inner = std::static_pointer_cast<inner_node>(node);
                const auto hash_value = inner->hash_code(itemset[depth]);

                const auto [it, _] = inner->children()
                        .try_emplace(hash_value, std::make_shared<leaf_node>());

                insert_(it->second, depth + 1);
            }
        };
        insert_(root_, 0);
    }

    auto hash_tree::search(const itemset_t &itemset) const -> std::optional<itemset_t> {
        using func_t = std::function<std::optional<itemset_t>(std::shared_ptr<hash_tree_node>, size_t)>;
        func_t search_ = [&](const auto &node, size_t depth) -> std::optional<itemset_t> {
            if (node->is_leaf()) {
                auto leaf = std::static_pointer_cast<leaf_node>(node);
                for (const auto &candidate: leaf->itemsets()) {
                    if (candidate == itemset) {
                        return candidate;
                    }
                }
            } else {
                auto inner = std::static_pointer_cast<inner_node>(node);
                const auto hash_value = inner->hash_code(itemset[depth]);

                if (inner->children().contains(hash_value)) {
                    return search_(inner->children().at(hash_value), depth + 1);
                }
            }
            return std::nullopt;
        };
        return search_(root_, 0);
    }

    auto hash_tree::split_leaf_node(const std::shared_ptr<leaf_node> &leaf, size_t depth) -> void {
        auto new_inner_node = std::make_shared<inner_node>(hash_func_);

        for (const auto &itemset: leaf->itemsets()) {
            const auto hash_value = hash_func_(itemset[depth]);

            const auto [it, _] = new_inner_node->children()
                    .try_emplace(hash_value, std::make_shared<leaf_node>());

            auto child_leaf = std::static_pointer_cast<leaf_node>(it->second);
            child_leaf->itemsets().emplace_back(itemset);
        }
        *leaf = *std::dynamic_pointer_cast<leaf_node>(new_inner_node);
    }
}