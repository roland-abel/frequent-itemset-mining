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
#include "fp_growth.h"

namespace rules::fp_growth {

    using std::views::filter;
    using std::views::transform;

    auto power_set(const itemset_t &items, bool include_empty_set) -> itemsets_t {
        itemsets_t result{};
        const auto num_subsets = static_cast<size_t>(std::pow(2, items.size()));

        for (size_t i = 0; i < num_subsets; ++i) {
            itemset_t subset;
            size_t bit_position = 0;

            for (const auto &item: items) {
                if (i & (1 << bit_position)) {
                    subset.insert(item);
                }
                ++bit_position;
            }

            if (include_empty_set || !subset.empty()) {
                result.insert(std::move(subset));
            }
        }
        return result;
    }

    auto insert_into_each_itemsets(const itemsets_t &itemsets, item_t item) -> itemsets_t {
        const auto copy_and_insert = [item](const auto &subset) -> itemset_t {
            auto result = subset; // call copy-constructor
            result.insert(item);
            return result;
        };

        return itemsets
               | std::views::transform(copy_and_insert)
               | std::ranges::to<itemsets_t>();
    }

    auto item_compare(const item_counts_t &item_counts) -> std::function<bool(const item_t &x, const item_t &y)> {
        return [&](const item_t &x, const item_t &y) {
            return item_counts.at(x) > item_counts.at(y);
        };
    }

    auto filter_items(const item_counts_t &item_counts, size_t min_support) -> itemset_filter_fn {
        auto is_frequent = [=](const item_t item) { return item_counts.at(item) >= min_support; };
        return [&](const itemset_t &items) {
            return items | filter(is_frequent) | std::ranges::to<items_t>();
        };
    }

    auto get_item_counts(const database_t &database) -> item_counts_t {
        auto item_counts = item_counts_t{};
        for (const auto &item: database | std::views::join) {
            ++item_counts[item];
        }
        return item_counts;
    }

    auto get_frequent_items(const item_counts_t &item_counts, size_t min_support) -> std::pair<items_t, frequencies_t> {
        auto is_frequent = [=](const auto &kv) { return kv.second >= min_support; };
        auto items = item_counts
                     | filter(is_frequent)
                     | std::views::keys
                     | std::ranges::to<std::vector>();

        std::ranges::sort(items, item_compare(item_counts));
        return {items, item_counts};
    }

    auto filter_and_sort_items(const itemset_t &itemset, const items_t &frequent_items) -> items_t {
        auto is_frequent = [&](const auto &item) { return itemset.contains(item); };

        auto items = frequent_items
                     | std::views::filter(is_frequent)
                     | std::ranges::to<std::vector>();

        std::sort(std::execution::par_unseq, items.begin(), items.end(), [&](const item_t &x, const item_t &y) {
            return std::ranges::find(frequent_items, x) < std::ranges::find(frequent_items, y);
        });

        return items;
    }

    auto filter_and_sort_items_(const item_counts_t &item_counts, size_t min_support) -> filter_and_sort_fn {
        return [&](const itemset_t &itemset) {
            auto items = filter_items(item_counts, min_support)(itemset);
            std::ranges::sort(items, item_compare(item_counts));
            return items;
        };
    }

    auto build_fp_tree(const database_t &transactions, const items_t &frequent_items) -> node_ptr {
        auto root = node_t::create_root();
        auto insert_items = [&](const items_t &items) {
            auto current = root;
            for (auto it = items.begin(); it != items.end(); it++) {
                const auto item = *it;
                auto node = current->find_child_item(item)
                        .or_else([&]() -> std::optional<node_ptr> { return current->add_child(item, 0); })
                        .value();

                node->frequency++;
                current = node;
            }
        };

        for (const auto &trans: transactions) {
            const auto &items = filter_and_sort_items(trans, frequent_items);
            insert_items(items);
        }
        return root;
    }

    auto build_fp_tree(const database_t &database, size_t min_support) -> node_ptr {
        const auto &item_counts = get_item_counts(database);
        const auto &[frequent_items, _] = get_frequent_items(item_counts, min_support);

        return build_fp_tree(database, frequent_items);
    }

    auto conditional_transactions(const node_ptr &root, item_t item) -> database_t {
        database_t transactions{};

        std::function<void(const node_ptr &)> conditional_transactions_ = [&](const node_ptr &node) -> void {
            // traverses from a given node to the root and collects the items along the path with regard to the frequency
            auto collect_path = [&](const node_ptr &node) -> itemset_t {
                itemset_t path{};

                node_ptr current = node->parent.lock();
                while (!current->is_root()) {
                    path.insert(current->item);
                    current = current->parent.lock();
                }
                return path;
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
        itemsets_t frequent_itemsets{};

        const auto update_frequent_itemsets = [&](const item_t &item, const itemsets_t &itemsets) {
            frequent_itemsets.insert(itemset_t{item});
            std::ranges::merge(frequent_itemsets, itemsets, std::inserter(frequent_itemsets, frequent_itemsets.end()));
        };

        const auto &item_counts = get_item_counts(database);
        const auto &[frequent_items, _] = get_frequent_items(item_counts, min_support);
        const auto &root = build_fp_tree(database, frequent_items);
        const auto &items_along_path = tree_has_single_path(root);

        if (items_along_path.has_value()) {
            return power_set(items_along_path.value(), false);
        } else {
            // traverses all frequent items in the reversed order
            for (auto &item: std::ranges::reverse_view(frequent_items)) {
                const auto &cond_trans = conditional_transactions(root, item);
                const auto &cond_itemsets = fp_growth_algorithm(cond_trans, min_support);
                const auto &itemsets = insert_into_each_itemsets(cond_itemsets, item);

                update_frequent_itemsets(item, itemsets);
            }
        }
        return frequent_itemsets;
    }
}