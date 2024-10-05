/// @file relim.cpp
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

#include <algorithm>
#include <ranges>
#include <functional>
#include "relim.h"

namespace fim::relim {

    using std::views::filter;
    using std::views::transform;

    auto preprocessing(database_t &database, size_t min_support) -> item_count_t {
        const auto &item_count = item_count_t::get_item_count(database);

        auto is_infreq_item = [&](const item_t &item) -> bool { return item_count.at(item) < min_support; };
        auto is_empty_itemset = [](const itemset_t &x) -> bool { return x.empty(); };

        for (itemset_t &trans: database) {
            trans.erase(std::remove_if(trans.begin(), trans.end(), is_infreq_item), trans.end());
            std::ranges::sort(trans, item_count.get_item_comparer());
        }

        const auto it = std::remove_if(database.begin(), database.end(), is_empty_itemset);
        database.erase(it, database.end());

        return item_count;
    }

//    auto relim_algorithm(const database_t &database, const itemset_t &frequent_items, size_t min_support) -> itemsets_t {
////        auto conditional_db = conditional_database_t::create_initial_database(database, frequent_items);
////
////        while (conditional_db.header.size() > 1) {
////            conditional_db.eliminate_next_item();
////        }
////
////        itemsets_t frequent_itemsets{};
////        return frequent_itemsets;
//    }
}