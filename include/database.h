/// @file database.h
/// @brief Implementation of the database containing the list of transactions.
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
#include "item_counts.h"

namespace fim {
    using database_counts_t = std::tuple<database_t, item_counts_t>;

    // The transaction database type.
    struct database_t : std::vector<itemset_t> {
        using std::vector<itemset_t>::vector;

        /// @brief Constructor that initializes the database with a collection of itemsets.
        /// @param itemsets A collection of itemsets to initialize the database.
        explicit database_t(itemsets_t itemsets);

        /// @brief Sorts the database lexicographically using the provided comparison function.
        /// @param compare A comparison function used to sort the itemsets lexicographically.
        /// @return A new sorted database.
        auto sort_lexicographically(const item_compare_t &compare) -> database_t;

        /// @brief Gets the frequencies of all items in the database.
        /// @return A collection of item frequencies.
        [[nodiscard]] auto get_item_counts() const -> item_counts_t;

        /// @brief Removes all infrequent items from the database and sorts all prefix sets.
        /// @param min_support The minimum support threshold used to filter infrequent items.
        /// @return A tuple containing the reduced database and item's frequencies.
        auto reduce_database(size_t min_support) -> database_counts_t;

        /// @brief Removes all infrequent items from the database and sorts all prefix sets (const version).
        /// @param min_support The minimum support threshold used to filter infrequent items.
        /// @return A tuple containing the reduced database and item's frequencies.
        [[nodiscard]] auto transaction_reduction(size_t min_support) const -> database_counts_t;
    };
}
