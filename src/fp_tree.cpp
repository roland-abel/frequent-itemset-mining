/// @file fp_tree.cpp
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

#include <execution>
#include <ranges>
#include <functional>
#include "fp_tree.h"

namespace rules::fp_tree {

    using std::views::filter;
    using std::views::transform;

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

    auto get_item_frequency(const node_ptr &root, item_t item) -> size_t {
        size_t frequency = 0;

        std::function<void(const node_ptr &)> count_frequency = [&](const node_ptr &current) -> void {
            if (current->item == item) {
                frequency += current->frequency;
            }

            std::ranges::for_each(current->children, count_frequency);
        };

        count_frequency(root);
        return frequency;
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

    auto get_item_counts(const database_t &database) -> item_counts_t {
        auto item_counts = item_counts_t{};
        for (const auto &item: database | std::views::join) {
            ++item_counts[item];
        }
        return item_counts;
    }

    auto get_frequent_items(const item_counts_t &item_counts, size_t min_support) -> std::pair<items_t, item_counts_t> {
        auto is_frequent = [=](const auto &kv) { return kv.second >= min_support; };

        auto item_compare = [&](const item_t &x, const item_t &y) {
            return item_counts.at(x) > item_counts.at(y);
        };

        auto items = item_counts
                     | filter(is_frequent)
                     | std::views::keys
                     | std::ranges::to<std::vector>();

        std::ranges::sort(items, item_compare);
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
}