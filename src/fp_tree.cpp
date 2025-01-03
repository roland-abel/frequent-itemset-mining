/// @file fp_tree.cpp
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

#include <execution>
#include <ranges>
#include <functional>
#include "fp_tree.h"

namespace fim::fp_tree {
    using std::views::filter;
    using std::views::transform;
    using std::ranges::find;
    using std::ranges::to;

    node_t::node_t(const item_t item, const size_t frequency, const std::shared_ptr<node_t> &parent)
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
        const auto it = std::ranges::find_if(children, [&](const node_ptr &node) {
            return node->item == child_item;
        });
        return it != children.end() ? std::optional{*it} : std::nullopt;
    }

    bool node_t::has_path_with_frequencies(const items_t &items, const std::vector<size_t> &frequencies) const {
        if (items.empty() || frequencies.empty() || items.size() != frequencies.size()) {
            return false;
        }

        auto current = std::make_shared<node_t>(*this);
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

    auto get_item_frequency(const node_ptr &root, const item_t item) -> size_t {
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

    auto tree_is_single_path(const node_ptr &root) -> std::optional<itemset_t> {
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

            items_along_path.emplace_back(current->item);
            if (num_children == 0) {
                return items_along_path;
            }

            current = current->children.at(0);
        }
    }

    auto power_set(const itemset_t &items, const bool include_empty_set) -> itemsets_t {
        itemsets_t result{};
        const auto num_subsets = static_cast<size_t>(std::pow(2, items.size()));

        for (size_t i = 0; i < num_subsets; ++i) {
            itemset_t subset{};
            size_t bit_position = 0;

            for (const auto &item: items) {
                if (i & 1 << bit_position) {
                    subset.emplace_back(item);
                }
                ++bit_position;
            }

            if (include_empty_set || !subset.empty()) {
                result.emplace_back(std::move(subset));
            }
        }
        return result;
    }

    auto insert_into_each_itemsets(const itemsets_t &itemsets, item_t item) -> itemsets_t {
        const auto copy_and_insert = [item](const itemset_t &subset) -> itemset_t {
            itemset_t result = subset; // call copy-constructor
            result.add(item);
            return result;
        };

        return itemsets
               | transform(copy_and_insert)
               | to<itemsets_t>();
    }

    auto filter_and_sort_items(const itemset_t &itemset, const items_t &freq_items) -> items_t {
        auto is_frequent = [&](const auto &item) { return find(itemset, item) != itemset.end(); };

        auto items = freq_items
                     | filter(is_frequent)
                     | to<std::vector>();

        std::ranges::sort(items, [&](const item_t &x, const item_t &y) {
            return find(freq_items, x) < find(freq_items, y);
        });

        return items;
    }

    auto build_fp_tree(const database_t &database, const items_t &freq_items) -> node_ptr {
        auto root = node_t::create_root();

        auto insert_items = [&](const items_t &items) {
            auto current = root;
            for (auto it = items.begin(); it != items.end(); ++it) {
                const auto item = *it;
                const auto node = current->find_child_item(item)
                        .or_else([&]() -> std::optional<node_ptr> { return current->add_child(item, 0); })
                        .value();

                node->frequency++;
                current = node;
            }
        };

        for (const auto &trans: database) {
            const auto &itemset = filter_and_sort_items(trans, freq_items);
            insert_items(itemset);
        }

        return root;
    }
}
