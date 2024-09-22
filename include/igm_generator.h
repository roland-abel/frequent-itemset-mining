/// @file igm_generator.h
/// @brief
///
/// @author Roland Abel
/// @date September 21, 2024
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
#include <functional>
#include <random>

namespace fim::generators::igm {

    using namespace fim::itemset;

    namespace {
        std::random_device rd;
        std::mt19937 gen(rd());
    }

    // Signature of a frequent itemset mining algorithm, like the Apriori algorithm
    using fim_algorithm_t = std::function<itemsets_t(const database_t &database, size_t min_support)>;

    ///
    /// @param itemsets
    /// @return
    auto generate_transaction(const itemsets_t &itemsets) -> itemset_t {
        itemset_t trans{};

        const auto get_sample_itemset = [&]() -> itemset_t {
            std::uniform_int_distribution<size_t> dist(0, itemsets.size() - 1);
            size_t idx = dist(gen);

            return itemsets[idx];
        };

        const auto get_sample_pattern = [&](const itemset_t &x) -> itemset_t {
            return itemset_t{};
        };

        const auto get_sample_noise = [&]() -> itemset_t {
            return itemset_t{};
        };


        return trans;
    }

    ///
    /// @param original_db The original database.
    /// @param min_support The minimal support of frequent itemsets.
    /// @param algorithm A frequent itemset mining algorithm, like the Apriori algorithm.
    /// @return A synthetic generated database.
    auto generate_database(const database_t &original_db, size_t min_support, const fim_algorithm_t &algorithm) -> database_t {
        using std::views::filter;

        const auto db_size = static_cast<double>(original_db.size());
        database_t synthetic_db{};

        const auto &frequent_itemsets = algorithm(original_db, min_support);
        const auto support_count = get_support_count(original_db, frequent_itemsets);

        auto is_significant = [&](const itemset_t &x) -> bool {
            return (support_count.at(x) / db_size) > (1. / std::pow(2, x.size()));
        };

        const auto &significant_itemsets = frequent_itemsets
                                           | filter(is_significant)
                                           | std::ranges::to<itemsets_t>();

        while (synthetic_db.size() < db_size) {
            const auto trans = generate_transaction(significant_itemsets);
            synthetic_db.emplace_back(trans);
        }

        return synthetic_db;
    }
}