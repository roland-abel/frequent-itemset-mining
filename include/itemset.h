/// @file suffix.h
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

#include <ostream>
#include <algorithm>
#include <ranges>
#include <vector>
#include <functional>
#include <unordered_map>

namespace fim::itemset {

    // The prefix type.
    using item_t = unsigned long;
    using item_compare_t = std::function<bool(const item_t &, const item_t &)>;

    auto default_item_comparer(const item_t &i, const item_t &j) -> bool;

    // The suffix type.
    struct itemset_t : public std::vector<item_t> {
        using std::vector<item_t>::vector;

        ///
        /// @param items
        itemset_t(std::initializer_list<item_t> items);

        ///
        /// @param item
        /// @return
        auto add(const item_t &item) -> itemset_t &;

        ///
        /// @param subset
        /// @param superset
        /// @return
        [[nodiscard]] auto is_subset(const itemset_t &superset) const -> bool;

        /// Gets the union of two suffix.
        /// @param x The first suffix.
        /// @param y The second suffix.
        /// @return The union of the two suffix.
        [[nodiscard]] auto set_union(const itemset_t &y) const -> itemset_t;

        /// Gets the difference of two suffix (elements in x but not in y).
        /// @param x The first suffix.
        /// @param y The second suffix.
        /// @return The difference of the two sets.
        [[nodiscard]] auto set_difference(const itemset_t &y) const -> itemset_t;

        ///
        /// @param item
        /// @return
        [[nodiscard]] auto contains(const item_t &item) const -> bool;

        /// Sorts the items.
        auto sort_itemset(const item_compare_t &comp = default_item_comparer) -> itemset_t &;

        ///
        /// @param to_remove
        /// @return
        [[nodiscard]] auto remove_item(const item_t &to_remove) const -> itemset_t;
    };

    ///
    /// @param x
    /// @param y
    /// @param comp
    /// @return
    auto lexicographical_compare(
            const itemset_t &x,
            const itemset_t &y,
            const item_compare_t &comp = default_item_comparer) -> bool;

    // Collection of prefix sets.
    struct itemsets_t : public std::vector<itemset_t> {
        using std::vector<itemset_t>::vector;

        auto add(const itemset_t &itemset) -> void {
            emplace_back(itemset);
        };

        auto add(const itemsets_t &itemsets) -> void {
            for (const itemset_t &x: itemsets) {
                add(x);
            }
        };

        [[nodiscard]] auto contains(const item_t &item) const -> bool {
            return std::ranges::contains(*this, itemset_t{item});
        };

        [[nodiscard]] auto contains(const itemset_t &itemset) const -> bool {
            return std::ranges::contains(*this, itemset);
        };

        auto sort_each_itemset(const item_compare_t &comp = default_item_comparer) -> itemsets_t {
            for (auto &itemset: *this) {
                itemset.sort_itemset(comp);
            }
            return *this;
        }
    };

    // The transaction database type.
    struct database_t : public std::vector<itemset_t> {
        using std::vector<itemset_t>::vector;

        database_t(itemsets_t itemsets)
                : std::vector<itemset_t>(std::move(itemsets)) {
        }

        auto sort_lexicographically(const item_compare_t &comp = default_item_comparer) -> database_t {
            std::ranges::sort(*this, [&](const itemset_t &x, const itemset_t &y) {
                return lexicographical_compare(x, y, comp);
            });
            return *this;
        };

        /// @brief
        /// @param comp
        /// @return
        auto sort_database(const item_compare_t &comp = default_item_comparer) -> database_t {
            for (auto &trans: *this) {
                trans.sort_itemset(comp);
            }
            return sort_lexicographically();
        }
    };

    // Hash code type
    using code_t = unsigned long;

    // Hash function for an suffix
    struct itemset_hash {
        auto operator()(const itemset_t &itemset) const -> std::size_t;
    };

    // Item set counting
    using itemset_count_t = std::unordered_map<itemset_t, size_t, itemset_hash>;

    // Item counting
    struct item_count_t : public std::unordered_map<item_t, size_t> {
        using std::unordered_map<item_t, size_t>::unordered_map;

        ///
        /// @param database
        /// @return
        static auto get_item_count(const database_t &database) -> item_count_t;

        ///
        /// @param min_support
        /// @return
        [[nodiscard]] auto get_frequent_items(size_t min_support) const -> itemset_t;


        /// @brief
        /// @return
        [[nodiscard]] auto get_item_comparer() const -> item_compare_t {
            return [&](const item_t &i, const item_t &j) -> bool {
                return at(i) < at(j);
            };
        }
    };

    /// Overloads the output stream operator to output an suffix to an ostream.
    /// @param os The output stream to write to.
    /// @param x The suffix to output.
    /// @return The output stream after writing the suffix.
    std::ostream &operator<<(std::ostream &os, const itemset_t &x);

    /// Computes the union of two suffix.
    /// @param x The first suffix.
    /// @param y The second suffix.
    /// @return The union of the two suffix.
    auto set_union(const itemset_t &x, const itemset_t &y) -> itemset_t;

    /// Computes the difference of two suffix (elements in x but not in y).
    /// @param x The first suffix.
    /// @param y The second suffix.
    /// @return The difference of the two sets.
    auto set_difference(const itemset_t &x, const itemset_t &y) -> itemset_t;

    ///
    /// @param x A sorted prefix set.
    /// @param y A sorted prefix set.
    /// @return
    auto is_subset(const itemset_t &x, const itemset_t &y) -> bool;

    /// TODO: Move
    using is_subset_t = std::function<bool(const itemset_t &x, const itemset_t &y)>;

    /// TODO: Move
    /// @param db
    /// @param itemsets
    /// @param is_subset
    /// @return
    auto get_support_count(const database_t &db,
                           const itemsets_t &itemsets,
                           const is_subset_t &is_subset = fim::itemset::is_subset) -> itemset_count_t;
}