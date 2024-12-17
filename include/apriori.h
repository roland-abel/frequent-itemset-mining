/// @file apriori.h
/// @brief Implementation of the Apriori algorithm.
///
/// @author Roland Abel
/// @date September 20, 2024
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
#include "database.h"

namespace fim::algorithm::apriori {
    using namespace fim;

    /// @brief Generates all frequent one-itemsets from the given item counts, based on the minimum support threshold.
    /// @param item_counts A collection of item counts, representing the frequency of individual items in the database.
    /// @param min_support The minimum support value used to filter frequent items.
    /// @return A collection of frequent one-itemsets that meet or exceed the minimum support.
    auto all_frequent_one_itemsets(const item_counts_t &item_counts, size_t min_support) -> itemsets_t;

    /// @brief Generates candidate frequent itemsets of size k from frequent itemsets of size k-1.
    /// @param frequent_itemsets A collection of frequent itemsets of size k-1.
    /// @param k The size of the itemsets to generate (the size of the new candidate itemsets).
    /// @param compare A comparison function or object used to compare itemsets for potential combinations.
    /// @return A collection of candidate frequent itemsets of size k.
    auto generate_candidates(
        const itemsets_t &frequent_itemsets,
        size_t k,
        const item_compare_t &compare) -> itemsets_t;

    /// @brief Removes candidate itemsets that do not meet the minimum support threshold.
    /// @param candidates A collection of candidate itemsets that need to be pruned based on support.
    /// @param database The dataset used to calculate the support of the candidate itemsets.
    /// @param min_support The minimum support threshold. Itemsets with support lower than this value will be pruned.
    /// @param compare A comparison function or object used to compare itemsets, likely for sorting or filtering.
    /// @return This function modifies the candidates in place.
    auto prune(
        itemsets_t &candidates,
        const database_t &database,
        size_t min_support,
        const item_compare_t &compare) -> void;

    /// @brief Implements the Apriori algorithm to find frequent itemsets in the given database.
    /// @param database The database used to find frequent itemsets.
    /// @param min_support The minimum support threshold for considering an itemset as frequent.
    /// @return A collection of frequent itemsets that meet or exceed the minimum support.
    auto apriori_algorithm(const database_t &database, size_t min_support) -> itemsets_t;
}
