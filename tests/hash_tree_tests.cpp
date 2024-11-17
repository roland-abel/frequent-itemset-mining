/// @file hash_tree_tests.cpp
/// @brief
///
/// @author Roland Abel
/// @date September 16, 2024
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
#include "hash_tree.h"

using namespace fim;
using namespace fim::hash;

class HashTreeTests : public ::testing::Test {
protected:
    static auto hash_func(const item_t &item) {
        return item % 5;
    }
};

TEST_F(HashTreeTests, InsertSearchTest) {
    hash_tree tree(3, hash_func);

    tree.insert({0, 1, 2});
    tree.insert({3, 6, 7});
    tree.insert({3, 5, 7});
    tree.insert({2, 4, 6});
    tree.insert({2, 4, 7});

    EXPECT_TRUE(tree.search({0, 1, 2}).has_value());
    EXPECT_TRUE(tree.search({3, 6, 7}).has_value());
    EXPECT_TRUE(tree.search({3, 5, 7}).has_value());
    EXPECT_TRUE(tree.search({2, 4, 6}).has_value());
    EXPECT_TRUE(tree.search({2, 4, 7}).has_value());
}

TEST_F(HashTreeTests, HashTreeIteratorTest) {
    hash_tree tree(3, hash_func);

    tree.insert({0, 1, 2});
    tree.insert({3, 6, 7});
    tree.insert({3, 5, 7});
    tree.insert({2, 4, 6});
    tree.insert({2, 4, 7});

    auto it = tree.begin();

    EXPECT_EQ(*it, itemset_t({2, 4, 6}));
    EXPECT_EQ(*(++it), itemset_t({2, 4, 7}));
    EXPECT_EQ(*(++it), itemset_t({3, 6, 7}));
    EXPECT_EQ(*(++it), itemset_t({3, 5, 7}));
    EXPECT_EQ(*(++it), itemset_t({0, 1, 2}));
}
