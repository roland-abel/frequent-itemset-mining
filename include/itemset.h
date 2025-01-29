/// @file itemset.h
/// @brief The itemset type and their associated methods.
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

#pragma once

#include <ranges>
#include <vector>
#include <functional>

namespace fim {
    // Forward declaration
    struct database_t;

    // The prefix type: Represents a single item.
    using item_t = unsigned long;
    using item_compare_t = std::function<bool(const item_t &, const item_t &)>;

    // Default comparison function for items.
    auto default_item_compare(const item_t &i, const item_t &j) -> bool;

    // The suffix type: Represents a set of items (used for frequent itemsets).
    struct itemset_t : std::vector<item_t> {
        using std::vector<item_t>::vector;

        /// @brief Constructs an itemset from a single item.
        /// @param item The item to be added to the itemset.
        explicit itemset_t(const item_t &item);

        /// @brief Constructs an itemset from a list of items.
        /// @param items The list of items to be added to the itemset.
        itemset_t(std::initializer_list<item_t> items);

        /// @brief Adds an item to the itemset.
        /// @param item The item to be added.
        /// @return A reference to the updated itemset.
        auto add(const item_t &item) -> itemset_t &;

        /// @brief Checks if the current itemset is a subset of another itemset.
        /// @param superset The itemset to check against.
        /// @return True if the current itemset is a subset of the superset, false otherwise.
        [[nodiscard]] auto is_subset(const itemset_t &superset) const -> bool;

        /// @brief Checks if the current itemset is a subset of another itemset using a custom comparison function.
        /// @param superset The itemset to check against.
        /// @param comp The comparison function to use.
        /// @return True if the current itemset is a subset of the superset, false otherwise.
        [[nodiscard]] auto is_subset(const itemset_t &superset, const item_compare_t &comp) const -> bool;

        /// @brief Computes the union of the current itemset with another itemset.
        /// @param y The itemset to union with.
        /// @return A new itemset representing the union of the two itemsets.
        [[nodiscard]] auto set_union(const itemset_t &y) const -> itemset_t;

        /// @brief Computes the difference of the current itemset and another itemset
        /// (elements in the current itemset but not in the other).
        /// @param y The itemset to subtract.
        /// @return A new itemset representing the difference.
        [[nodiscard]] auto set_difference(const itemset_t &y) const -> itemset_t;

        /// @brief Checks if the current itemset contains a specific item.
        /// @param item The item to check.
        /// @return True if the item is in the itemset, false otherwise.
        [[nodiscard]] auto contains(const item_t &item) const -> bool;

        /// @brief Sorts the items in the itemset.
        /// @param compare The comparison function to use for sorting.
        /// @return A reference to the sorted itemset.
        auto sort_itemset(const item_compare_t &compare) -> itemset_t &;

        /// @brief Sorts the items in the itemset and returns a new sorted itemset.
        /// @param compare The comparison function to use for sorting.
        /// @return A new itemset with sorted items.
        [[nodiscard]] auto sort_itemset(const item_compare_t &compare) const -> itemset_t;
    };

    /// @brief Compares two itemsets lexicographically using a custom comparison function.
    /// @param x The first itemset.
    /// @param y The second itemset.
    /// @param comp The comparison function to use (default is `default_item_compare`).
    /// @return True if `x` is lexicographically smaller than `y`, false otherwise.
    auto lexicographical_compare(
        const itemset_t &x,
        const itemset_t &y,
        const item_compare_t &comp = default_item_compare) -> bool;

    // Collection of prefix sets: A vector of itemsets.
    struct itemsets_t : std::vector<itemset_t> {
        using std::vector<itemset_t>::vector;

        /// @brief Constructs a collection of itemsets from an existing vector of itemsets.
        /// @param itemsets A vector of itemsets to initialize the collection.
        explicit itemsets_t(const std::vector<itemset_t> &itemsets);

        /// @brief Adds an itemset to the collection.
        /// @param itemset The itemset to be added.
        auto add(const itemset_t &itemset) -> void;

        /// @brief Adds multiple itemsets to the collection.
        /// @param itemsets A collection of itemsets to be added.
        auto add(const itemsets_t &itemsets) -> void;

        /// @brief Checks if the collection contains a specific item.
        /// @param item The item to check.
        /// @return True if the collection contains the item, false otherwise.
        [[nodiscard]] auto contains(const item_t &item) const -> bool;

        /// @brief Checks if the collection contains a specific itemset.
        /// @param itemset The itemset to check.
        /// @return True if the collection contains the itemset, false otherwise.
        [[nodiscard]] auto contains(const itemset_t &itemset) const -> bool;

        /// @brief Sorts each itemset in the collection using the provided comparison function.
        /// @param compare The comparison function to use.
        /// @return The updated collection of itemsets.
        auto sort_each_itemset(const item_compare_t &compare) -> itemsets_t;
    };

    // Hash function for itemsets (used in hash-based containers like unordered_map).
    struct itemset_hash {
        auto operator()(const itemset_t &itemset) const -> std::size_t;
    };

    /// @brief Overloads the output stream operator to print an itemset to an output stream.
    /// @param os The output stream to write to.
    /// @param x The itemset to output.
    /// @return The output stream after writing the itemset.
    std::ostream &operator<<(std::ostream &os, const itemset_t &x);

    /// @brief Computes the union of two itemsets.
    /// @param x The first itemset.
    /// @param y The second itemset.
    /// @return A new itemset representing the union of the two itemsets.
    auto set_union(const itemset_t &x, const itemset_t &y) -> itemset_t;

    /// @brief Computes the difference of two itemsets (elements in `x` but not in `y`).
    /// @param x The first itemset.
    /// @param y The second itemset.
    /// @return A new itemset representing the difference.
    auto set_difference(const itemset_t &x, const itemset_t &y) -> itemset_t;

    /// @brief Checks if one itemset is a subset of another.
    /// @param x The first itemset.
    /// @param y The second itemset.
    /// @return True if `x` is a subset of `y`, false otherwise.
    auto is_subset(const itemset_t &x, const itemset_t &y) -> bool;
}
