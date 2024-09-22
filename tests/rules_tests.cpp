/// @file rules_tests.cpp
/// @brief Unit test for the Apriori algorithm.
///
/// @author Roland Abel
/// @date September 22, 2024
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

#include <gtest/gtest.h>
#include <ranges>
#include "utils.h"
#include "test_data.h"

using namespace fim;
using namespace fim::tests;
using namespace std::chrono;

class RulesTests : public ::testing::Test {
protected:
    float eps = 1e-4;
};


// TODO ...

//TEST_F(RulesTests, ConfidenceTest) {
//    const auto min_support = 4;
//
//    const auto &database = get_database();
//    const auto [itemsets, frequencies] = apriori_algorithm(database, min_support);
//
//    EXPECT_NEAR(get_confidence(frequencies, itemset_t({Bread, Flour, Sugar}), itemset_t({Cheese})), 1.0, eps);
//    EXPECT_NEAR(get_confidence(frequencies, itemset_t({Sugar, Flour}), itemset_t({Milk})), 0.8, eps);
//    EXPECT_NEAR(get_confidence(frequencies, itemset_t({Flour}), itemset_t({Milk})), 0.8333, eps);
//    EXPECT_NEAR(get_confidence(frequencies, itemset_t({Milk}), itemset_t({Butter})), 0.75, eps);
//}
//
//TEST_F(RulesTests, GenerateRulesForOneItemsetTest) {
//    const auto min_support = 4;
//    const auto min_confidence = 0.9f;
//
//    const auto &database = get_database();
//    const auto [itemsets, frequencies] = apriori_algorithm(database, min_support);
//
//    const itemset_t z = {Cheese, Bread, Sugar, Flour};
//    const auto rules = generate_rules(z, frequencies, min_confidence);
//
//    auto check_rule = [&](const itemset_t &premise, const itemset_t &conclusion, float conf) -> bool {
//        return rules.contains({premise, conclusion})
//               && std::abs(get_confidence(frequencies, premise, conclusion) - conf) < eps;
//    };
//
//    EXPECT_EQ(rules.size(), 9);
//
//    EXPECT_TRUE(check_rule({Bread, Flour, Sugar}, {Cheese}, 1.0));
//    EXPECT_TRUE(check_rule({Cheese, Flour, Sugar}, {Bread}, 1.0));
//    EXPECT_TRUE(check_rule({Cheese, Bread, Sugar}, {Flour}, 1.0));
//    EXPECT_TRUE(check_rule({Cheese, Bread, Flour}, {Sugar}, 1.0));
//
//    EXPECT_TRUE(check_rule({Bread, Flour}, {Cheese, Sugar}, 1.0));
//    EXPECT_TRUE(check_rule({Bread, Cheese}, {Flour, Sugar}, 1.0));
//    EXPECT_TRUE(check_rule({Bread, Sugar}, {Cheese, Flour}, 1.0));
//    EXPECT_TRUE(check_rule({Cheese, Flour}, {Sugar, Bread}, 1.0));
//
//    EXPECT_TRUE(check_rule({Bread}, {Cheese, Flour, Sugar}, 1.0));
//}
//
//TEST_F(RulesTests, GenerateRulesTest) {
//    const auto min_support = 4;
//    const auto min_confidence = 0.75f;
//
//    const auto &database = get_database();
//    const auto [itemsets, frequencies] = apriori_algorithm(database, min_support);
//    const auto rules = generate_rules(itemsets, frequencies, min_confidence);
//
//    auto check_rule = [&](const itemset_t &premise, const itemset_t &conclusion, float conf) -> bool {
//        return rules.contains({premise, conclusion})
//               && std::abs(get_confidence(frequencies, premise, conclusion) - conf) < eps;
//    };
//
//    EXPECT_EQ(rules.size(), 70);
//
//    EXPECT_TRUE(check_rule({Milk}, {Butter}, 0.75));
//    EXPECT_TRUE(check_rule({Milk, Cheese}, {Sugar}, 1));
//    EXPECT_TRUE(check_rule({Milk, Sugar}, {Cheese}, 0.8));
//    EXPECT_TRUE(check_rule({Milk, Sugar}, {Butter}, 0.8));
//    EXPECT_TRUE(check_rule({Milk, Sugar}, {Flour}, 0.8));
//    EXPECT_TRUE(check_rule({Milk, Flour}, {Sugar}, 0.8));
//    EXPECT_TRUE(check_rule({Bread}, {Cheese}, 1));
//    EXPECT_TRUE(check_rule({Bread}, {Cheese, Sugar}, 1));
//    EXPECT_TRUE(check_rule({Bread}, {Cheese, Sugar, Flour}, 1));
//    EXPECT_TRUE(check_rule({Bread}, {Cheese, Flour}, 1));
//    EXPECT_TRUE(check_rule({Bread}, {Sugar}, 1));
//    EXPECT_TRUE(check_rule({Bread}, {Sugar, Flour}, 1));
//    EXPECT_TRUE(check_rule({Bread}, {Flour}, 1));
//    EXPECT_TRUE(check_rule({Bread, Cheese}, {Sugar}, 1));
//    EXPECT_TRUE(check_rule({Bread, Cheese}, {Sugar, Flour}, 1));
//    EXPECT_TRUE(check_rule({Bread, Cheese}, {Flour}, 1));
//    EXPECT_TRUE(check_rule({Bread, Cheese, Sugar}, {Flour}, 1));
//    EXPECT_TRUE(check_rule({Bread, Cheese, Flour}, {Sugar}, 1));
//    EXPECT_TRUE(check_rule({Bread, Sugar}, {Cheese}, 1));
//    EXPECT_TRUE(check_rule({Bread, Sugar}, {Cheese, Flour}, 1));
//    EXPECT_TRUE(check_rule({Bread, Sugar}, {Flour}, 1));
//    EXPECT_TRUE(check_rule({Bread, Sugar, Flour}, {Cheese}, 1));
//    EXPECT_TRUE(check_rule({Bread, Flour}, {Cheese}, 1));
//    EXPECT_TRUE(check_rule({Bread, Flour}, {Cheese, Sugar}, 1));
//    EXPECT_TRUE(check_rule({Bread, Flour}, {Sugar}, 1));
//    EXPECT_TRUE(check_rule({Cheese}, {Milk}, 0.8));
//    EXPECT_TRUE(check_rule({Cheese}, {Milk, Sugar}, 0.8));
//    EXPECT_TRUE(check_rule({Cheese}, {Bread}, 0.8));
//    EXPECT_TRUE(check_rule({Cheese}, {Bread, Sugar}, 0.8));
//    EXPECT_TRUE(check_rule({Cheese}, {Bread, Sugar, Flour}, 0.8));
//    EXPECT_TRUE(check_rule({Cheese}, {Bread, Flour}, 0.8));
//    EXPECT_TRUE(check_rule({Cheese}, {Butter}, 0.8));
//    EXPECT_TRUE(check_rule({Cheese}, {Butter, Sugar}, 0.8));
//    EXPECT_TRUE(check_rule({Cheese}, {Sugar}, 1));
//    EXPECT_TRUE(check_rule({Cheese}, {Sugar, Flour}, 0.8));
//    EXPECT_TRUE(check_rule({Cheese}, {Flour}, 0.8));
//    EXPECT_TRUE(check_rule({Cheese, Butter}, {Sugar}, 1));
//    EXPECT_TRUE(check_rule({Cheese, Sugar}, {Milk}, 0.8));
//    EXPECT_TRUE(check_rule({Cheese, Sugar}, {Bread}, 0.8));
//    EXPECT_TRUE(check_rule({Cheese, Sugar}, {Bread, Flour}, 0.8));
//    EXPECT_TRUE(check_rule({Cheese, Sugar}, {Butter}, 0.8));
//    EXPECT_TRUE(check_rule({Cheese, Sugar}, {Flour}, 0.8));
//    EXPECT_TRUE(check_rule({Cheese, Sugar, Flour}, {Bread}, 1));
//    EXPECT_TRUE(check_rule({Cheese, Flour}, {Bread}, 1));
//    EXPECT_TRUE(check_rule({Cheese, Flour}, {Bread, Sugar}, 1));
//    EXPECT_TRUE(check_rule({Cheese, Flour}, {Sugar}, 1));
//    EXPECT_TRUE(check_rule({Butter}, {Milk}, 0.857143));
//    EXPECT_TRUE(check_rule({Butter, Sugar}, {Milk}, 0.8));
//    EXPECT_TRUE(check_rule({Butter, Sugar}, {Cheese}, 0.8));
//    EXPECT_TRUE(check_rule({Butter, Sugar}, {Flour}, 0.8));
//    EXPECT_TRUE(check_rule({Butter, Flour}, {Sugar}, 1));
//    EXPECT_TRUE(check_rule({Coffee}, {Milk}, 0.8));
//    EXPECT_TRUE(check_rule({Coffee}, {Butter}, 0.8));
//    EXPECT_TRUE(check_rule({Coffee}, {Sugar}, 0.8));
//    EXPECT_TRUE(check_rule({Coffee}, {Sugar, Flour}, 0.8));
//    EXPECT_TRUE(check_rule({Coffee}, {Flour}, 0.8));
//    EXPECT_TRUE(check_rule({Coffee, Sugar}, {Flour}, 1));
//    EXPECT_TRUE(check_rule({Coffee, Flour}, {Sugar}, 1));
//    EXPECT_TRUE(check_rule({Sugar}, {Milk}, 0.833333));
//    EXPECT_TRUE(check_rule({Sugar}, {Cheese}, 0.833333));
//    EXPECT_TRUE(check_rule({Sugar}, {3}, 0.833333));
//    EXPECT_TRUE(check_rule({Sugar}, {Flour}, 0.833333));
//    EXPECT_TRUE(check_rule({Sugar, Flour}, {Milk}, 0.8));
//    EXPECT_TRUE(check_rule({Sugar, Flour}, {Bread}, 0.8));
//    EXPECT_TRUE(check_rule({Sugar, Flour}, {Bread, Cheese}, 0.8));
//    EXPECT_TRUE(check_rule({Sugar, Flour}, {Cheese}, 0.8));
//    EXPECT_TRUE(check_rule({Sugar, Flour}, {Butter}, 0.8));
//    EXPECT_TRUE(check_rule({Sugar, Flour}, {Coffee}, 0.8));
//    EXPECT_TRUE(check_rule({Flour}, {Milk}, 0.833333));
//    EXPECT_TRUE(check_rule({Flour}, {Sugar}, 0.833333));
//}
