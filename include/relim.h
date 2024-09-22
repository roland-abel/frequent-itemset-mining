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

namespace fim::relim {

    using namespace itemset;

    class relim_data_t {
    public:
        using pair_t = std::pair<size_t, itemsets_t>;
        using map_t = std::unordered_map<item_t, pair_t>;

        auto get_count(const item_t &item) -> size_t {
            return map_.at(item).first;
        }

        auto get_itemsets(const item_t &item) -> itemsets_t {
            return map_.at(item).second;
        }

        auto insert(const itemset_t &itemset) -> void {
            auto split = [&]() -> std::pair<item_t, itemset_t> {
                itemset_t tail{};
                std::copy(std::next(itemset.begin()), itemset.end(), std::back_inserter(tail));

                return std::make_pair(itemset.front(), tail);
            };

            const auto [prefix, suffix] = split();
            auto [it, inserted] = map_.try_emplace(prefix, std::pair{1, itemsets_t{}});

            if (suffix.empty()) {
                if (!inserted) {
                    it->second.first++;
                }
            } else {
                it->second.second.emplace_back(suffix);
            }
            it->second.first++;




//            const auto &[prefix, tail] = split();
//            auto it = map_.find(prefix);
//
//            if (it == map_.end()) {
//                if (!tail.empty()) {
//                    map_.insert(std::make_pair(prefix, std::make_pair<>(1, itemsets_t{tail})));
//                } else {
//                    it->second.first++;
//                }
//            } else {
//                if (!tail.empty()) {
//                    it->second.second.emplace_back(tail);
//                }
//                it->second.first++;
//            }
        }

    private:
        map_t map_{};
    };

    // Removes all infrequent items from the database and sorts all item sets.
    /// @param database
    /// @param min_support
    /// @return
    auto preprocessing(database_t &database, size_t min_support) -> void;

    ///
    /// @param database
    /// @return
    auto create_initial_database(database_t &database) -> relim_data_t;

    /// @param database
    /// @param item
    /// @return
    auto projection_database(const database_t &database, const item_t &item) -> database_t;

    ///
    /// @param database
    /// @param min_support
    /// @return
    auto relim_algorithm(const database_t &database, size_t min_support) -> itemsets_t;
}
