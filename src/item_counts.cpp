/// @file item_counts.cpp
/// @brief Implementation of itemset counter's type and functions namespace.
///
/// @author Roland Abel
/// @date December 11, 2024
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

#include "item_counts.h"
#include "database.h"
#include <algorithm>

namespace fim {
    auto item_counts_t::get_frequent_items(size_t min_support) const -> itemset_t {
        auto is_frequent = [=](const auto &kv) { return kv.second >= min_support; };
        auto to_item = [](const auto &kv) { return kv.first; };

        auto items = *this
                     | std::views::filter(is_frequent)
                     | std::views::transform(to_item)
                     | std::ranges::to<itemset_t>();

        std::ranges::sort(items, [&](const item_t &x, const item_t &y) {
            return at(x) > at(y);
        });

        return items;
    }

    auto item_counts_t::get_item_compare() const -> item_compare_t {
        return [&](const item_t &i, const item_t &j) -> bool {
            const auto weight_i = at(i);
            const auto weight_j = at(j);

            return weight_i != weight_j ? weight_i < weight_j : i < j;
        };
    }

    auto item_counts_t::get_item_reverse_compare() const -> item_compare_t {
        return [&](const item_t &i, const item_t &j) -> bool {
            const auto weight_i = at(i);
            const auto weight_j = at(j);

            return weight_i != weight_j ? weight_i > weight_j : i > j;
        };
    }

    auto itemset_counts_t::create_itemset_counts(
        const database_t &transactions,
        const itemsets_t &itemsets,
        const item_compare_t &compare) -> itemset_counts_t {
        itemset_counts_t count{};

        for (const itemset_t &x: itemsets) {
            for (const itemset_t &y: transactions) {
                if (x.is_subset(y, compare)) {
                    ++count[x];
                }
            }
        }
        return count;
    }

    auto itemset_counts_t::get_count(const itemset_t &itemset) const -> size_t {
        return this->contains(itemset) ? this->at(itemset) : 0;
    }

    auto itemset_counts_t::get_support(const itemset_t &itemset, const size_t db_size) const -> float {
        return static_cast<float>(get_count(itemset)) / static_cast<float>(db_size);
    }
}
