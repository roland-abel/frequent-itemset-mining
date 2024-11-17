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
        /// The itemset is inserted into the list based on the comparison function,
        /// ensuring that the list remains sorted.
        ///
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

    /// @brief
    struct conditional_database_t {
        header_t header{};
        item_compare_t compare{};

        /// @brief
        /// @param freq_items
        /// @param compare
        explicit conditional_database_t(const itemset_t &freq_items, const item_compare_t &compare);

        ///
        /// @param database
        /// @param freq_items
        /// @param compare
        /// @return
        static auto create_initial_database(
            const database_t &database,
            const itemset_t &freq_items,
            const item_compare_t &compare) -> conditional_database_t;

        ///
        /// @return
        [[nodiscard]] auto get_prefix_database() const -> conditional_database_t;

        ///
        /// @param prefix_db
        /// @return
        auto eliminate(const conditional_database_t &prefix_db) -> item_t;
    };

    ///
    /// @param database
    /// @param min_support
    /// @return
    auto relim_algorithm(const database_t &database, size_t min_support) -> itemsets_t;
}
