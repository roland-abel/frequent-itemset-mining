/// @file item_count.h
/// @brief
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

#include "itemset.h"

namespace fim {

    using counts_t = std::vector<size_t>;

    // Item counting
    struct item_counts_t : public std::unordered_map<item_t, size_t> {
        using std::unordered_map<item_t, size_t>::unordered_map;

        /// Gets the frequent items.
        /// @param min_support The minimal support.
        /// @return The sorted list of items.
        [[nodiscard]] auto get_frequent_items(size_t min_support) const -> itemset_t;

        /// @brief Gets a comparator for items based on their support.
        /// @return Comparer function with signature `bool(const item_t &i, const item_t &j)`.
        [[nodiscard]] auto get_item_compare() const -> item_compare_t;

        /// @brief 
        /// @return
        auto reverse_item_compare() const -> item_compare_t;
    };

    // Item set counting
    struct itemset_counts_t : public std::unordered_map<itemset_t, size_t, itemset_hash> {
        using std::unordered_map<itemset_t, size_t, itemset_hash>::unordered_map;

        /// @brief 
        /// @param transactions 
        /// @param itemsets 
        /// @param compare 
        /// @return 
        static auto create_itemset_counts(
                const itemsets_t& transactions,
                const itemsets_t& itemsets,
                const item_compare_t& compare) -> itemset_counts_t;

        ///
        /// @param itemsets
        /// @return
        auto get_count(const itemset_t& itemsets) const -> size_t;

        /// @brief 
        /// @return 
        auto get_counts() const -> counts_t;
    };
}