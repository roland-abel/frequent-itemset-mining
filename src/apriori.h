/// @file apriori.h
/// @brief
///
/// @author Roland Abel
/// @date December 8, 2023
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

#include <set>
#include <unordered_map>
#include <algorithm>
#include <ranges>
#include <vector>
#include <map>

#include "config.h"

namespace rules {

    // An item.
    using item_t = unsigned long;

    // An itemset.
    using itemset_t = std::set<item_t>;

    // Collection of itemsets.
    using itemset_collection_t = std::set<itemset_t>;

    // Transaction database type.
    using transactions_t = std::multiset<itemset_t>;

    // Frequency of the itemsets.
    using support_counter_t = std::map<itemset_t, size_t>;

    // Frequent itemsets. The vector contains at index (k-1) the k-element frequent itemsets.
    using frequent_itemsets_t = std::vector<itemset_t>;

    // An associate rule containing the premise and the conclusion.
    // premise => conclusion
    using associated_rule_t = std::tuple<itemset_t, itemset_t>;

    // A vector of associate rules.
    using association_rules_t = std::set<associated_rule_t>;

    // Result type of the frequent itemset determination.
    using frequent_itemsets_result_t = std::tuple<frequent_itemsets_t, support_counter_t>;

    // The result type of the Apriori algorithm.
    using apriori_result_t = std::tuple<association_rules_t, frequent_itemsets_t, support_counter_t>;

    /// Calculates the intersection of two itemsets.
    /// @param x The first x.
    /// @param y The second x.
    /// @return The intersection of the itemsets x and y.
    auto set_intersection(const itemset_t &x, const itemset_t &y) -> itemset_t;

    /// Calculates the union of two itemsets.
    /// @param x The first itemset
    /// @param y The second itemset
    /// @return The union of itemsets x and y
    auto set_union(const itemset_t &x, const itemset_t &y) -> itemset_t;

    /// Removes an item form the given itemset.
    /// @param x The itemset.
    /// @param item The item to be removed.
    /// @return An itemset which contains all item from itemset x expect the given item.
    auto remove_from_itemset(const itemset_t &x, const item_t &item) -> itemset_t;

    /// Insert an item to the given itemset.
    /// @param x The itemset.
    /// @param item The item to be to inserted.
    /// @return The itemset which contains the new item.
    auto add_to_itemset(const itemset_t &x, const item_t &item) -> itemset_t;

    /// Gets an value indicating whether itemset x contains itemset y as a subset.
    /// @param x The left hand side itemset (super set).
    /// @param y The right hand side itemset (sub set).
    /// @return True if itemset x contains itemset y as a subset; false otherwise.
    auto is_subset(const itemset_t &x, const itemset_t &y) -> bool;

    /// Gets the 1-element itemsets from the transactions.
    /// @param transactions The transaction data base.
    /// @return The 1-element itemsets.
    auto get_candidates(const transactions_t &transactions) -> itemset_collection_t;

    /// @brief Gets candidate itemsets of a specified length from a given collection.
    /// @param collection The collection from which candidate itemsets are to be retrieved.
    /// @param k Length of the candidate itemsets to be retrieved.
    /// @return itemset_collection_t Candidate itemsets of the specified length.
    auto get_candidates(const itemset_collection_t &collection, size_t k) -> itemset_collection_t;

    /// Gets frequent itemset from the given candidate itemsets.
    /// @param candidates The candidate itemset collection.
    /// @param min_support The minimal support.
    /// @param global_support_counter The support counter.
    /// @param transactions The transaction data base.
    /// @return The frequent itemset.
    auto prune(
            const itemset_collection_t &candidates,
            float min_support,
            support_counter_t &global_support_counter,
            const transactions_t &transactions) -> itemset_collection_t;

    /// @brief Gets candidate association rules for a given itemset based on support values and minimum confidence.
    /// @param itemset The itemset for which candidate association rules are to be retrieved.
    /// @param support_counter The support counter containing support values for itemsets.
    /// @param min_confidence The minimum confidence threshold for association rules.
    /// @return association_rules_t Candidate association rules satisfying the given conditions.
    auto get_rule_candidates(
            const itemset_t &itemset,
            const support_counter_t &support_counter,
            float min_confidence) -> association_rules_t;

    /// @brief Retrieves association rules based on the given rules, support counter, minimum confidence, and length.
    /// @param rules The initial set of association rules.
    /// @param support_counter The support counter containing support values for itemsets.
    /// @param min_conf The minimum confidence threshold for association rules.
    /// @param k Length of the association rules.
    /// @return association_rules_t The association rules satisfying the given conditions.
    auto get_association_rules(
            const association_rules_t &rules,
            const support_counter_t &support_counter,
            float min_conf,
            size_t k) -> association_rules_t;

    /// @brief Gets association rules for a given itemset based on support values and minimum confidence.
    /// @param itemset The itemset for which association rules are to be retrieved.
    /// @param support_counter The support counter containing support values for itemsets.
    /// @param min_conf The minimum confidence threshold for association rules.
    /// @return association_rules_t The association rules satisfying the given conditions.
    auto get_association_rules(
            const itemset_t &itemset,
            const support_counter_t &support_counter,
            float min_conf) -> association_rules_t;

    /// @brief Retrieves association rules based on frequent itemsets, support counter, and minimum confidence.
    /// @param frequent_itemsets The frequent itemsets from which association rules are to be generated.
    /// @param support_counter The support counter containing support values for itemsets.
    /// @param min_conf The minimum confidence threshold for association rules.
    /// @return association_rules_t The association rules satisfying the given conditions.
    auto get_association_rules(
            const frequent_itemsets_t &frequent_itemsets,
            const support_counter_t &support_counter,
            float min_conf) -> association_rules_t;

    /// @brief Computes frequent itemsets and their support values from transactions based on a minimum support threshold.
    /// @param transactions The transactions from which frequent itemsets are to be extracted.
    /// @param min_support The minimum support threshold for itemsets to be considered frequent.
    /// @return std::tuple<frequent_itemsets_t, support_counter_t> A tuple containing frequent itemsets and their support counter.
    auto get_frequent_itemsets(const transactions_t &transactions, float min_support) -> frequent_itemsets_result_t;

    /// @brief Executes the Apriori algorithm to find association rules.
    /// @param transactions The transactions database.
    /// @param min_support The minimum support threshold for frequent itemsets.
    /// @param min_conf The minimum confidence threshold for association rules.
    /// @return apriori_result_t Result of the Apriori algorithm.
    auto apriori(const transactions_t &transactions, float min_support, float min_conf) -> apriori_result_t;
}
