/// @file apriori.cpp
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

#include "apriori.h"

namespace rules {
    namespace rng = std::ranges;
    using namespace std::views;

    auto set_intersection(const itemset_t &x, const itemset_t &y) -> itemset_t {
        itemset_t items{};
        rng::set_intersection(x, y, std::inserter(items, items.begin()));
        return items;
    }

    auto set_union(const itemset_t &x, const itemset_t &y) -> itemset_t {
        itemset_t items{};
        rng::set_union(x, y, std::inserter(items, items.begin()));
        return items;
    }

    auto remove_from_itemset(const itemset_t &x, const item_t &item) -> itemset_t {
        return x | filter([&](const auto &i) { return i != item; }) | rng::to<itemset_t>();
    }

    auto add_to_itemset(const itemset_t &x, const item_t &item) -> itemset_t {
        auto y = x;
        y.insert(item);
        return y;
    }

    auto is_subset(const itemset_t &x, const itemset_t &y) -> bool {
        return rng::includes(y, x);
    }

    auto get_candidates(const transactions_t &transactions) -> itemset_collection_t {
        auto candidates = itemset_collection_t{};

        for (const auto &x: transactions) {
            for (const auto item: x) {
                candidates.insert(itemset_t{item});
            }
        }
        return candidates;
    }

    auto get_candidates(const itemset_collection_t &collection, const size_t k) -> itemset_collection_t {
        itemset_collection_t candidates{};

        auto has_subset = [k](const itemset_t &x, const itemset_t &y) -> bool {
            // check whether the itemset y is a (k-2)-element subset of the itemset x
            return std::equal(x.cbegin(), std::next(x.cbegin(), static_cast<signed>(k) - 2), y.cbegin());
        };

        // self-join
        for (auto x_iter = collection.begin(); x_iter != collection.end(); ++x_iter) {
            for (auto y_iter = next(x_iter); y_iter != collection.end(); ++y_iter) {
                const auto &x = *x_iter;
                if (const auto &y = *y_iter; has_subset(x, y)) {
                    if (const auto u = set_union(x, y); u.size() == k)
                        candidates.insert(u);
                }
            }
        }
        return std::move(candidates);
    }

    auto prune(
            const itemset_collection_t &candidates,
            const float min_support,
            support_counter_t &global_support_counter,
            const transactions_t &transactions) -> itemset_collection_t {
        support_counter_t counter{};

        for (const auto &t: transactions) {
            for (const auto &y: candidates) {
                if (is_subset(y, t)) {
                    global_support_counter[y] += 1;
                    counter[y] += 1;
                }
            }
        }

        auto itemsets = itemset_collection_t{};
        const auto total = transactions.size();

        for (auto const &[x, count]: counter) {
            const auto support = static_cast<float>(count) / static_cast<float>(total);
            if (support >= min_support) {
                itemsets.insert(x);
            }
        }

        return std::move(itemsets);
    }

    auto get_rule_candidates(
            const itemset_t &itemset,
            const support_counter_t &support_counter,
            const float min_confidence) -> association_rules_t {

        auto candidates = association_rules_t{};
        for (const auto &item: itemset) {
            const auto &x = remove_from_itemset(itemset, item);
            const auto &y = add_to_itemset(itemset_t{}, item);
            const auto &u = set_union(x, y);

            const float conf = static_cast<float>(support_counter.at(u)) / static_cast<float>(support_counter.at(x));
            if (conf >= min_confidence) {
                candidates.insert(associated_rule_t(x, y));
            }
        }
        return candidates;
    }

    auto get_association_rules(
            const association_rules_t &rules,
            const support_counter_t &support_counter,
            const float min_conf,
            const size_t k) -> association_rules_t {
        auto candidates = association_rules_t{};

        // self-join
        for (auto r_iter = rules.begin(); r_iter != rules.end(); ++r_iter) {
            for (auto s_iter = next(r_iter); s_iter != rules.end(); ++s_iter) {
                const auto &[x1, y1] = *r_iter;
                const auto &[x2, y2] = *s_iter;

                const auto u = set_union(y1, y2);
                const auto i = set_intersection(x1, x2);

                if (u.size() != k) {
                    continue;
                }

                const float conf = static_cast<float>(support_counter.at(set_union(u, i))) / static_cast<float>(
                        support_counter.at(i));
                if (conf >= min_conf) {
                    // Add rule: "X1 intersection X2 => Y1 union Y2"
                    candidates.insert(std::make_tuple(
                            set_intersection(x1, x2),
                            set_union(y1, y2)));
                }
            }
        }
        return std::move(candidates);
    }

    auto get_association_rules(
            const itemset_t &itemset,
            const support_counter_t &support_counter,
            const float min_conf) -> association_rules_t {
        auto all_rules = association_rules_t{};
        auto rules = get_rule_candidates(itemset, support_counter, min_conf);

        all_rules.insert(rules.begin(), rules.end());

        auto k = 2;
        while (!rules.empty()) {
            auto next_rules = get_association_rules(rules, support_counter, min_conf, k);
            all_rules.insert(next_rules.begin(), next_rules.end());

            rules = std::move(next_rules);
            k += 1;
        }
        return all_rules;
    }

    auto get_association_rules(
            const frequent_itemsets_t &frequent_itemsets,
            const support_counter_t &support_counter,
            const float min_conf) -> association_rules_t {
        auto rules = association_rules_t{};

        for (const auto &itemset: frequent_itemsets | filter([](const auto &x) { return x.size() > 1; })) {
            const auto r = get_association_rules(itemset, support_counter, min_conf);
            rules.insert(r.begin(), r.end());
        }
        return rules;
    }

    auto get_frequent_itemsets(
            const transactions_t &transactions,
            const float min_support) -> frequent_itemsets_result_t {
        auto support_counter = support_counter_t{};
        auto frequent_itemsets = frequent_itemsets_t{};

        auto candidates = get_candidates(transactions);
        auto itemsets = prune(
                candidates,
                min_support,
                support_counter,
                transactions);

        auto k = 2;
        while (!itemsets.empty()) {
            frequent_itemsets.insert(
                    frequent_itemsets.cend(),
                    itemsets.cbegin(),
                    itemsets.cend());

            candidates = get_candidates(itemsets, k);
            itemsets = prune(candidates, min_support, support_counter, transactions);

            k += 1;
        }
        return std::make_tuple(frequent_itemsets, support_counter);
    }

    auto apriori(
            const transactions_t &transactions,
            const float min_support,
            const float min_conf) -> apriori_result_t {

        // first determine all frequent item sets
        const auto &[frequent_itemsets, support_counter] = get_frequent_itemsets(transactions, min_support);

        // determine all rules that can be derived from the frequent item sets
        const auto rules = get_association_rules(frequent_itemsets, support_counter, min_conf);
        return apriori_result_t{rules, frequent_itemsets, support_counter};
    }
}
