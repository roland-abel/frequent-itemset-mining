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

    using std::views::transform;
    using std::ranges::to;
    using std::views::drop;
    using std::views::take;

    using namespace itemset;

    /// Suffix type with number of occurrences.
    struct suffix_t {
        size_t count{};
        itemset_t itemset{};
    };

    /// List of suffixes.
    using suffixes_t = std::list<suffix_t>;

    /// Head element
    struct header_element_t {
        size_t count{};
        item_t prefix{};
        suffixes_t suffixes{};

        ///
        /// @param suffix
        /// @param comp
        /// @param quantity
        auto add_suffix(
                const itemset_t &suffix,
                const item_compare_t &comp,
                const size_t quantity = 1) -> void {

            auto it = suffixes.begin();

            // search for insert position
            while (it != suffixes.end() && lexicographical_compare(it->itemset, suffix, comp)) {
                ++it;
            }

            // check, whether the itemset already exists
            if (it != suffixes.begin() && (std::prev(it)->itemset == suffix)) {
                // itemset found, increasing counter
                std::prev(it)->count += quantity;
            } else if (it != suffixes.end() && it->itemset == suffix) {
                // itemset found, increasing counter
                it->count += quantity;
            } else {
                // itemset haven't been found, insert a new suffix_t element
                suffixes.insert(it, suffix_t{quantity, suffix});
            }
        }
    };

    /// Header type
    using header_t = std::vector<header_element_t>;

    /// @brief
    struct conditional_database_t {
        header_t header{};
        item_compare_t item_comparer = default_item_comparer;

        /// @brief
        /// @param freq_items
        explicit conditional_database_t(const itemset_t &freq_items, item_compare_t comp)
                : item_comparer(std::move(comp)) {
            auto to_header_element = [](const item_t &item) {
                return header_element_t{0, item, suffixes_t{}};
            };
            header = freq_items | transform(to_header_element) | to<header_t>();
        }

        ///
        /// @param database
        /// @param freq_items
        /// @param item_comparer
        /// @return
        static auto create_initial_database(
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
                    it->add_suffix(suffix, comp);
                }
            }
            return conditional_db;
        }

        ///
        /// @return
        auto get_prefix_database() const -> conditional_database_t {
            const auto items = header
                               | take(header.size() - 1)
                               | transform([](const auto &x) { return x.prefix; })
                               | to<itemset_t>();

            conditional_database_t conditional_db(items, item_comparer);
            auto it = conditional_db.header.rbegin();

            for (const auto &[count, itemset]: header.back().suffixes) {
                const auto &prefix = itemset.front();
                const auto &suffix = itemset | drop(1) | to<itemset_t>();

                if (prefix != it->prefix) {
                    it++;
                }

                it->count++;
                if (not suffix.empty()) {
                    it->add_suffix(suffix, item_comparer, count);
                }
            }
            return conditional_db;
        }

        ///
        /// @param prefix_db
        /// @return
        auto eliminate(const conditional_database_t &prefix_db) -> item_t {
            const item_t eliminated_prefix = header.back().prefix;
            header.pop_back();

            auto it = header.rbegin();
            auto it_prefix = prefix_db.header.rbegin();

            while (it != header.rend() && it_prefix != prefix_db.header.rend()) {
                it->count += it_prefix->count;

                for (const auto &[count, itemset]: it_prefix->suffixes) {
                    it->add_suffix(itemset, item_comparer, count);
                }

                it++;
                it_prefix++;
            }
            return eliminated_prefix;
        }
    };

    // Removes all infrequent items from the database and sorts all prefix sets.
    /// @param database
    /// @param min_support
    /// @return
    auto preprocessing(database_t &database, size_t min_support) -> item_count_t;

    ///
    /// @param database
    /// @param min_support
    /// @return
    auto relim_algorithm(
            const database_t &database,
            const itemset_t &frequent_items,
            size_t min_support) -> itemsets_t;
}
