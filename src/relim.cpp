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
#include "relim.h"

namespace fim::algorithms::relim {

    using std::views::filter;
    using std::views::transform;

    auto suffixes_t::add_itemset(
            const itemset_t &itemset,
            const item_compare_t &comp,
            const size_t quantity) -> void {
        auto it = begin();

        // search for insert position
        while (it != end() && lexicographical_compare(it->itemset, itemset, comp)) {
            ++it;
        }

        if (it != end() && it->itemset == itemset) {
            // itemset found, increasing counter
            it->count += quantity;
        } else {
            // itemset haven't been found, insert a new suffix_t element
            insert(it, suffix_t{quantity, itemset});
        }
    }

    conditional_database_t::conditional_database_t(const itemset_t &freq_items, item_compare_t comp)
            : item_comparer(std::move(comp)) {
        auto to_header_element = [](const item_t &item) {
            return header_element_t{0, item, suffixes_t{}};
        };

        header = freq_items
                 | transform(to_header_element)
                 | to<header_t>();
    }

    auto conditional_database_t::create_initial_database(
            const database_t &database,
            const itemset_t &freq_items,
            const item_compare_t &comp) -> conditional_database_t {

        conditional_database_t conditional_db(freq_items, comp);
        auto it = conditional_db.header.rbegin();

        for (const itemset_t &trans: database) {
            const auto &prefix = trans.front();
            const auto &suffix = trans | drop(1) | to<itemset_t>();

            if (prefix != it->prefix) {
                it++;
            }

            it->count++;
            if (not suffix.empty()) {
                it->suffixes.add_itemset(suffix, comp);
            }
        }
        return conditional_db;
    }

    auto conditional_database_t::get_prefix_database() const -> conditional_database_t {
        const auto &[support, item, suffixes] = header.back();
        const auto items = header
                           | take(header.size() - 1)
                           | transform([](const auto &x) { return x.prefix; })
                           | to<itemset_t>();

        conditional_database_t conditional_db(items, item_comparer);
        auto it = conditional_db.header.rbegin();

        for (const auto &[count, itemset]: suffixes) {
            const auto &prefix = itemset.front();
            const auto &suffix = itemset | drop(1) | to<itemset_t>();

            if (prefix != it->prefix) {
                it++;
            }

            it->count += count;
            if (not suffix.empty()) {
                it->suffixes.add_itemset(suffix, item_comparer, count);
            }
        }
        return conditional_db;
    }

    auto preprocessing(database_t &database, size_t min_support) -> item_count_t {
        const auto &item_count = item_count_t::get_item_count(database);
        const auto &comparer = item_count.get_item_comparer();

        auto is_infreq_item = [&](const item_t &item) -> bool {
            return item_count.at(item) < min_support;
        };

        auto is_empty_itemset = [](const itemset_t &x) -> bool {
            return x.empty();
        };

        for (itemset_t &trans: database) {
            trans.erase(std::remove_if(trans.begin(), trans.end(), is_infreq_item), trans.end());
            std::ranges::sort(trans, comparer);
        }

        const auto it = std::remove_if(database.begin(), database.end(), is_empty_itemset);
        database.erase(it, database.end());

        return item_count;
    }

    auto relim_algorithm(database_t &database, size_t min_support) -> itemsets_t {
        itemsets_t freq_itemsets{};

        const auto item_count = preprocessing(database, min_support);
        const auto comp = item_count.get_item_comparer();

        auto combine = [&](const itemset_t &prefix, const itemset_t &suffix) -> itemset_t {
            auto itemset = prefix.set_union(suffix);
            itemset.sort_itemset(comp);

            return itemset;
        };

        auto report_freq_itemset = [&](const itemset_t &itemset, size_t support) -> itemset_t {
            freq_itemsets.emplace_back(itemset);
            return itemset;
        };

        database.sort_lexicographically(comp);

        using func_t = std::function<void(const itemset_t &, conditional_database_t &)>;
        func_t relim_algorithm_ = [&](const itemset_t &prefix, conditional_database_t &cond_db) -> void {
            while (not cond_db.header.empty()) {
                auto elem = cond_db.header.back();
                auto new_prefix = combine(itemset_t{prefix}, itemset_t{elem.prefix});

                if (elem.count >= min_support) {
                    report_freq_itemset(new_prefix, elem.count);
                }

                auto prefix_db = cond_db.get_prefix_database();
                cond_db.eliminate(prefix_db);

                relim_algorithm_(new_prefix, prefix_db);
            }
        };

        const auto &freq_items = item_count.get_frequent_items(min_support);
        auto conditional_db = conditional_database_t::create_initial_database(database, freq_items, comp);

        relim_algorithm_({}, conditional_db);
        return freq_itemsets;
    }
}