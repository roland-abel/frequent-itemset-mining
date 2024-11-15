/// @file utils.h
/// @brief
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

#include "itemset.h"
#include "apriori.h"
#include "fp_growth.h"
#include "relim.h"
#include "eclat.h"

namespace fim {

    ///
    using algorithm_function_t = std::function<itemsets_t(database_t &database, size_t min_support)>;

    ///
    enum class algorithm_t : int {
        APRIORI,
        FP_GROWTH,
        RELIM,
        ECLAT
    };

    struct configuration_t {
        std::string input_path;
        std::string output_path;
        float min_support;
        algorithm_t algorithm;
    };

    const auto map_algorithm_function = std::map<algorithm_t, algorithm_function_t>{
            {algorithm_t::APRIORI,   fim::algorithm::apriori::apriori_algorithm},
            {algorithm_t::FP_GROWTH, fim::algorithm::fp_growth::fp_growth_algorithm},
            {algorithm_t::RELIM,     fim::algorithm::relim::relim_algorithm},
            {algorithm_t::ECLAT,     fim::algorithm::eclat::eclat_algorithm},
    };

    algorithm_function_t get_algorithm(const algorithm_t algorithm) {
        return map_algorithm_function.at(algorithm);
    }
}