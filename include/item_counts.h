/// @file item_counts.h
/// @brief Structures and functions for counting items and itemsets
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

#pragma once

#include "itemset.h"

namespace fim {
    struct database_t;

    using counts_t = std::vector<size_t>;

    // Item counting
    struct item_counts_t : std::unordered_map<item_t, size_t> {
        using std::unordered_map<item_t, size_t>::unordered_map;

        /// @brief Gets the frequent items that meet the minimum support threshold.
        /// @param min_support The minimal support threshold for filtering frequent items.
        /// @return A sorted list of items that meet the minimum support threshold.
        [[nodiscard]] auto get_frequent_items(size_t min_support) const -> itemset_t;

        /// @brief Gets a comparator for items based on their support (count).
        /// @return A comparison function that can be used to compare two items based on their support.
        [[nodiscard]] auto get_item_compare() const -> item_compare_t;
    };

    // Item set counting
    struct itemset_counts_t : std::unordered_map<itemset_t, size_t, itemset_hash> {
        using std::unordered_map<itemset_t, size_t, itemset_hash>::unordered_map;

        /// @brief Creates a map of itemsets and their corresponding counts from the given transactions and itemsets.
        /// @param transactions A collection of transactions where each transaction is a set of items.
        /// @param itemsets A collection of itemsets for which counts need to be calculated.
        /// @param compare A comparison function used to compare items in the itemsets.
        /// @return A map (itemset_counts_t) that associates each itemset with its frequency count in the transactions.
        static auto create_itemset_counts(
            const database_t &transactions,
            const itemsets_t &itemsets,
            const item_compare_t &compare) -> itemset_counts_t;

        /// @brief Gets the count (frequency) of a specific itemset.
        /// @param itemsets The itemset whose count is to be retrieved.
        /// @return The count (frequency) of the specified itemset in the itemset counts map.
        [[nodiscard]] auto get_count(const itemset_t &itemsets) const -> size_t;
    };
}
