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

#include <vector>
#include <unordered_map>

namespace fim::itemset {

    // The item type.
    using item_t = unsigned long;

    // The itemset type.
    using itemset_t = std::vector<item_t>;

    // Collection of itemsets.
    using itemsets_t = std::vector<itemset_t>;

    // The transaction database type.
    using database_t = std::vector<itemset_t>;

    // Hash function for an itemset
    struct itemset_hash {
        std::size_t operator()(const itemset_t &itemset) const;
    };

    // Support counting
    using support_count_t = std::unordered_map<itemset_t, size_t, itemset_hash>;

    // Item counting
    using item_count_t = std::unordered_map<item_t, size_t>;
}