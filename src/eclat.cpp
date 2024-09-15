/// @file eclat.cpp
/// @brief
///
/// @author Roland Abel
/// @date September 10, 2024
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

#include <algorithm>
#include <ranges>
#include <functional>
#include "eclat.h"

namespace rules::eclat {

    auto set_intersection(const tidset_t &x, const tidset_t &y) -> tidset_t {
        tidset_t tidset{};
        std::set_intersection(x.begin(), x.end(), y.begin(), y.end(), std::inserter(tidset, tidset.begin()));

        return tidset;
    }

    auto to_vertical_database(const database_t &database) -> vertical_database_t {
        vertical_database_t vertical_trans{};
        for (tid_t tid = 0; tid < database.size(); ++tid) {
            const auto &trans = database[tid];

            for (const auto &item: trans) {
                vertical_trans[item].insert(tid);
            }
        }
        return vertical_trans;
    }

    auto eclat_algorithm(const database_t &database, size_t min_support) -> itemsets_t {
        itemsets_t frequent_itemsets{};

        // Creates initial tids.
        auto all_tids = [&]() -> tidset_t {
            return std::views::iota(size_t{0}, database.size()) | std::ranges::to<tidset_t>();
        };

        auto create_frequent_itemset = [&](const item_t &item, const itemset_t &prefix) -> itemset_t {
            itemset_t itemset = prefix;
            itemset.insert(item);
            frequent_itemsets.insert(itemset);

            return itemset;
        };

        std::function<void(const itemset_t &, const vertical_database_t &, const tidset_t &)> eclat_ = [&](
                const itemset_t &prefix,
                const vertical_database_t &vertical_trans,
                const tidset_t &current_tidset) -> void {

            for (auto it = vertical_trans.begin(); it != vertical_trans.end(); ++it) {
                const auto &[item, tidset] = *it;
                const auto &new_tidset = set_intersection(current_tidset, tidset);

                if (new_tidset.size() >= min_support) {
                    const auto &new_itemset = create_frequent_itemset(item, prefix);
                    vertical_database_t new_vertical_trans{};

                    for (auto jt = std::next(it); jt != vertical_trans.end(); ++jt) {
                        const auto &[new_item, new_item_tidset] = *jt;
                        const auto &intersected_tidset = set_intersection(new_tidset, new_item_tidset);

                        if (!intersected_tidset.empty()) {
                            new_vertical_trans[new_item] = intersected_tidset;
                        }
                    }

                    // Recursive call
                    eclat_(new_itemset, new_vertical_trans, new_tidset);
                }
            }
        };

        const auto &vertical_trans = to_vertical_database(database);
        eclat_({}, vertical_trans, all_tids());

        return frequent_itemsets;
    }
}