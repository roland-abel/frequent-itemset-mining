/// @file apriori.cpp
/// @brief
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

#include <algorithm>
#include <ranges>
#include <sstream>
#include "apriori.h"

namespace rules::apriori {

    std::ostream &operator<<(std::ostream &os, const itemset_t &x) {
        os << "{";
        for (auto itr = x.begin(); itr != x.end(); ++itr) {
            os << *itr;
            if (std::next(itr) != x.end()) {
                os << ", ";
            }
        }
        os << "}";
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const rule_t &r) {
        const auto &[premise, conclusion] = r;
        os << premise << " -> " << conclusion;
        return os;
    }

    itemset_t set_union(const itemset_t &x, const itemset_t &y) {
        itemset_t d{};
        std::ranges::set_union(x, y, std::inserter(d, d.end()));
        return d;
    };

    itemset_t set_difference(const itemset_t &x, const itemset_t &y) {
        itemset_t d{};
        std::ranges::set_difference(x, y, std::inserter(d, d.end()));
        return d;
    };

    float get_confidence(const frequencies_t &frequencies, const itemset_t &x, const itemset_t &y) {
        return static_cast<float>(frequencies.at(hash_code(set_union(x, y)))) / static_cast<float>(frequencies.at(hash_code(x)));
    };

    auto apriori_gen(const itemsets_t &itemsets, size_t k) -> itemsets_t {
        // check whether the k-2 first items of x and y are match
        auto first_items_match = [&](const itemset_t &x, const itemset_t &y) -> std::pair<bool, itemset_t> {
            auto x_iter = x.begin();
            auto y_iter = y.begin();

            for (auto i = 0; i < k - 2; i++, x_iter++, y_iter++) {
                if (*x_iter != *y_iter) {
                    // the first k-2 items are not match
                    return std::make_pair(false, itemset_t{});
                }
            }

            // create new candidate (k+1)-itemset
            itemset_t z{};
            std::copy(x.begin(), std::prev(x_iter, -1), std::inserter(z, z.begin()));

            z.insert(*x_iter);
            z.insert(*y_iter);

            return std::make_pair(true, z);
        };

        auto candidates = itemsets_t{};

        // find all pairs of itemsets where the first k − 1 items are identical (self-join).
        for (auto x = itemsets.begin(); x != itemsets.end(); x++) {
            for (auto y = std::next(x); y != itemsets.end(); y++) {
                auto [match, z] = first_items_match(*x, *y);
                if (!match) {
                    continue;
                }
                candidates.insert(z);
            }
        }
        return candidates;
    }

    auto apriori_algorithm(const transactions_t &transactions, size_t min_support) -> std::pair<itemsets_t, frequencies_t> {
        auto frequencies = frequencies_t{};

        auto has_support = [&](const auto &itemset, const auto &frequencies) {
            return static_cast<float>(frequencies.at(hash_code(itemset))) >= min_support;
        };

        auto prune = [&](itemsets_t &itemsets) -> itemsets_t {
            for (const auto &t: transactions) {
                for (const auto &x: itemsets) {
                    if (std::ranges::includes(t, x)) {
                        frequencies[hash_code(x)]++;
                    }
                }
            }

            std::erase_if(itemsets, [&](const auto &x) { return !has_support(x, frequencies); });
            std::erase_if(frequencies, [&](const auto &f) { return f.second < min_support; });

            return itemsets;
        };

        auto find_frequent_one_itemsets = [&]() {
            auto itemsets = itemsets_t{};
            for (const auto &t: transactions) {
                for (const auto item: t) {
                    const auto x = itemset_t{item};
                    itemsets.insert(x);
                }
            }

            prune(itemsets);
            return itemsets;
        };

        auto frequent_itemsets = itemsets_t{};

        // find all 1-element itemsets
        auto itemsets = find_frequent_one_itemsets();
        frequent_itemsets.insert_range(itemsets);

        for (auto k = 2; !itemsets.empty(); k++) {
            // create k-itemsets from the previous (k-1)-itemsets
            itemsets = apriori_gen(itemsets, k);

            // remove all itemsets with low support
            prune(itemsets);

            // insert frequent itemsets
            frequent_itemsets.insert_range(itemsets);
        }

        return std::make_pair(frequent_itemsets, frequencies);
    }

    auto generate_rules(
            const itemset_t &z,
            const frequencies_t &frequencies,
            float min_confidence) -> rules_t {

        // Gets the confidence of the rule of the form "x -> y" with and x=z\y.
        auto confidence = [&](const auto &x, const auto &y) -> float {
            return static_cast<float>(frequencies.at(hash_code(set_union(z, y)))) / static_cast<float>(frequencies.at(hash_code(x)));
        };

        auto get_conclusions = [](const rules_t &rules) -> itemsets_t {
            itemsets_t conclusions{};
            for (const auto &[_, conclusion]: rules) {
                conclusions.insert(conclusion);
            }
            return conclusions;
        };

        /// For all conclusions y generate all rules of the form "x -> y" with and x=z\y.
        auto generate_rules = [&](itemsets_t &conclusions) -> rules_t {
            auto rules = rules_t{};

            for (auto it = conclusions.begin(); it != conclusions.end();) {
                const auto y = *it;
                const auto &x = set_difference(z, y);

                if (x.empty()) {
                    return rules;
                }

                // confidence of "z\y -> y"
                if (confidence(x, y) >= min_confidence) {
                    // insert rule "z\y -> y"
                    rules.insert(rule_t(x, y));
                    ++it;
                } else {
                    it = conclusions.erase(it);
                }
            }
            return rules;
        };

        /// For itemset z generate all rules of the form "x -> y" with |y|=1 and x=z\y.
        auto create_initial_rules = [&]() -> rules_t {
            itemsets_t conclusions{};
            for (const auto &item: z) {
                const auto conclution = itemsets_t{itemset_t{item}};
                conclusions.insert(conclution.begin(), conclution.end());
            }
            return generate_rules(conclusions);
        };

        auto associated_rules = rules_t{};

        auto rules = create_initial_rules();
        associated_rules.insert(rules.begin(), rules.end());

        auto conclusions = get_conclusions(rules);

        for (auto k = 2; !conclusions.empty(); k++) {
            // Creates k-itemsets from the previous (k-1)-itemsets
            conclusions = apriori_gen(conclusions, k);

            // Creates rules of the form "z\y -> y" for each conclution y.
            rules = generate_rules(conclusions);
            associated_rules.insert(rules.begin(), rules.end());
        }
        return associated_rules;
    }

    auto generate_rules(
            const itemsets_t &itemsets,
            const frequencies_t &frequencies,
            float min_confidence) -> rules_t {

        auto association_rules = rules_t{};
        for (const auto &z: itemsets | std::views::filter([](const auto &x) { return x.size() > 1; })) {
            const auto rules = generate_rules(z, frequencies, min_confidence);
            association_rules.insert(rules.begin(), rules.end());
        }

        return association_rules;
    }
}
