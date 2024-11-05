/// @file eclat.h
/// @brief
///
/// @author Roland Abel
/// @date September 08, 2024
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

#include <set>
#include "itemset.h"

namespace fim::algorithms::eclat {

    using namespace itemset;
    
    // Transaction id
    using tid_t = size_t;

    // Transactions identification set
    using tidset_t = std::set<tid_t>;

    // The vertical transaction database type.
    using vertical_database_t = std::unordered_map<item_t, tidset_t>;

    /// Computes the intersection of two tid sets.
    /// @param x The first suffix.
    /// @param y The second suffix.
    /// @return The union of the two suffix.
    auto set_intersection(const tidset_t &x, const tidset_t &y) -> tidset_t;

    /// Converts the given transaction data to a vertical representation of the data.
    /// @param database The transaction data
    /// @return The vertical representation of the transaction data.
    auto to_vertical_database(const database_t &database) -> vertical_database_t;

    ///
    /// @param database
    /// @param min_support
    /// @return
    auto eclat_algorithm(const database_t &database, size_t min_support) -> itemsets_t;
}