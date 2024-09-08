/// @file apriori.h
/// @brief
/// @see: https://www.macs.hw.ac.uk/~dwcorne/Teaching/agrawal94fast.pdf
///
/// @author Roland Abel
/// @date July 07, 2024
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
#include <iostream>
#include "dtypes.h"

namespace rules::apriori {

    /// Hash function for an itemset.
    /// @param itemset The given itemset.
    /// @return The hash code for the given x.
    std::size_t hash_code(const itemset_t &itemset);

    /// Overloads the output stream operator to output an itemset to an ostream.
    /// @param os The output stream to write to.
    /// @param x The itemset to output.
    /// @return The output stream after writing the itemset.
    std::ostream &operator<<(std::ostream &os, const itemset_t &x);

    /// Overloads the output stream operator to output an rule to an ostream.
    /// @param os The output stream to write to.
    /// @param r The rule to output.
    /// @return The output stream after writing the rule.
    std::ostream &operator<<(std::ostream &os, const rule_t &r);

    /// Computes the union of two itemsets.
    /// @param x The first itemset.
    /// @param y The second itemset.
    /// @return The union of the two itemsets.
    itemset_t set_union(const itemset_t &x, const itemset_t &y);

    /// Computes the difference of two itemsets (elements in x but not in y).
    /// @param x The first itemset.
    /// @param y The second itemset.
    /// @return The difference of the two sets.
    itemset_t set_difference(const itemset_t &x, const itemset_t &y);

    /// Computes the support for a given itemset.
    /// @param frequencies The frequencies of itemsets.
    /// @param x The itemset.
    /// @param num_transactions Number of transactions.
    /// @return The support of x.
    float get_support(const frequencies_t &frequencies, const itemset_t &x, size_t num_transactions);

    /// Computes the confidence level of rule x -> y given a set of frequencies.
    /// @param frequencies The frequencies of itemsets.
    /// @param x The premise of the rule.
    /// @param y The conclusion of the rule.
    /// @return The confidence level of the rule x -> y.
    float get_confidence(const frequencies_t &frequencies, const itemset_t &x, const itemset_t &y);

    /// @brief Creates itemsets of the length k from the given itemsets of length k-1 by self-joining.
    /// @param k The size of the returned itemsets.
    /// @return The k-element frequent itemsets.
    auto apriori_gen(const itemsets_t &itemsets, size_t k) -> itemsets_t;

    /// @brief Creates the collection of all frequent itemsets by apply the Apriori principle:
    /// If an itemset is frequent, then all of its subsets must also be frequent.
    /// @param transactions The itemsets database.
    /// @param min_support The minimum support threshold for frequent itemsets.
    /// @return Pair of the frequent itemsets with regard to min_support and their frequents.
    auto apriori_algorithm(const transactions_t &itemsets, float min_support) -> std::pair<itemsets_t, frequencies_t>;

    /// @brief Creates association rules based on one frequent itemset.
    /// @param z The frequent itemset for which the rules are generated.
    /// @param frequencies The frequencies of the frequent itemsets.
    /// @param min_confidence The minimum confidence threshold for the rules.
    /// @return rules_t The association rules.
    auto generate_rules(
            const itemset_t &z,
            const frequencies_t &frequencies,
            float min_confidence) -> rules_t;

    /// @brief Creates association rules based on the collection of frequent itemsets.
    /// @param itemsets The collection of frequent itemsets.
    /// @param frequencies The frequencies of the frequent itemsets.
    /// @param min_confidence The minimum confidence threshold for the rules.
    /// @return rules_t The association rules.
    auto generate_rules(
            const itemsets_t &itemsets,
            const frequencies_t &frequencies,
            float min_confidence) -> rules_t;
}
