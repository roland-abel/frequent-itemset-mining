/// @file apriori.cpp
/// @brief
///
/// @author Roland Abel
/// @date July 07, 2024
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

#include "apriori.h"

namespace fim::algorithms::apriori {

    using std::views::transform;
    using std::views::filter;
    using std::ranges::to;

    using namespace fim::itemset;
    using namespace fim::algorithms::apriori;

    auto all_frequent_1_itemsets(const database_t &database, size_t min_support) -> itemsets_t {
        const auto is_frequent = [=](const auto &pair) -> bool { return pair.second >= min_support; };
        const auto get_item = [](const auto &pair) -> itemset_t { return itemset_t{pair.first}; };

        item_count_t item_count{};
        for (const item_t &item: database | std::views::join) {
            ++item_count[item];
        }

        return item_count
               | filter(is_frequent)
               | transform(get_item)
               | to<itemsets_t>();
    }

    auto generate_candidates(const itemsets_t &frequent_itemsets, size_t k) -> itemsets_t {
        // Check whether the k-2 first items of x and y are matched
        auto merge_itemsets_if_equal_prefix = [&](const itemset_t &x, const itemset_t &y) -> std::optional<itemset_t> {
            if (std::ranges::equal(x | std::views::take(k - 2), y | std::views::take(k - 2))) {
                itemset_t candidate{};
                std::ranges::copy(x | std::views::take(k - 1), std::back_inserter(candidate));
                candidate.push_back(y[k - 2]);
                std::ranges::sort(candidate);

                return candidate;
            }
            return std::nullopt;
        };

        // Combine pairs of frequent frequent_itemsets
        auto create_candidates = [&]() -> itemsets_t {
            itemsets_t candidates{};
            for (auto x = frequent_itemsets.begin(); x != frequent_itemsets.end(); x++) {
                for (auto y = std::next(x); y != frequent_itemsets.end(); y++) {
                    if (auto matched = merge_itemsets_if_equal_prefix(*x, *y); matched) {
                        candidates.emplace_back(std::move(*matched));
                    }
                }
            }
            return candidates;
        };

        auto all_subset_frequent = [&](const itemset_t &candidate) -> bool {
            auto is_frequent = [&](const itemset_t &itemset) {
                return std::ranges::contains(frequent_itemsets, itemset);
            };

            auto create_subset = [&](const item_t &item) {
                return candidate
                       | filter([&](const item_t &i) { return i != item; })
                       | to<itemset_t>();
            };

            return std::ranges::all_of(candidate | transform(create_subset), is_frequent);
        };

        return create_candidates()
               | filter(all_subset_frequent)
               | to<itemsets_t>();
    }

    auto prune(itemsets_t &candidates, const database_t &database, size_t min_support) -> void {
        const auto count = get_support_count(database, candidates, is_subset);
        const auto is_infrequent = [&](const itemset_t &z) -> bool {
            return !count.contains(z) || count.at(z) < min_support;
        };

        std::erase_if(candidates, is_infrequent);
    }

    auto apriori_algorithm(const database_t &database, size_t min_support) -> itemsets_t {
        itemsets_t frequent_itemsets{};
        auto insert_itemset = [&](const auto &itemsets) {
            std::ranges::copy(itemsets, std::back_inserter(frequent_itemsets));
        };

        // Find all 1-element suffix
        auto itemsets = all_frequent_1_itemsets(database, min_support);
        insert_itemset(itemsets);

        for (auto k = 2; !itemsets.empty(); k++) {
            // Create k-suffix from the previous (k-1)-suffix
            itemsets = generate_candidates(itemsets, k);

            // Remove all suffix with low support
            prune(itemsets, database, min_support);

            // Insert frequent candidates
            insert_itemset(itemsets);
        }
        return frequent_itemsets;
    }
}
