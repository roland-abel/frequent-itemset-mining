/// @file apriori_tests.cpp
/// @brief Unit test for the Apriori algorithm.
///
/// @author Roland Abel
/// @date December 8, 2023
///
/// Copyright (c) 2023 Roland Abel
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

#include <gtest/gtest.h>
#include "apriori.h"

using namespace rules;

class AprioriTests : public ::testing::Test {
protected:

    enum Items {
        Milk, Bread, Cheese, Butter, Coffee, Sugar, Flour
    };

    static inline rules::itemset_t all_items = {
            Milk, Bread, Cheese, Butter, Coffee, Sugar, Flour
    };

    static inline rules::transactions_t get_transactions() {
        return {
                {Milk,   Cheese, Butter, Bread,  Sugar,  Flour},
                {Cheese, Butter, Bread,  Coffee, Sugar,  Flour},
                {Milk,   Butter, Coffee, Sugar,  Flour},
                {Milk,   Butter},
                {Milk,   Butter, Coffee},
                {Milk,   Flour},
                {Milk,   Cheese, Butter, Bread,  Coffee, Sugar, Flour},
                {},
                {Milk,   Cheese, Butter, Sugar},
                {Milk,   Cheese, Bread,  Coffee, Sugar,  Flour}
        };
    }
};

TEST_F(AprioriTests, IsSubsetTest) {
    EXPECT_TRUE(is_subset({}, {}));
    EXPECT_TRUE(is_subset({}, {Bread}));
    EXPECT_TRUE(is_subset({Sugar, Milk, Bread}, {Milk, Bread, Sugar}));
    EXPECT_TRUE(is_subset({Milk, Sugar}, {Milk, Bread, Sugar}));

    EXPECT_FALSE(is_subset({Milk}, {}));
    EXPECT_FALSE(is_subset({Milk, Coffee}, {Milk, Bread, Sugar}));
}

TEST_F(AprioriTests, SetIntersectionTest) {
    EXPECT_EQ(set_intersection({Milk, Coffee, Bread}, {}), itemset_t{});
    EXPECT_EQ(set_intersection({Milk, Coffee, Bread}, {Coffee}), itemset_t{Coffee});
    EXPECT_EQ(set_intersection({Coffee, Bread, Sugar}, {Coffee, Sugar, Milk}), itemset_t({Coffee, Sugar}));
}

TEST_F(AprioriTests, SetUnionTest) {
    EXPECT_EQ(set_union({Milk, Coffee, Bread}, {}), itemset_t({Milk, Coffee, Bread}));
    EXPECT_EQ(set_union({Milk, Coffee, Bread}, {Coffee}), itemset_t({Milk, Coffee, Bread}));
    EXPECT_EQ(set_union({Coffee, Bread, Sugar}, {Coffee, Sugar, Milk}), itemset_t({Coffee, Bread, Sugar, Sugar, Milk}));
}

TEST_F(AprioriTests, NumberOfTransactionsTest) {
    EXPECT_EQ(get_transactions().size(), 10);
}

TEST_F(AprioriTests, GetOneElementItemsetsTest) {
    const auto &transactions = get_transactions();
    const auto itemset_collection = get_candidates(transactions);

    EXPECT_EQ(itemset_collection.size(), 7);
}

TEST_F(AprioriTests, GetPruneTest) {
    const auto &transactions = get_transactions();
    const auto candidate_itemsets = get_candidates(transactions);

    auto support_counter = support_counter_t{};
    const auto min_support = 0.4f;

    auto frequent_itemsets = prune(
            candidate_itemsets,
            min_support,
            support_counter,
            transactions);

    EXPECT_EQ(frequent_itemsets.size(), 7);

    EXPECT_EQ(support_counter[{Milk}], 8);
    EXPECT_EQ(support_counter[{Bread}], 4);
    EXPECT_EQ(support_counter[{Cheese}], 5);
    EXPECT_EQ(support_counter[{Butter}], 7);
    EXPECT_EQ(support_counter[{Coffee}], 5);
    EXPECT_EQ(support_counter[{Sugar}], 6);
    EXPECT_EQ(support_counter[{Flour}], 6);
}

