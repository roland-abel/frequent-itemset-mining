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
#include "test_data.h"

using namespace rules;
using namespace rules::apriori;
using namespace rules::fp_growth;
using namespace rules::tests;

class FPTreeTests : public ::testing::Test {
protected:
};


TEST_F(FPTreeTests, NodeIsRootTest) {
    EXPECT_TRUE(node_t::create_root()->is_root());
}

TEST_F(FPTreeTests, NodeIsNotRootTest) {
    const auto &root = node_t::create_root();
    const auto &child = root->add_child(Milk, 1);

    EXPECT_FALSE(child->is_root());
}

TEST_F(FPTreeTests, RootNodeIsValidTest) {
    const auto &root = node_t::create_root();

    EXPECT_EQ(root->item, 0);
    EXPECT_EQ(root->frequency, 0);
    EXPECT_TRUE(root->children.empty());
}

TEST_F(FPTreeTests, AddChildTest) {
    const auto &root = node_t::create_root();

    const auto &node1 = root->add_child(Milk, 3);
    const auto &node2 = root->add_child(Sugar, 2);

    EXPECT_EQ(node1->item, Milk);
    EXPECT_EQ(node1->frequency, 3);
    EXPECT_TRUE(node1->children.empty());

    EXPECT_EQ(node2->item, Sugar);
    EXPECT_EQ(node2->frequency, 2);
    EXPECT_TRUE(node2->children.empty());

    EXPECT_EQ(root->children.size(), 2);
}

TEST_F(FPTreeTests, FindChildItemTest) {
    const auto &root = node_t::create_root();
    root->add_child(Milk, 3);
    root->add_child(Sugar, 2);
    root->add_child(Coffee, 2);

    auto child = root->find_child_item(Sugar);

    EXPECT_TRUE(child.has_value());
    EXPECT_EQ(child.value()->frequency, 2);
}

TEST_F(FPTreeTests, ChildNotFoundTest) {
    const auto &root = node_t::create_root();
    root->add_child(Milk, 3);
    root->add_child(Sugar, 2);
    root->add_child(Coffee, 2);

    EXPECT_FALSE(root->find_child_item(Flour).has_value());
}

TEST_F(FPTreeTests, TreeHasSinglePathTest) {
    const auto &root = node_t::create_root();
    root->add_child(Milk, 5)->add_child(Cheese, 4)->add_child(Coffee, 2);

    const auto &items_along_path = tree_has_single_path(root);
    EXPECT_TRUE(items_along_path.has_value());

    const auto &path = items_along_path.value();
    EXPECT_FALSE(path.contains(root->item));
    EXPECT_TRUE(path.contains(Milk));
    EXPECT_TRUE(path.contains(Cheese));
    EXPECT_TRUE(path.contains(Coffee));
}

TEST_F(FPTreeTests, TreeHasNoSinglePathTest) {
    const auto &root = node_t::create_root();

    const auto &child = root->add_child(Milk, 8)->add_child(Cheese, 5);
    child->add_child(Butter, 4)->add_child(Flour, 1);
    child->add_child(Coffee, 3)->add_child(Cheese, 1);

    const auto &items_along_path = tree_has_single_path(root);
    EXPECT_FALSE(items_along_path.has_value());
}

TEST_F(FPTreeTests, GetItemFrequencyTest) {
    const auto min_support = 4;
    const auto &root = build_fp_tree(get_database(), min_support);

    EXPECT_EQ(get_item_frequency(root, Milk), 8);
    EXPECT_EQ(get_item_frequency(root, Butter), 7);
    EXPECT_EQ(get_item_frequency(root, Sugar), 6);
    EXPECT_EQ(get_item_frequency(root, Flour), 6);
    EXPECT_EQ(get_item_frequency(root, Cheese), 5);
    EXPECT_EQ(get_item_frequency(root, Coffee), 5);
    EXPECT_EQ(get_item_frequency(root, Bread), 4);
    EXPECT_EQ(get_item_frequency(root, Cream), 0);
}