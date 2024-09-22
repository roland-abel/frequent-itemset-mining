/// @file igm_generator_tests.cpp
/// @brief
///
/// @author Roland Abel
/// @date September 21, 2024
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
#include "itemset.h"
#include "apriori.h"
#include "igm_generator.h"

using namespace fim::itemset;
using namespace fim::generators::igm;

class IgmGeneratorTests : public ::testing::Test {
protected:

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

    static fim_algorithm_t get_algorithm() {
        return fim::algorithms::apriori::apriori_algorithm;
    };

    static database_t get_database() {
        itemsets_t database = {
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

        auto sort_itemset = [](itemset_t &x) -> itemset_t {
            std::ranges::sort(x);
            return x;
        };

        return database
               | std::views::transform(sort_itemset)
               | std::ranges::to<database_t>();
    }
};

TEST_F(IgmGeneratorTests, DummyTest) {
    const size_t min_support = 4;

    const auto &algorithm = get_algorithm();
    const auto &db = get_database();

    const auto synthetic_db = generate_database(db, min_support, algorithm);


}
