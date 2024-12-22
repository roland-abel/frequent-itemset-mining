/// @file database.cpp
/// @brief Implementation of the database type.
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

#include <algorithm>
#include <utility>
#include "itemset.h"
#include "database.h"
#include "item_counts.h"

namespace fim {
    database_t::database_t(itemsets_t itemsets)
        : std::vector<itemset_t>(std::move(itemsets)) {
    }

    auto database_t::sort_lexicographically(const item_compare_t &compare) -> database_t {
        for (itemset_t &trans: *this) {
            std::ranges::sort(trans, compare);
        }

        std::ranges::sort(*this, [&](const itemset_t &x, const itemset_t &y) {
            return lexicographical_compare(x, y, compare);
        });
        return *this;
    }

    auto database_t::get_item_counts() const -> item_counts_t {
        item_counts_t counts{};
        for (const auto &item: *this | std::views::join) {
            ++counts[item];
        }
        return std::move(counts);
    }

    auto database_t::reduce_database(const size_t min_support) -> database_counts_t {
        const auto &item_counts = get_item_counts();

        auto is_infreq_item = [&](const item_t &item) -> bool {
            return item_counts.at(item) < min_support;
        };

        auto is_empty_itemset = [](const itemset_t &x) -> bool {
            return x.empty();
        };

        for (itemset_t &trans: *this) {
            std::erase_if(trans, is_infreq_item);
        }

        const auto it = std::ranges::remove_if(*this, is_empty_itemset).begin();
        erase(it, end());

        const auto &counts = get_item_counts();
        sort_lexicographically(counts.get_item_compare());

        return std::make_tuple(*this, counts);
    }

    auto database_t::transaction_reduction(const size_t min_support) const -> database_counts_t {
        database_t db(*this); // copy database
        return db.reduce_database(min_support);
    }
}
