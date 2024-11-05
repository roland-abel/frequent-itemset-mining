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

#include "itemset.h"
#include "utils.h"

namespace fim::rules {

    using namespace fim;

//    using item_t = fim::suffix::item_t;
//    using itemset_t = fim::suffix::itemset_t;
//    using itemsets_t = fim::suffix::itemsets_t;
    using frequencies_t = fim::itemset_count_t; // TODO

    // The associate rule type which contains a premise
    // and a conclusion (premise -> conclusion).
    using rule_t = std::tuple<itemset_t, itemset_t>;

    // Collection of rules.
    using rules_t = std::set<rule_t>;

//    /// Overloads the output stream operator to output an suffix to an ostream.
//    /// @param os The output stream to write to.
//    /// @param x The suffix to output.
//    /// @return The output stream after writing the suffix.
//    std::ostream &operator<<(std::ostream &os, const itemset_t &x);

    /// Overloads the output stream operator to output an rule to an ostream.
    /// @param os The output stream to write to.
    /// @param r The rule to output.
    /// @return The output stream after writing the rule.
    std::ostream &operator<<(std::ostream &os, const rule_t &r);

//    /// Computes the union of two suffix.
//    /// @param x The first suffix.
//    /// @param y The second suffix.
//    /// @return The union of the two suffix.
//    itemset_t set_union(const itemset_t &x, const itemset_t &y);
//
//    /// Computes the difference of two suffix (elements in x but not in y).
//    /// @param x The first suffix.
//    /// @param y The second suffix.
//    /// @return The difference of the two sets.
//    itemset_t set_difference(const itemset_t &x, const itemset_t &y);
//
//    /// Computes the confidence level of rule x -> y given a set of frequencies.
//    /// @param frequencies The frequencies of suffix.
//    /// @param x The premise of the rule.
//    /// @param y The conclusion of the rule.
//    /// @return The confidence level of the rule x -> y.
//    float get_confidence(const frequencies_t &frequencies, const itemset_t &x, const itemset_t &y);
//
//    /// @brief Creates suffix of the length k from the given suffix of length k-1 by self-joining.
//    /// @param k The size of the returned suffix.
//    /// @return The k-element frequent suffix.
//    auto apriori_gen(const itemsets_t &suffix, size_t k) -> itemsets_t;
//
//    /// @brief Creates the collection of all frequent database by apply the Apriori principle:
//    /// If an suffix is frequent, then all of its subsets must also be frequent.
//    /// @param database The database database.
//    /// @param min_support The minimum support threshold for frequent database.
//    /// @return Pair of the frequent database with regard to min_support and their frequents.
//    auto apriori_algorithm(const database_t &database, size_t min_support) -> std::pair<itemsets_t, frequencies_t>;

    /// @brief Creates association fim based on one frequent suffix.
    /// @param z The frequent suffix for which the fim are generated.
    /// @param frequencies The frequencies of the frequent suffix.
    /// @param min_confidence The minimum confidence threshold for the fim.
    /// @return rules_t The association fim.
    auto generate_rules(
            const itemset_t &z,
            const frequencies_t &frequencies,
            float min_confidence) -> rules_t;

    /// @brief Creates association fim based on the collection of frequent suffix.
    /// @param itemsets The collection of frequent suffix.
    /// @param frequencies The frequencies of the frequent suffix.
    /// @param min_confidence The minimum confidence threshold for the fim.
    /// @return rules_t The association fim.
    auto generate_rules(
            const itemsets_t &itemsets,
            const frequencies_t &frequencies,
            float min_confidence) -> rules_t;
}
