/// @file fp_growth.cpp
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

#include <functional>
#include <algorithm>
#include <execution>
#include <ranges>
#include "fp_tree.h"
#include "fp_growth.h"

namespace fim::fp_growth {

    using namespace fim::fp_tree;
    using std::views::filter;
    using std::views::transform;

    auto conditional_transactions(const node_ptr &root, item_t item) -> database_t {
        database_t transactions{};

        std::function<void(const node_ptr &)> conditional_transactions_ = [&](const node_ptr &node) -> void {
            // traverses from a given node to the root and collects the items along the path with regard to the frequency
            auto collect_path = [&](const node_ptr &node) -> itemset_t {
                itemset_t path{};

                node_ptr current = node->parent.lock();
                while (!current->is_root()) {
                    path.add(current->item);
                    current = current->parent.lock();
                }
                return path.sort_itemset();
            };

            if (node->item == item) {
                itemset_t items = collect_path(node);
                for (int i = 0; i < node->frequency; ++i) {
                    transactions.emplace_back(items);
                }
            } else {
                for (const auto &child: node->children) {
                    conditional_transactions_(child);
                }
            }
        };

        conditional_transactions_(root);
        return transactions;
    }

    auto fp_growth_algorithm(const database_t &database, size_t min_support) -> itemsets_t {
        itemsets_t freq_items{};

        const auto update_frequent_itemsets = [&](const item_t &item, const itemsets_t &itemsets) {
            freq_items.add(itemset_t{item});
            freq_items.add(itemsets);
        };

        const auto &count = get_item_count(database);
        const auto &[items, _] = get_ordered_frequent_items(count, min_support);
        const auto &root = build_fp_tree(database, items);
        const auto &items_along_path = tree_has_single_path(root);

        if (items_along_path.has_value()) {
            return power_set(items_along_path.value(), false);
        } else {
            // traverses all frequent items in the reversed order
            for (auto &item: std::ranges::reverse_view(items)) {
                const auto &cond_trans = conditional_transactions(root, item);
                const auto &cond_itemsets = fp_growth_algorithm(cond_trans, min_support);
                const auto &itemsets = insert_into_each_itemsets(cond_itemsets, item);

                update_frequent_itemsets(item, itemsets);
            }
        }
        return freq_items;
    }
}