TEST_F(AprioriTests, GetCandidatesTest) {
    const auto &transactions = get_transactions();
    const auto itemsets = get_candidates(transactions);

    EXPECT_EQ(get_candidates(itemsets, 2).size(), 21);
}

TEST_F(AprioriTests, GetFrequentItemsetsTest) {
    auto get_count_by_size = [](const auto &itemsets, int k) {
        return std::ranges::count_if(itemsets.begin(), itemsets.end(), [k](const auto &x) { return x.size() == k; });
    };

    const auto &transactions = get_transactions();
    const auto min_support = 0.4f;

    const auto [frequent_itemsets, _] = get_frequent_itemsets(transactions, min_support);

    EXPECT_EQ(get_count_by_size(frequent_itemsets, 1), 7);
    EXPECT_EQ(get_count_by_size(frequent_itemsets, 2), 17);
    EXPECT_EQ(get_count_by_size(frequent_itemsets, 3), 10);
    EXPECT_EQ(get_count_by_size(frequent_itemsets, 4), 1);
}

TEST_F(AprioriTests, AssociationRulesFormItemsetTest) {
    const auto &transactions = get_transactions();

    const auto min_support = 0.4f;
    const auto min_conf = 0.9f;

    const itemset_t itemset = {Cheese, Bread, Flour, Sugar};

    const auto [frequent_itemsets, support_counter] = get_frequent_itemsets(transactions, min_support);
    const auto rules = get_association_rules(itemset, support_counter, min_conf);

    EXPECT_EQ(rules.size(), 9);

    EXPECT_TRUE(rules.contains({{Bread, Flour, Sugar}, {Cheese}}));
    EXPECT_TRUE(rules.contains({{Cheese, Flour, Sugar}, {Bread}}));
    EXPECT_TRUE(rules.contains({{Cheese, Bread, Sugar}, {Flour}}));
    EXPECT_TRUE(rules.contains({{Cheese, Bread, Flour}, {Sugar}}));

    EXPECT_TRUE(rules.contains({{Bread, Flour}, {Cheese, Sugar}}));
    EXPECT_TRUE(rules.contains({{Bread, Cheese}, {Flour, Sugar}}));
    EXPECT_TRUE(rules.contains({{Bread, Sugar}, {Cheese, Flour}}));
    EXPECT_TRUE(rules.contains({{Cheese, Flour}, {Sugar, Bread}}));

    EXPECT_TRUE(rules.contains({{Bread}, {Cheese, Flour, Sugar}}));
}

TEST_F(AprioriTests, AprioriTest) {
    const auto &transactions = get_transactions();

    const auto min_support = 0.4f;
    const auto min_conf = 0.9f;
    const auto &[rules, frequent_itemsets, support_counter] = apriori(transactions, min_support, min_conf);

    EXPECT_EQ(frequent_itemsets.size(), 35);
    EXPECT_EQ(rules.size(), 29);

    EXPECT_TRUE(rules.contains({{Bread, Flour, Sugar}, {Cheese}}));
    EXPECT_TRUE(rules.contains({{Cheese, Flour, Sugar}, {Bread}}));
    EXPECT_TRUE(rules.contains({{Cheese, Bread, Sugar}, {Flour}}));
    EXPECT_TRUE(rules.contains({{Cheese, Bread, Flour}, {Sugar}}));

    EXPECT_TRUE(rules.contains({{Bread, Flour}, {Cheese, Sugar}}));
    EXPECT_TRUE(rules.contains({{Bread, Cheese}, {Flour, Sugar}}));
    EXPECT_TRUE(rules.contains({{Bread, Sugar}, {Cheese, Flour}}));
    EXPECT_TRUE(rules.contains({{Cheese, Flour}, {Sugar, Bread}}));

    EXPECT_TRUE(rules.contains({{Bread}, {Cheese, Flour, Sugar}}));
}
