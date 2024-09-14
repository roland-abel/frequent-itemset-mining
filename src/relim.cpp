/// @file relim.cpp
/// @brief
///
/// @author Roland Abel
/// @date September 12, 2024
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
#include "relim.h"

namespace rules::relim {

    auto get_item_counts(const transactions_t &transactions) -> item_counts_t {
        item_counts_t item_counts{};

        for (const auto &item: transactions | std::views::join) {
            ++item_counts[item];
        }
        return item_counts;
    }

    auto relim_algorithm(const transactions_t &transactions, size_t min_support) -> itemsets_t {
        auto is_frequent = [=](const auto &kv) { return kv.second >= min_support; };
        auto get_item = [](const auto &kv) { return kv.first; };

        itemsets_t frequent_itemsets{};

        auto create_frequent_itemset = [&](const item_t &item, const itemset_t &prefix) -> itemset_t {
            itemset_t itemset = prefix;
            itemset.insert(item);
            frequent_itemsets.insert(itemset);

            return itemset;
        };

        std::function<void(const transactions_t &, const itemset_t &)> relim_ = [&](
                const transactions_t &transactions,
                const itemset_t &prefix) -> void {

            const auto &frequent_items = get_item_counts(transactions)
                                         | std::views::filter(is_frequent)
                                         | std::views::transform(get_item)
                                         | std::ranges::to<std::vector>();

            for (const auto &item: frequent_items) {
                const auto &new_itemset = create_frequent_itemset(item, prefix);

                transactions_t new_transactions{};
                for (const auto &trans: transactions) {
                    if (trans.contains(item)) {
                        itemset_t reduced_transaction = trans;
                        reduced_transaction.erase(item);

                        if (!reduced_transaction.empty()) {
                            new_transactions.emplace_back(reduced_transaction);
                        }
                    }
                    relim_(new_transactions, new_itemset);
                }
            }
        };

        relim_(transactions, {});
        return frequent_itemsets;
    }
}