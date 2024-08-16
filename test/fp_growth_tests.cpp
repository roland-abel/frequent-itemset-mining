/// @file fp_growth_tests.cpp
/// @brief Unit test for the FP-Growth algorithm.
///
/// @author Roland Abel
/// @date August 10, 2024
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
#include <ranges>
#include "fp_growth.h"

using namespace rules;
using namespace rules::apriori;
using namespace rules::fp_growth;

namespace {
    float eps = 1e-4;
}

class FPGrowthTests : public ::testing::Test {
protected:
    enum Items {
        Milk, Bread, Cheese, Butter, Coffee, Sugar, Flour, Cream
    };

    static inline transactions_t get_transactions() {
        return {
                {Milk,   Cheese, Butter, Bread,  Sugar,  Flour, Cream},
                {Cheese, Butter, Bread,  Coffee, Sugar,  Flour},
                {Milk,   Butter, Coffee, Sugar,  Flour},
                {Milk,   Butter},
                {Milk,   Butter, Coffee},
                {Milk,   Flour},
                {Milk,   Cheese, Butter, Bread,  Coffee, Sugar, Flour},
                {Cream},
                {Milk,   Cheese, Butter, Sugar},
                {Milk,   Cheese, Bread,  Coffee, Sugar,  Flour}
        };
    }
};

TEST_F(FPGrowthTests, MakeRootTest) {
    auto root = make_root();

    EXPECT_EQ(root->item, {});
    EXPECT_EQ(root->frequency, 0);
    EXPECT_EQ(root->parent, nullptr);
    EXPECT_TRUE(root->children.empty());

    root->children.push_back(make_root());
}

TEST_F(FPGrowthTests, MakeNodeTest) {
    auto root = make_root();

    auto node1 = make_node(Milk, 3, root);
    auto node2 = make_node(Sugar, 2, root);

    EXPECT_EQ(node1->item, Milk);
    EXPECT_EQ(node1->frequency, 3);
    EXPECT_EQ(node1->parent, root);
    EXPECT_TRUE(node1->children.empty());

    EXPECT_EQ(node2->item, Sugar);
    EXPECT_EQ(node2->frequency, 2);
    EXPECT_EQ(node2->parent, root);
    EXPECT_TRUE(node2->children.empty());

    EXPECT_EQ(root->children.size(), 2);
}

TEST_F(FPGrowthTests, FindNodeTest) {
    auto root = make_root();
    auto children = children_t{
            make_node(Milk, 3, root),
            make_node(Sugar, 2, root),
            make_node(Coffee, 2, root)
    };

    auto it = find_node(children, Sugar);
    EXPECT_TRUE(it != children.end());

    const auto& node = *it;
    EXPECT_EQ(node->frequency, 2);
}

TEST_F(FPGrowthTests, NodeNotFoundTest) {
    auto root = make_root();
    auto children = children_t{
            make_node(Milk, 3, root),
            make_node(Sugar, 2, root),
            make_node(Coffee, 2, root)
    };

    auto it = find_node(children, Flour);
    EXPECT_TRUE(it == children.end());
}

TEST_F(FPGrowthTests, CreateFrequentItemsTest) {
    const auto min_support_abs = 6;
    const auto &transactions = get_transactions();

    auto items = create_frequent_items(transactions, min_support_abs);
    auto is_frequent_item = [&](const item_t item) {
        return std::find(items.begin(), items.end(), item) != items.end();
    };

    EXPECT_EQ(items.size(), 4);

    EXPECT_TRUE(is_frequent_item(Milk));
    EXPECT_TRUE(is_frequent_item(Butter));
    EXPECT_TRUE(is_frequent_item(Sugar));
    EXPECT_TRUE(is_frequent_item(Flour));

    EXPECT_TRUE(is_frequent_item(Flour));
}

