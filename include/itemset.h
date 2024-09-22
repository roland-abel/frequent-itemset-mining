/// @file itemset.h
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

    // The item type.
    using item_t = unsigned long;

    // The itemset type.
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
        auto is_subset(const itemset_t &superset) const -> bool;

        /// Gets the union of two itemsets.
        /// @param x The first itemset.
        /// @param y The second itemset.
        /// @return The union of the two itemsets.
        auto set_union(const itemset_t &y) const -> itemset_t;

        /// Gets the difference of two itemsets (elements in x but not in y).
        /// @param x The first itemset.
        /// @param y The second itemset.
        /// @return The difference of the two sets.
        auto set_difference(const itemset_t &y) const -> itemset_t;

        ///
        /// @param item
        /// @return
        auto contains(const item_t &item) const -> bool;

        /// Sorts the items.
        auto sort() -> itemset_t &;

        ///
        /// @param to_remove
        /// @return
        auto remove_item(const item_t &to_remove) const -> itemset_t {
            return *this
                   | std::ranges::views::filter([&](const item_t &item) { return item != to_remove; })
                   | std::ranges::to<itemset_t>();
        }
    };

    // Collection of itemsets.
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

        auto contains(const item_t &item) const -> bool {
            return std::ranges::contains(*this, itemset_t{item});
        };

        auto contains(const itemset_t &itemset) const -> bool {
            return std::ranges::contains(*this, itemset);
        };
    };

    // The transaction database type.
    struct database_t : public std::vector<itemset_t> {
        using std::vector<itemset_t>::vector;
    };

    // Hash code type
    using code_t = unsigned long;

    // Hash function for an itemset
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
        auto get_frequent_items(size_t min_support) const -> itemset_t;
    };

    /// Overloads the output stream operator to output an itemset to an ostream.
    /// @param os The output stream to write to.
    /// @param x The itemset to output.
    /// @return The output stream after writing the itemset.
    std::ostream &operator<<(std::ostream &os, const itemset_t &x);

    /// Computes the union of two itemsets.
    /// @param x The first itemset.
    /// @param y The second itemset.
    /// @return The union of the two itemsets.
    auto set_union(const itemset_t &x, const itemset_t &y) -> itemset_t;

    /// Computes the difference of two itemsets (elements in x but not in y).
    /// @param x The first itemset.
    /// @param y The second itemset.
    /// @return The difference of the two sets.
    auto set_difference(const itemset_t &x, const itemset_t &y) -> itemset_t;

    ///
    /// @param x A sorted item set.
    /// @param y A sorted item set.
    /// @return
    auto is_subset(const itemset_t &x, const itemset_t &y) -> bool;


    /// TODO: Move
    using is_subset_t = std::function<bool(const itemset_t &x, const itemset_t &y)>;

    /// TODO: Move
    /// @param db
    /// @param itemsets
    /// @param is_subset
    /// @return
    auto get_support_count(const database_t &db, const itemsets_t &itemsets, const is_subset_t &is_subset = fim::itemset::is_subset) -> itemset_count_t;
}