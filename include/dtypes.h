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

namespace rules {

    // The item type.
    using item_t = unsigned long;

    // The itemset type.
    using itemset_t = std::set<item_t>;

    // Collection of itemsets.
    using itemsets_t = std::set<itemset_t>;

    // The transaction database type.
    using transactions_t = std::vector<itemset_t>;

    // The associate rule type which contains a premise and a conclusion (premise -> conclusion).
    using rule_t = std::tuple<itemset_t, itemset_t>;

    // Collection of rules.
    using rules_t = std::set<rule_t>;

    // Type for counting frequents of itemsets.
    using frequencies_t = std::unordered_map<size_t, size_t>;
}