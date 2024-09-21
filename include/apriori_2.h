/// @file apriori.h
/// @brief
/// @see: https://www.macs.hw.ac.uk/~dwcorne/Teaching/agrawal94fast.pdf
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

#pragma once

//#include "dtypes.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <ranges>
#include <algorithm>
#include <iostream>

namespace fim::apriori_2 {

    using std::views::transform;
    using std::views::filter;

    // The item type
    using item_t = unsigned long;

    // The itemset type
    using itemset_t = std::vector<item_t>;

    // Collection of itemsets
    using itemsets_t = std::vector<itemset_t>;

    // Transaction database type
    using database_t = std::vector<itemset_t>;

    // Hash function for an itemset
    struct itemset_hash {
        std::size_t operator()(const itemset_t &itemset) const {
            std::size_t seed = 0;
            for (const auto &item: itemset) {
                seed ^= std::hash<item_t>{}(item) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };

    // Support counting
    using support_count_t = std::unordered_map<itemset_t, size_t, itemset_hash>;

    ///
    using item_count_t = std::unordered_map<item_t, size_t>;

    /// Function to check if an itemset x is a subset of an other itemset y.
    /// @param x The sorted itemset to examine.
    /// @param y The sorted itemset.
    /// @return
    auto is_subset(const itemset_t &x, const itemset_t &y) -> bool {
        return std::ranges::includes(y, x);
    }

    ///
    /// @param db
    /// @param min_support
    /// @return
    auto all_frequent_1_itemsets(const database_t &db, size_t min_support) -> itemsets_t {
        const auto is_frequent = [=](const auto &pair) -> bool { return pair.second >= min_support; };
        const auto get_item = [](const auto &pair) -> itemset_t { return itemset_t{pair.first}; };

        item_count_t item_count{};
        for (const item_t &item: db | std::views::join) {
            ++item_count[item];
        }

        return item_count
               | filter(is_frequent)
               | transform(get_item)
               | std::ranges::to<itemsets_t>();
    }

    // Generate a candidate frequent itemsets of size k from frequent itemsets of size k-1
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
                       | std::ranges::to<itemset_t>();
            };

            return std::ranges::all_of(candidate | transform(create_subset), is_frequent);
        };

        return create_candidates()
               | filter(all_subset_frequent)
               | std::ranges::to<itemsets_t>();
    }

    ///
    /// @param candidates
    /// @param db
    /// @param support_count
    auto prune(itemsets_t &candidates, const database_t &db, size_t min_support) -> void {
        support_count_t support_count{};

        // Support counting for candidate itemsets
        for (const auto &trans: db) {
            for (const auto &candidate: candidates) {
                if (is_subset(candidate, trans)) {
                    ++support_count[candidate];
                }
            }
        }

        const auto is_infrequent = [&](const itemset_t &z) -> bool { return support_count.at(z) < min_support; };
        std::erase_if(candidates, is_infrequent);
    };

    // Support counting for candidate itemsets
    void count_support(const itemsets_t &candidates, const database_t &db, support_count_t &support_count) {
        for (const auto &transaction: db) {
            for (const auto &candidate: candidates) {
                if (is_subset(candidate, transaction)) {
                    ++support_count[candidate];
                }
            }
        }
    }

    // The Apriori algorithm implementation
    itemsets_t apriori(const database_t &db, size_t min_support) {
        itemsets_t frequent_itemsets{};
        auto insert_itemset = [&](const auto &itemsets) {
            std::ranges::copy(itemsets, std::back_inserter(frequent_itemsets));
        };

        // Find all 1-element itemsets
        auto itemsets = all_frequent_1_itemsets(db, min_support);
        insert_itemset(itemsets);

        for (auto k = 2; !itemsets.empty(); k++) {
            // Create k-itemsets from the previous (k-1)-itemsets
            itemsets = generate_candidates(itemsets, k);

            // Remove all itemsets with low support
            prune(itemsets, db, min_support);

            // Insert frequent candidates
            insert_itemset(itemsets);
        }
        return frequent_itemsets;
    }
}
