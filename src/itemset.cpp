/// @file itemset.cpp
/// @brief
///
/// @author Roland Abel
/// @date September 21, 2004
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

#include <ostream>
#include <functional>
#include <algorithm>
#include <utility>
#include "itemset.h"

namespace fim {

    auto default_item_comparer(const item_t &i, const item_t &j) -> bool {
        return i < j;
    }

    itemset_t::itemset_t(const item_t &item)
            : std::vector<item_t>() {
        emplace_back(item);
    }

    itemset_t::itemset_t(std::initializer_list<item_t> items)
            : std::vector<item_t>(items) {
    }

    auto itemset_t::is_subset(const itemset_t &superset) const -> bool {
        return std::ranges::includes(superset, *this);
    }

    auto itemset_t::set_union(const itemset_t &y) const -> itemset_t {
        itemset_t z{};
        std::ranges::set_union(*this, y, std::back_inserter(z));

        return z;
    }

    auto itemset_t::set_difference(const itemset_t &y) const -> itemset_t {
        itemset_t z{};
        std::ranges::set_difference(*this, y, std::back_inserter(z));

        return z;
    }

    auto itemset_t::contains(const item_t &item) const -> bool {
        return std::ranges::contains(*this, item);
    }

    auto itemset_t::sort_itemset(const item_compare_t &comp) -> itemset_t & {
        std::sort(begin(), end(), comp);
        return *this;
    }

    auto itemset_t::add(const item_t &item) -> itemset_t & {
        emplace_back(item);
        return *this;
    }

    auto itemset_hash::operator()(const itemset_t &itemset) const -> std::size_t {
        std::size_t seed = 0;
        for (const auto &item: itemset) {
            seed ^= std::hash<item_t>{}(item) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }

    auto itemsets_t::add(const itemset_t &itemset) -> void {
        emplace_back(itemset);
    }

    auto itemsets_t::add(const itemsets_t &itemsets) -> void {
        for (const itemset_t &x: itemsets) {
            add(x);
        }
    }

    auto itemsets_t::contains(const item_t &item) const -> bool {
        return std::ranges::contains(*this, itemset_t{item});
    }

    auto itemsets_t::contains(const itemset_t &itemset) const -> bool {
        return std::ranges::contains(*this, itemset);
    }

    auto itemsets_t::sort_each_itemset(const item_compare_t &comp) -> itemsets_t {
        for (auto &itemset: *this) {
            itemset.sort_itemset(comp);
        }
        return *this;
    }

    auto is_subset(const itemset_t &x, const itemset_t &y) -> bool {
        return x.is_subset(y);
    }

    auto set_union(const itemset_t &x, const itemset_t &y) -> itemset_t {
        return x.set_union(y);
    };

    auto set_difference(const itemset_t &x, const itemset_t &y) -> itemset_t {
        return x.set_difference(y);
    };

    std::ostream &operator<<(std::ostream &os, const itemset_t &x) {
        os << "{";
        for (auto itr = x.begin(); itr != x.end(); ++itr) {
            os << (char) *itr;
            if (std::next(itr) != x.end()) {
                os << ", ";
            }
        }
        os << "}";
        return os;
    }

    auto lexicographical_compare(const itemset_t &x, const itemset_t &y, const item_compare_t &comp) -> bool {
        auto it_x = x.begin();
        auto it_y = y.begin();

        for (; it_x != x.end() && it_y != y.end(); ++it_x, ++it_y) {
            if (comp(*it_x, *it_y))
                return true;

            if (comp(*it_y, *it_x))
                return false;
        }
        return std::distance(it_x, x.end()) > std::distance(it_y, y.end());
    }

    auto item_count_t::get_item_count(const database_t &database) -> item_count_t {
        item_count_t counts{};
        for (const auto &item: database | std::views::join) {
            ++counts[item];
        }
        return std::move(counts);
    }

    auto item_count_t::get_frequent_items(size_t min_support) const -> itemset_t {
        auto is_frequent = [=](const auto &kv) { return kv.second >= min_support; };
        auto get_item = [](const auto &kv) { return kv.first; };

        auto items = *this
                     | std::views::filter(is_frequent)
                     | std::views::transform(get_item)
                     | std::ranges::to<itemset_t>();

        std::ranges::sort(items, [&](const item_t &x, const item_t &y) {
            return at(x) > at(y);
        });

        return items;
    }

    auto item_count_t::get_item_comparer() const -> item_compare_t {
        return [&](const item_t &i, const item_t &j) -> bool {
            const auto weight_i = at(i);
            const auto weight_j = at(j);

            return (weight_i != weight_j) ? (weight_i < weight_j) : (i < j);
        };
    }

    database_t::database_t(itemsets_t itemsets)
            : std::vector<itemset_t>(std::move(itemsets)) {
    }

    auto database_t::sort_lexicographically(const item_compare_t &comp) -> database_t {
        std::ranges::sort(*this, [&](const itemset_t &x, const itemset_t &y) {
            return lexicographical_compare(x, y, comp);
        });
        return *this;
    }

    auto database_t::sort_database(const item_compare_t &comp) -> database_t {
        for (auto &trans: *this) {
            trans.sort_itemset(comp);
        }
        return sort_lexicographically();
    }
}
