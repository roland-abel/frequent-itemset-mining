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

    auto preprocessing(database_t &database, size_t min_support) -> void {
        const auto &count = item_count_t::get_item_count(database);

        auto is_infrequent_item = [&](const item_t &item) -> bool {
            return count.at(item) < min_support;
        };

        auto is_empty_itemset = [](const itemset_t &x) -> bool {
            return x.empty();
        };

        for (itemset_t &trans: database) {
            trans.erase(std::remove_if(trans.begin(), trans.end(), is_infrequent_item), trans.end());

            std::ranges::sort(trans, [&](const item_t &a, const item_t &b) {
                return count.at(a) < count.at(b);
            });
        }

        database.erase(std::remove_if(database.begin(), database.end(), is_empty_itemset), database.end());
    }

    auto create_initial_database(database_t &database) -> relim_data_t {
        relim_data_t data{};
        for (const itemset_t &itemset: database) {
            data.insert(itemset);
        }

        return data;
    }


    auto projection_database(const database_t &database, const item_t &item) -> database_t {
        auto contains_item = [item](const itemset_t &x) { return x.contains(item); };
        auto remove_item = [item](const itemset_t &x) { return x.remove_item(item); };

        return database
               | filter(contains_item)
               | transform(remove_item)
               | std::ranges::to<database_t>();
    }

    auto relim_algorithm(const database_t &database, size_t min_support) -> itemsets_t {
//        auto is_frequent = [=](const auto &kv) { return kv.second >= min_support; };
//        auto get_item = [](const auto &kv) { return kv.first; };
//
//        itemsets_t frequent_itemsets{};
//
//        auto create_frequent_itemset = [&](const item_t &item, const itemset_t &prefix) -> itemset_t {
//            itemset_t itemset = prefix;
//            itemset.add(item);
//            frequent_itemsets.add(itemset);
//
//            return itemset;
//        };
//

//
//        using relim_func_t = std::function<void(const database_t &, const itemset_t &)>;
//        relim_func_t relim_ = [&](const database_t &db, const itemset_t &prefix) -> void {
//
//
//
//
//            const auto &frequent_items = get_item_count(db)
//                                         | filter(is_frequent)
//                                         | transform(get_item)
//                                         | std::ranges::to<std::vector>();
//
//            for (const auto &item: frequent_items) {
//                const auto &new_itemset = create_frequent_itemset(item, prefix);
//
//                database_t new_transactions{};
//                for (const auto &trans: db) {
//
//                    if (auto it = std::ranges::find(trans, item); it != trans.end()) {
//                        const auto &reduced_transaction = trans.remove_item(*it);
//                        if (!reduced_transaction.empty()) {
//                            new_transactions.emplace_back(reduced_transaction);
//                        }
//                    }
//
////                    if (auto it = std::ranges::find(trans, item); it != trans.end()) {
////
////                        itemset_t reduced_transaction = trans;  // TODO
////                        reduced_transaction.erase(*it);
////
////                        if (!reduced_transaction.empty()) {
////                            new_transactions.add(reduced_transaction);
////                        }
////                    }
//
//                    relim_(new_transactions, new_itemset);
//                }
//            }
//        };
//
//        relim_(database, {});

        itemsets_t frequent_itemsets{};

        auto create_frequent_itemset = [&](const item_t &item, const itemset_t &prefix) -> itemset_t {
            itemset_t itemset = prefix;
            return itemset.add(item);
        };

        using func_t = std::function<void(const database_t &, const itemset_t &)>;
        func_t find_frequent_itemsets = [&](const database_t &db, const itemset_t &prefix) {

            for (const auto &item: prefix) {

                const database_t &proj_db = projection_database(db, item);

                const auto &count = item_count_t::get_item_count(proj_db);
                const auto &items = count.get_frequent_items(min_support);

                if (!items.empty()) {
                    const auto &new_itemset = create_frequent_itemset(item, prefix);
                    frequent_itemsets.push_back(new_itemset);

                    find_frequent_itemsets(proj_db, new_itemset);
                }
            }
        };

//
//            const auto &count = item_count_t::get_item_count(db);
//            const auto &items = count.get_frequent_items(min_support);
//
//            database_t proj_db{};
//            for (const auto &item: items) {
//
//                const auto &new_itemset = create_frequent_itemset(item, prefix);
//                frequent_itemsets.push_back(new_itemset);
//
//                for (const auto trans: db) {
//                    if (trans.contains(item)) {
//                        const auto &reduced_transaction = trans.remove_item(item);
//                        if (!reduced_transaction.empty()) {
//                            proj_db.emplace_back(reduced_transaction);
//                        }
//                    }
//                    find_frequent_itemsets(proj_db, new_itemset);
//                }
//            }
//        };

        const auto &count = item_count_t::get_item_count(database);
        const auto &items = count.get_frequent_items(min_support);

        find_frequent_itemsets(database, items);
        return frequent_itemsets;
    }
}