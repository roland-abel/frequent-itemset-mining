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

#include <functional>
#include <algorithm>
#include <ranges>
#include "fp_tree.h"
#include "fp_growth.h"

namespace fim::algorithm::fp_growth {
    using namespace fim::fp_tree;
    using std::views::filter;
    using std::views::transform;

    auto conditional_transactions(const node_ptr &root, item_t item) -> database_t {
        database_t transactions{};

        std::function<void(const node_ptr &)> conditional_transactions_ = [&](const node_ptr &node) -> void {
            // traverses from a given node to the root and collects the items along the path with regard to the frequency
            auto collect_path = [&](const node_ptr &n) -> itemset_t {
                itemset_t path{};

                node_ptr current = n->parent.lock();
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

    auto fp_growth_algorithm(const database_t &database, const size_t min_support) -> itemsets_t {
        itemsets_t freq_itemsets{};

        const auto [db, item_counts] = database.transaction_reduction(min_support);
        const auto compare = item_counts.get_item_compare();

        const auto update_frequent_itemsets = [&](const item_t &item, const itemsets_t &itemsets) {
            freq_itemsets.add(itemset_t{item});
            freq_itemsets.add(itemsets);
        };

        const auto &freq_items = item_counts.get_frequent_items(min_support);
        const auto &root = build_fp_tree(db, freq_items);
        const auto &items_along_path = tree_is_single_path(root);

        if (items_along_path.has_value()) {
            return power_set(items_along_path.value(), false);
        }

        // traverses all frequent items in the reversed order
        for (auto &item: std::ranges::reverse_view(freq_items)) {
            const auto &cond_trans = conditional_transactions(root, item);
            const auto &cond_itemsets = fp_growth_algorithm(cond_trans, min_support);
            const auto &itemsets = insert_into_each_itemsets(cond_itemsets, item);

            update_frequent_itemsets(item, itemsets);
        }
        return freq_itemsets;
    }
}
