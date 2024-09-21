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
#include "fp_growth.h"
#include "hash_tree.h"

using namespace fim::hash;

enum Items {
    Milk = 1,
    Bread,
    Cheese,
    Butter,
    Coffee,
    Sugar,
    Flour,
    Cream
};

class HashTreeTests : public ::testing::Test {
protected:
    static auto hash_func(const item_t &item) {
        return item % 5;
    }

    static auto get_itemsets() -> itemsets_t {
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

TEST_F(HashTreeTests, InsertSearchTest) {
    const itemset_t itemset =  {Milk, Cheese, Butter, Bread, Sugar, Flour, Cream};
    hash_tree tree(5, hash_func);

    tree.insert(itemset);
    const auto opt = tree.search(itemset);

    ASSERT_TRUE(opt.has_value());
    EXPECT_EQ(opt.value(), itemset);
}
