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

#include <ranges>
#include <utility>
#include "itemset.h"

namespace fim::relim {
    using std::views::transform;
    using std::ranges::to;
    using std::views::drop;
    using std::views::take;

    using namespace fim;
    using namespace itemset;

    /// Suffix type with number of occurrences.
    struct suffix_t {
        size_t count{};
        itemset_t itemset{};
    };

    /// List of suffixes.
    struct suffixes_t : std::list<suffix_t> {
        using std::list<suffix_t>::list;

        ///
        /// @param itemset
        /// @param comp
        /// @param quantity
        auto add_itemset(
                const itemset_t &itemset,
                const item_compare_t &comp,
                const size_t quantity = 1) -> void;
    };

    /// Head element
    struct header_element_t {
        size_t count = 0;
        item_t prefix{};
        suffixes_t suffixes{};
    };

    /// Header type
    using header_t = std::vector<header_element_t>;

    /// @brief
    struct conditional_database_t {
        header_t header{};
        item_compare_t item_comparer = default_item_comparer;

        /// @brief
        /// @param freq_items
        explicit conditional_database_t(const itemset_t &freq_items, item_compare_t comp);

        ///
        /// @param database
        /// @param freq_items
        /// @param item_comparer
        /// @return
        static auto create_initial_database(
                const database_t &database,
                const itemset_t &freq_items,
                const item_compare_t &comp) -> conditional_database_t;

        ///
        /// @return
        auto get_prefix_database() const -> conditional_database_t;

        ///
        /// @param prefix_db
        /// @return
        auto eliminate(const conditional_database_t &prefix_db) -> item_t {
            const auto &prefix = header.back().prefix;
            header.pop_back();

            auto it = header.rbegin();
            auto it_prefix = prefix_db.header.rbegin();

            while (it != header.rend() && it_prefix != prefix_db.header.rend()) {
                it->count += it_prefix->count;

                for (const auto &[count, itemset]: it_prefix->suffixes) {
                    it->suffixes.add_itemset(itemset, item_comparer, count);
                }

                it++;
                it_prefix++;
            }
            return prefix;
        }
    };

    // Removes all infrequent items from the database and sorts all prefix sets.
    /// @param database
    /// @param min_support
    /// @return
    auto preprocessing(database_t &database, size_t min_support) -> item_count_t;

    ///
    /// @param database
    /// @param cond_db
    /// @return
    auto relim_algorithm(database_t &database, size_t cond_db) -> itemsets_t;
}
