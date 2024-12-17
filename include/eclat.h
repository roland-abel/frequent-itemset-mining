/// @file eclat.h
/// @brief Implementation of the ECLAT algorithm.
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

#include <set>
#include "database.h"
#include "itemset.h"

namespace fim::algorithm::eclat {
    // Transaction id type, representing a unique identifier for each transaction.
    using tid_t = size_t;

    // Transaction identification set, representing a collection of transaction ids.
    using tidset_t = std::set<tid_t>;

    // The vertical transaction database type, mapping item identifiers to the set of ids containing that item.
    using vertical_database_t = std::unordered_map<item_t, tidset_t>;

    /// @brief Computes the intersection of two transaction-id sets.
    /// @param x The first transaction id set (suffix).
    /// @param y The second transaction id set (suffix).
    /// @return The intersection of the two sets, containing only transaction ids that appear in both sets.
    auto set_intersection(const tidset_t &x, const tidset_t &y) -> tidset_t;

    /// @brief Converts the given transaction database to a vertical representation.
    /// @param database The transaction database, which is a collection of itemsets (transactions).
    /// @return A vertical database, which maps each item to the set of transaction ids that contain it.
    auto to_vertical_database(const database_t &database) -> vertical_database_t;

    /// @brief Implements the ECLAT algorithm to find frequent itemsets in the transaction database.
    /// @param database The transaction database used to find frequent itemsets.
    /// @param min_support The minimum support threshold for considering an itemset as frequent.
    /// @return A collection of frequent itemsets that meet or exceed the minimum support.
    auto eclat_algorithm(const database_t &database, size_t min_support) -> itemsets_t;
}
