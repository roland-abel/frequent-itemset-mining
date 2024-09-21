/// @file apriori_hash_tree.h
/// @brief
///
/// @author Roland Abel
/// @date September 19, 2024
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

#include <iostream>
#include <vector>
#include <unordered_map>
#include <optional>
#include <set>
#include <algorithm>

//#include "dtypes.h"
#include "hash_tree.h"

using namespace rules::hash;

namespace rules::apriori_hash_tree {

    // The item type.
    using item_t = unsigned long;

    // The itemset type.
    using itemset_t = std::vector<item_t>;

    // Collection of itemsets.
    using itemsets_t = std::vector<itemset_t>;

    // The transaction database type.
    using database_t = std::vector<itemset_t>;

    ///
    /// @param subset
    /// @param superset
    /// @return
    bool is_subset(const itemset_t &subset, const itemset_t &superset) {
        return std::includes(superset.begin(), superset.end(), subset.begin(), subset.end());
    }

    ///
    /// @param itemset
    /// @param db
    /// @return
    auto count_support(const itemset_t &itemset, const database_t &db) -> size_t {
        int count = 0;
        for (const auto &transaction: db) {
            if (is_subset(itemset, transaction)) {
                ++count;
            }
        }
        return count;
    }

    itemsets_t generate_candidates(const itemsets_t &frequent_itemsets, size_t k) {
        itemsets_t candidates;
        size_t n = frequent_itemsets.size();

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 1; j < n; ++j) {
                itemset_t candidate;
                // Versuche, zwei Itemsets zu mergen, wenn die ersten k-1 Items identisch sind.


                if (std::equal(frequent_itemsets[i].begin(), std::next(frequent_itemsets[i].begin(), k - 2), frequent_itemsets[j].begin())) {
                    candidate = frequent_itemsets[i];
                    candidate.push_back(frequent_itemsets[j][k - 2]);
                    candidates.push_back(candidate);
                }
            }
        }

        return candidates;
    }

    // Entferne Kandidaten mit nicht häufigen Teilmengen.
    itemsets_t prune_candidates(const itemsets_t &candidates, const hash_tree &ht, size_t k) {
        itemsets_t pruned_candidates;

        for (const auto &candidate: candidates) {
            bool all_subsets_frequent = true;

            // Prüfe alle (k-1)-Teilmengen des Kandidaten.
            for (size_t i = 0; i < candidate.size(); ++i) {
                itemset_t subset = candidate;
                subset.erase(subset.begin() + i);  // Erzeuge eine (k-1)-Teilmenge.

                if (!ht.search(subset).has_value()) {
                    all_subsets_frequent = false;
                    break;
                }
            }

            if (all_subsets_frequent) {
                pruned_candidates.push_back(candidate);
            }
        }

        return pruned_candidates;
    }

    ///
    /// @param db
    /// @param min_support
    /// @return
    itemsets_t apriori_ht_algorithm(const database_t &db, size_t min_support) {
        auto hash_func = [](const item_t &item) {
            return item % 5;
        };

        hash_tree ht(5, hash_func);
        itemsets_t frequent_itemsets;
        size_t k = 1;

        // Initiale Kandidaten (1-Itemsets) bestimmen.
        std::unordered_map<item_t, int> item_count;
        for (const auto &transaction: db) {
            for (const auto &item: transaction) {
                ++item_count[item];
            }
        }

        // Filtere die häufigen 1-Itemsets nach minimaler Unterstützung.
        for (const auto &[item, count]: item_count) {
            if (count >= min_support) {
                itemset_t frequent_itemset = {item};
                frequent_itemsets.push_back(frequent_itemset);
                ht.insert(frequent_itemset);
            }
        }

        // Iterativ Kandidaten der Größe k+1 generieren, bis keine mehr existieren.
        while (!frequent_itemsets.empty()) {
            k++;

            // Kandidaten der Größe k generieren.
            itemsets_t candidates = generate_candidates(frequent_itemsets, k);

            // Kandidaten prunen.
            itemsets_t pruned_candidates = prune_candidates(candidates, ht, k);

            // Zähle Unterstützung für die übriggebliebenen Kandidaten.
            frequent_itemsets.clear();
            for (const auto &candidate: pruned_candidates) {
                int support_count = count_support(candidate, db);
                if (support_count >= min_support) {
                    frequent_itemsets.push_back(candidate);
                    ht.insert(candidate);
                }
            }
        }
        return frequent_itemsets;
    }
}
