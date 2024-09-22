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
#include <utility>
#include "itemset.h"

namespace fim::relim {

    using namespace itemset;


    /// @brief
    struct conditional_database2_t {
        using head_t = std::vector<size_t>;

        head_t head{};
        itemsets_t itemsets{};
        itemset_t frequent_items{};

        /// @brief
        /// @param frequent_items
        explicit conditional_database2_t(itemset_t frequent_items)
                : frequent_items(std::move(frequent_items)),
                  head(frequent_items.size(), 0),
                  itemsets(frequent_items.size(), itemset_t{}) {
        }

        /// @brief
        /// @param database
        /// @param count
        /// @param min_support
        /// @return
        static auto create(
                const database_t &database,
                const item_count_t &count,
                size_t min_support) -> conditional_database2_t {

            using std::views::drop;
            using std::ranges::to;

            const auto frequent_items = count.get_frequent_items(min_support);
            conditional_database2_t contaitional_db(frequent_items);

            auto it_head = contaitional_db.head.rbegin();
            auto it_item = frequent_items.rbegin();

            for (const itemset_t &trans: database) {
                const auto &prefix = trans.front();
                const auto &suffix = trans | drop(1) | to<itemset_t>();

                if (prefix != *it_item) {
                    it_head++;
                    it_item++;
                }





            }
            return contaitional_db;
        }

        auto insert(const itemset_t &itemset) -> void {
            auto split_itemset = [&]() -> std::pair<item_t, itemset_t> {
                itemset_t suffix{};
                std::copy(std::next(itemset.begin()), itemset.end(), std::back_inserter(suffix));

                return std::make_pair(itemset.front(), suffix);
            };

            const auto [prefix, suffix] = split_itemset();


        }
    };


    /// @brief
    class conditional_database_t {
    public:
        struct data_t {
            size_t count{};
            itemset_t itemset{};
        };

        using data_list_t = std::list<data_t>;
        using map_t = std::unordered_map<item_t, std::pair<size_t, data_list_t>>;

        auto get(const item_t &item) -> std::pair<size_t, data_list_t> {
            return map_.at(item);
        }

//        auto data_list(const item_t &item) -> data_list_t {
//            return map_.at(item).second;
//        }

        auto insert(const itemset_t &itemset) -> void {
            auto split_itemset = [&]() -> std::pair<item_t, itemset_t> {
                itemset_t suffix{};
                std::copy(std::next(itemset.begin()), itemset.end(), std::back_inserter(suffix));

                return std::make_pair(itemset.front(), suffix);
            };

            const auto [prefix, suffix] = split_itemset();
            const auto [it, inserted] = map_.try_emplace(prefix, std::pair{0, data_list_t{}});

            if (inserted) {
                if (suffix.empty()) {
                    it->second.first++;
                } else {
                    it->second.first++;
                }
            } else {
                if (suffix.empty()) {
                    it->second.first++;
                } else {
                    it->second.second.emplace_back(data_t{0, suffix});
                    it->second.first++;
                }
            }

//            if (suffix.empty()) {
//                if (!inserted) {
//                    it->second.first++;
//                }
//            } else {
//                it->second.second.emplace_back(data_t{0, suffix});
//            }
//            it->second.first++;
        }

    private:
        map_t map_{};


    };

    // Removes all infrequent items from the database and sorts all item sets.
    /// @param database
    /// @param min_support
    /// @return
    auto preprocessing(database_t &database, size_t min_support) -> item_count_t;

    /// @brief
    /// @param database
    /// @param count
    /// @param min_support
    /// @return
    auto create_initial_database(database_t &database) -> conditional_database_t;

    ///
    /// @param database
    /// @param min_support
    /// @return
    auto relim_algorithm(const database_t &database, size_t min_support) -> itemsets_t;
}
