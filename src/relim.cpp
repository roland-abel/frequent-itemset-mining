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

#include "relim.h"
#include <algorithm>
#include <ranges>

namespace fim::algorithm::relim {
    using std::views::filter;
    using std::views::transform;

    auto suffixes_t::add_itemset(
        const itemset_t &itemset,
        const item_compare_t &compare,
        const size_t count) -> void {
        const auto sorted_itemset = itemset.sort_itemset(compare);
        auto it = begin();

        // search for insert position
        while (it != end() && lexicographical_compare(it->itemset, sorted_itemset, compare)) {
            ++it;
        }

        if (it != end() && it->itemset == sorted_itemset) {
            // itemset found, increasing counter
            it->count += count;
        } else {
            // itemset haven't been found, insert a new suffix_t element
            insert(it, suffix_t{count, sorted_itemset});
        }
    }

    conditional_database_t::conditional_database_t(const itemset_t &freq_items, const item_compare_t &compare)
        : compare(compare) {
        auto to_header_element = [](const item_t &item) {
            return header_element_t{0, item, suffixes_t{}};
        };

        auto items = freq_items.sort_itemset(compare);
        std::ranges::reverse(items);

        header = items
                 | transform(to_header_element)
                 | to<header_t>();
    }

    auto conditional_database_t::create_initial_database(
        const database_t &database,
        const itemset_t &freq_items,
        const item_compare_t &compare) -> conditional_database_t {
        conditional_database_t conditional_db(freq_items, compare);

        auto &header = conditional_db.header;
        auto it = header.rbegin();

        for (const itemset_t &trans: database) {
            const auto &prefix = trans.front();
            const auto &suffix = trans | drop(1) | to<itemset_t>();

            it = std::find_if(it, header.rend(), [&](const auto &h) {
                return h.prefix == prefix;
            });

            it->count++;
            if (not suffix.empty()) {
                it->suffixes.add_itemset(suffix, compare);
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

        conditional_database_t conditional_db(items, compare);

        auto &header = conditional_db.header;
        auto it = header.rbegin();

        for (const auto &[count, itemset]: suffixes) {
            // itemset == {prefix} + {suffix}
            const item_t &prefix = itemset.front();
            const itemset_t &suffix = itemset | drop(1) | to<itemset_t>();

            // find prefix
            it = std::find_if(it, header.rend(), [&](const auto &x) {
                return x.prefix == prefix;
            });

            it->count += count;

            if (not suffix.empty()) {
                it->suffixes.add_itemset(suffix, compare, count);
            }
        }
        return conditional_db;
    }

    auto conditional_database_t::eliminate(const conditional_database_t &prefix_db) -> item_t {
        const auto &prefix = header.back().prefix;
        header.pop_back();

        auto it = header.rbegin();
        auto it_prefix = prefix_db.header.rbegin();

        while (it != header.rend() && it_prefix != prefix_db.header.rend()) {
            it->count += it_prefix->count;

            for (const auto &[count, itemset]: it_prefix->suffixes) {
                it->suffixes.add_itemset(itemset, compare, count);
            }

            ++it;
            ++it_prefix;
        }
        return prefix;
    }

    auto relim_algorithm(const database_t &database, const size_t min_support) -> itemsets_t {
        itemsets_t freq_itemsets{};

        const auto [db, item_count] = database.transaction_reduction(min_support);
        const auto compare = item_count.get_item_compare();

        auto combine = [&](const itemset_t &prefix, const itemset_t &suffix) -> itemset_t {
            auto itemset = prefix.set_union(suffix);
            itemset.sort_itemset(compare);

            return itemset;
        };

        auto insert_itemset = [&](const itemset_t &itemset, size_t) -> itemset_t {
            freq_itemsets.emplace_back(itemset);
            return itemset;
        };

        using func_t = std::function<void(const itemset_t &, conditional_database_t &)>;
        func_t relim_algorithm_ = [&](const itemset_t &itemset_prefix, conditional_database_t &conditional_db) -> void {
            while (not conditional_db.header.empty()) {
                const auto [count, prefix, suffixes] = conditional_db.header.back();
                const auto new_prefix = combine(itemset_t{itemset_prefix}, itemset_t{prefix});

                if (count >= min_support) {
                    insert_itemset(new_prefix, count);
                }

                auto prefix_db = conditional_db.get_prefix_database();
                conditional_db.eliminate(prefix_db);

                relim_algorithm_(new_prefix, prefix_db);
            }
        };

        const auto &freq_items = item_count.get_frequent_items(min_support);
        auto conditional_db = conditional_database_t::create_initial_database(db, freq_items, compare);

        relim_algorithm_({}, conditional_db);
        return freq_itemsets;
    }
}
