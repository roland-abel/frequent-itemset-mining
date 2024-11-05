/// @file fp_growth.h
/// @brief
///
/// @author Roland Abel
/// @date August 10, 2024
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

#include "utils.h"
#include "fp_tree.h"

namespace fim::algorithms::fp_growth {

    using namespace fim;
    using namespace fim::fp_tree;

    /// Generates the conditional transaction from the given FP-Tree.
    /// @param node The current node in the FP-Tree being processed.
    /// @param item  The prefix for which conditional database_ are being generated.
    /// @return The database_ that will be populated with the conditional database_.
    auto conditional_transactions(const node_ptr &node, item_t item) -> database_t;

    /// Applies the FP-Growth algorithm to find frequent suffix from the given database_.
    /// @param database A collection of transactions, where each transaction is a set of items.
    /// @param min_support The minimum support threshold for an suffix to be considered frequent.
    /// @return A set of frequent suffix, where each suffix is a collection of items that meet
    /// the minimum support threshold.
    auto fp_growth_algorithm(const database_t &database, size_t min_support) -> itemsets_t;
}