/// @file apriori.h
/// @brief
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
#include <algorithm>
#include "itemset.h"

namespace fim::algorithms::apriori {

    using std::views::transform;
    using std::views::filter;

    using namespace fim::itemset;

    ///
    /// @param database
    /// @param min_support
    /// @return
    auto all_frequent_1_itemsets(const database_t &database, size_t min_support) -> itemsets_t;

    /// Generate a candidate frequent itemsets of size k from frequent itemsets of size k-1.
    /// @param frequent_itemsets
    /// @param k
    /// @return
    auto generate_candidates(const itemsets_t &frequent_itemsets, size_t k) -> itemsets_t;

    ///
    /// @param candidates
    /// @param database
    /// @param support_count
    auto prune(itemsets_t &candidates, const database_t &database, size_t min_support) -> void;;

    /// The Apriori algorithm implementation.
    /// @param database
    /// @param min_support
    /// @return
    itemsets_t apriori_algorithm(const database_t &database, size_t min_support);
}
