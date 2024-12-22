/// @file algorithms.h
/// @brief Enum type and mapping to frequent itemset mining algorithms.
///
/// @author Roland Abel
/// @date July 07, 2024
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
#include "apriori.h"
#include "fp_growth.h"
#include "relim.h"
#include "eclat.h"

namespace fim::algorithm {
    /// Define a type alias for a function that takes a database and a minimum support value as inputs,
    /// and perform a frequent itemset mining algorithm.
    using algorithm_function_t = std::function<itemsets_t(const database_counts_t &database, size_t min_support)>;

    /// Enum of frequent itemset mining algorithms.
    enum class algorithm_t : int {
        APRIORI,
        FP_GROWTH,
        RELIM,
        ECLAT
    };

    // Map from an enum to function pointers representing frequent itemset mining algorithms.
    const auto map_algorithm_function = std::map<algorithm_t, algorithm_function_t>{
        {algorithm_t::APRIORI, apriori::apriori_algorithm_},
        {algorithm_t::FP_GROWTH, fp_growth::fp_growth_algorithm_},
        {algorithm_t::RELIM, relim::relim_algorithm_},
        {algorithm_t::ECLAT, eclat::eclat_algorithm_}
    };

    /// @brief Retrieves the algorithm function associated with the specified enum type.
    /// @param algorithm The specified enum algorithm type.
    /// @return A function pointer to the algorithm that corresponds to the provided algorithm type.
    inline algorithm_function_t get_algorithm(const algorithm_t algorithm) {
        return map_algorithm_function.at(algorithm);
    }
}
