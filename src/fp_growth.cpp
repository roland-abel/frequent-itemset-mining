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
#include "fp_growth.h"

namespace rules::fp_growth {

    node_t::node_t(item_t item, size_t frequency, const std::shared_ptr<node_t> &parent)
            : item(item), frequency(frequency), parent(parent), children({}) {
    }

    bool node_t::is_root() const {
        return item == 0 && frequency == 0;
    }

    node_ptr node_t::create_root() {
        return std::make_shared<node_t>(node_t(0, 0, nullptr));
    }

    node_ptr node_t::add_child(const item_t &child_item, size_t child_frequency) {
        auto child = std::make_shared<node_t>(child_item, child_frequency, shared_from_this());
        children.emplace_back(child);
        return child;
    }

    std::optional<node_ptr> node_t::find_child_item(const item_t &child_item) const {
        const auto it = std::find_if(children.begin(), children.end(), [&](const node_ptr &node) {
            return node->item == child_item;
        });
        return it != children.end() ? std::optional{*it} : std::nullopt;
    }

    bool node_t::has_path_with_frequencies(const items_t &items, const std::vector<size_t> &frequencies) const {
        if (items.empty() || frequencies.empty() || items.size() != frequencies.size()) {
            return false;
        }

        node_ptr current = std::make_shared<node_t>(*this);
        size_t depth = 0;

        while (current) {
            const auto next = current->find_child_item(items[depth]);
            if (!next.has_value()) {
                return false;
            }
            current = next.value();

            if (current->item != items[depth] || current->frequency != frequencies[depth]) {
                return false;
            }

            depth++;

            if (depth >= items.size()) {
                break;
            }
        }
        return depth == items.size();
    }

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

    auto insert_into_all_itemsets(const itemsets_t &itemsets, item_t item) -> itemsets_t {
        const auto copy_and_insert = [item](const auto &subset) -> itemset_t {
            auto result = subset; // call copy-constructor
            result.insert(item);
            return result;
        };

        return itemsets
               | std::views::transform(copy_and_insert)
               | std::ranges::to<itemsets_t>();
    }

    auto find_frequent_items(const transactions_t &transactions, size_t min_support_abs) -> std::pair<items_t, frequencies_t> {
        auto frequencies = frequencies_t{};

        for (const auto &item: transactions | std::views::join) {
            ++frequencies[item];
        }

        auto items = frequencies
                     | std::views::filter([=](const auto &kv) { return kv.second >= min_support_abs; })
                     | std::views::transform([](const auto &kv) { return kv.first; })
                     | std::ranges::to<std::vector>();

        std::ranges::sort(items, [&](const item_t x, const item_t y) {
            return frequencies[x] > frequencies[y];
        });
        return {items, frequencies};
    }

    auto filter_and_sort_items(const itemset_t &itemset, const items_t &frequent_items) -> items_t {
        auto items = frequent_items
                     | std::views::filter([&](const auto &item) { return itemset.contains(item); })
                     | std::ranges::to<std::vector>();

        std::ranges::sort(items, [&](const item_t &x, const item_t &y) {
            return std::ranges::find(frequent_items, x) < std::ranges::find(frequent_items, y);
        });

        return items;
    }

    auto insert_items(const node_ptr &root, const items_t &items) {
        auto current = root;
        for (auto it = items.begin(); it != items.end(); it++) {
            const auto item = *it;
            auto node = current->find_child_item(item)
                    .or_else([&]() ->  std::optional<node_ptr> { return current->add_child(item, 0); })
                    .value();

            node->frequency++;
            current = node;
        }
    }

    auto build_fp_tree(const transactions_t &transactions, const items_t &frequent_items) -> node_ptr {
        auto root = node_t::create_root();

        for (const auto &trans: transactions) {
            const auto &items = filter_and_sort_items(trans, frequent_items);
            insert_items(root, items);
        }
        return root;
    }

    auto build_fp_tree(const transactions_t &itemsets, size_t min_support_abs) -> node_ptr {
        const auto &[frequent_items, _] = find_frequent_items(itemsets, min_support_abs);
        return build_fp_tree(itemsets, frequent_items);
    }

    size_t get_item_frequency(const node_ptr &root, item_t item) {
        size_t frequency = 0;

        std::function<void(const node_ptr &)> count_frequency = [&](const node_ptr &current) -> void {
            if (current->item == item) {
                frequency += current->frequency;
            }

            std::ranges::for_each(current->children, [&](const auto &child) {
                count_frequency(child);
            });
        };

        count_frequency(root);
        return frequency;
    }

    auto conditional_transactions(const node_ptr &root, item_t item) -> transactions_t {
        transactions_t transactions{};

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

    auto tree_has_single_path(const node_ptr &root) -> std::optional<itemset_t> {
        itemset_t items_along_path{};

        if (root->children.size() != 1) {
            return std::nullopt;
        }

        node_ptr current = root->children.at(0);
        while (true) {
            const auto num_children = current->children.size();
            if (num_children > 1) {
                return std::nullopt;
            }

            items_along_path.insert(current->item);
            if (num_children == 0) {
                return items_along_path;
            }

            current = current->children.at(0);
        }
    }

    auto fp_growth_algorithm(const transactions_t &transactions, size_t min_support_abs) -> itemsets_t {
        itemsets_t frequent_itemsets{};

        const auto update_frequent_itemsets = [&](const item_t &item, const itemsets_t &itemsets) {
            frequent_itemsets.insert(itemset_t{item});
            std::ranges::merge(frequent_itemsets, itemsets, std::inserter(frequent_itemsets, frequent_itemsets.end()));
        };

        const auto &[frequent_items, _] = find_frequent_items(transactions, min_support_abs);
        const auto &root = build_fp_tree(transactions, frequent_items);
        const auto &items_along_path = tree_has_single_path(root);

        if (items_along_path.has_value()) {
            return power_set(items_along_path.value(), false);
        } else {
            // traverses all frequent items in the reversed order
            for (auto it = frequent_items.rbegin(); it != frequent_items.rend(); ++it) {
                const auto &item = *it;

                const auto &cond_trans = conditional_transactions(root, item);
                const auto &cond_itemsets = fp_growth_algorithm(cond_trans, min_support_abs);
                const auto &itemsets = insert_into_all_itemsets(cond_itemsets, item);

                update_frequent_itemsets(item, itemsets);
            }
        }
        return frequent_itemsets;
    }
}