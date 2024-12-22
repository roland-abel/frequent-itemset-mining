/// @file relim.h
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

#include <ranges>
#include "itemset.h"
#include "database.h"

namespace fim::algorithm::relim {
    using std::views::transform;
    using std::ranges::to;
    using std::views::drop;
    using std::views::take;

    using namespace fim;

    /// Suffix type with number of occurrences.
    struct suffix_t {
        size_t count{};
        itemset_t itemset{};
    };

    /// List of suffixes.
    struct suffixes_t : std::list<suffix_t> {
        using std::list<suffix_t>::list;

        /// Adds an itemset to the list, maintaining lexicographical order.
        /// @param itemset The itemset to be added to the list.
        /// @param compare The comparison function used to determine the order of the items in the list.
        /// @param count The current number of items in the list.
        auto add_itemset(
            const itemset_t &itemset,
            const item_compare_t &compare,
            size_t count = 1) -> void;
    };

    /// Head element
    struct header_element_t {
        size_t count = 0;
        item_t prefix{};
        suffixes_t suffixes{};
    };

    /// Header type
    using header_t = std::vector<header_element_t>;

    /// @brief A structure that represents a conditional database use for RElim algorithm.
    struct conditional_database_t {
        header_t header{};
        item_compare_t compare{};

        /// @brief Constructs a conditional database from a set of frequent items.
        /// @param freq_items A set of frequent items.
        /// @param compare A comparison function used to compare items.
        explicit conditional_database_t(const itemset_t &freq_items, const item_compare_t &compare);

        /// @brief Creates the initial conditional database.
        /// @param database The original database that contains all transactions.
        /// @param freq_items A set of frequent items.
        /// @param compare A comparison function used to compare items.
        /// @return A new conditional database created from the given parameters.
        static auto create_initial_database(
            const database_t &database,
            const itemset_t &freq_items,
            const item_compare_t &compare) -> conditional_database_t;

        /// @brief Returns a conditional database filtered by the prefix.
        /// @return A new conditional database that only includes transactions with the given prefix.
        [[nodiscard]] auto get_prefix_database() const -> conditional_database_t;

        /// @brief Eliminates items from the database based on the given prefix database.
        /// @param prefix_db A conditional database.
        /// @return The item that is eliminated after filtering the prefix database.
        auto eliminate(const conditional_database_t &prefix_db) -> item_t;
    };

    /// @brief Implements the RElim algorithm to find frequent itemsets in the database.
    /// @param database The input database containing transactions.
    /// @param min_support The minimum support threshold used to filter frequent itemsets.
    /// @return A collection of frequent itemsets that meet the minimum support criteria.
    auto relim_algorithm(const database_t &database, size_t min_support) -> itemsets_t;

    /// @brief Implements the RElim algorithm to find frequent itemsets in the database.
    /// @param database A tuple containing the reduced database and item's frequencies.
    /// @param min_support The minimum support threshold used to filter frequent itemsets.
    /// @return A collection of frequent itemsets that meet the minimum support criteria.
    auto relim_algorithm_(const database_counts_t &database, size_t min_support) -> itemsets_t;
}
