/// @file fp_growth.h
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

#include <algorithm>
#include <utility>
#include <ranges>
#include "apriori.h"

namespace rules::fp_growth {

    struct fp_node_t;

    using item_t = rules::apriori::item_t;
    using items_t = std::vector<item_t>;
    using frequencies_t = std::unordered_map<item_t, size_t>;

    using transactions_t = rules::apriori::transactions_t;
    using fp_node_ptr = std::shared_ptr<fp_node_t>;
    using children_t = std::vector<fp_node_ptr>;

    struct fp_node_t {
        item_t item{};
        size_t frequency{};
        fp_node_ptr parent;
        children_t children{};
    };

    auto make_root() -> fp_node_ptr {
        return std::make_shared<fp_node_t>(fp_node_t{item_t{}, 0, nullptr, {}});
    };

    auto make_node(const item_t item, size_t frequency, fp_node_ptr &parent) -> fp_node_ptr {
        auto node = std::make_shared<fp_node_t>(fp_node_t{item, frequency, parent, children_t{}});
        parent->children.push_back(node);

        return node;
    };

    auto find_node(const children_t &children, item_t item) -> decltype(children.begin()) {
        return std::find_if(children.begin(), children.end(), [item](const fp_node_ptr &node) {
            return node->item == item;
        });
    }

    auto create_frequent_items(const transactions_t &itemsets, float min_support_abs) -> items_t {
        auto frequencies = frequencies_t{};

        for (const auto &t: itemsets) {
            for (const item_t item: t) {
                if (t.contains(item)) {
                    frequencies[item]++;
                }
            }
        }

        auto is_frequent_item = [=](const auto &kv) { return kv.second >= min_support_abs; };
        auto item_selector = [](const auto &kv) { return kv.first; };
        auto item_comparer = [&](const item_t x, const item_t y) { return frequencies[x] < frequencies[y]; };

        auto items = frequencies
                     | std::views::filter(is_frequent_item)
                     | std::views::transform(item_selector)
                     | std::ranges::to<std::vector>();

        std::ranges::sort(items, item_comparer);
        return items;
    }

    struct fp_tree_t {
        size_t min_support_abs;

        //        bool is_empty() const;
    };


    auto create_fp_tree(const transactions_t &itemsets, float min_support) -> fp_tree_t;
}
