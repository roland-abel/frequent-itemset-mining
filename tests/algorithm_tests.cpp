/// @file algorithm_tests.cpp
/// @brief
///
/// @author Roland Abel
/// @date November 07, 2024
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
#include "apriori.h"
#include "fp_growth.h"
#include "relim.h"
#include "eclat.h"

using namespace fim;
using namespace fim::algorithms::fp_growth;
using namespace fim::algorithms::relim;
using namespace fim::algorithms::eclat;

using std::views::transform;

class AlgorithmTests : public ::testing::Test {
protected:
    using algorithm_t = std::function<itemsets_t(database_t &database, size_t min_support)>;
    using algorithms_t = std::vector<std::pair<std::string_view, algorithm_t>>;

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

    const algorithms_t get_algorithms() const {
        return {
                {"apriori",   fim::algorithms::apriori::apriori_algorithm},
                {"fp_growth", fim::algorithms::fp_growth::fp_growth_algorithm},
                // {"relim",     fim::algorithms::relim::relim_algorithm},
                {"eclat",     fim::algorithms::eclat::eclat_algorithm},
        };
    }

    static size_t min_support() {
        return 4;
    }

    static database_t get_database() {
        return database_t{
                {Milk,   Cheese, Butter, Bread,  Sugar,  Flour, Cream},
                {Cheese, Butter, Bread,  Coffee, Sugar,  Flour},
                {Milk,   Butter, Coffee, Sugar,  Flour},
                {Milk,   Cream,  Butter},
                {Milk,   Butter, Coffee},
                {Milk,   Flour},
                {Milk,   Cheese, Butter, Bread,  Coffee, Sugar, Flour},
                {Cream},
                {Milk,   Cheese, Butter, Sugar},
                {Milk,   Cheese, Bread,  Coffee, Sugar,  Flour}
        }.sort_database();
    }

    static void validate_algorithm(const algorithm_t &algorithm) {
        auto db = get_database();
        auto itemsets = algorithm(db, min_support()).sort_each_itemset();

        ASSERT_EQ(itemsets.size(), 35);

        // 1-suffix
        EXPECT_TRUE(itemsets.contains({Milk}));
        EXPECT_TRUE(itemsets.contains({Bread}));
        EXPECT_TRUE(itemsets.contains({Cheese}));
        EXPECT_TRUE(itemsets.contains({Butter}));
        EXPECT_TRUE(itemsets.contains({Coffee}));
        EXPECT_TRUE(itemsets.contains({Sugar}));
        EXPECT_TRUE(itemsets.contains({Flour}));

        // 2-suffix
        EXPECT_TRUE(itemsets.contains({Milk, Cheese}));
        EXPECT_TRUE(itemsets.contains({Milk, Butter}));
        EXPECT_TRUE(itemsets.contains({Milk, Coffee}));
        EXPECT_TRUE(itemsets.contains({Milk, Sugar}));
        EXPECT_TRUE(itemsets.contains({Milk, Flour}));
        EXPECT_TRUE(itemsets.contains({Bread, Cheese}));
        EXPECT_TRUE(itemsets.contains({Bread, Sugar}));
        EXPECT_TRUE(itemsets.contains({Bread, Flour}));
        EXPECT_TRUE(itemsets.contains({Cheese, Butter}));
        EXPECT_TRUE(itemsets.contains({Cheese, Sugar}));
        EXPECT_TRUE(itemsets.contains({Cheese, Flour}));
        EXPECT_TRUE(itemsets.contains({Butter, Coffee}));
        EXPECT_TRUE(itemsets.contains({Butter, Sugar}));
        EXPECT_TRUE(itemsets.contains({Butter, Flour}));
        EXPECT_TRUE(itemsets.contains({Coffee, Sugar}));
        EXPECT_TRUE(itemsets.contains({Coffee, Flour}));
        EXPECT_TRUE(itemsets.contains({Sugar, Flour}));

        // 3-suffix
        EXPECT_TRUE(itemsets.contains({Milk, Cheese, Sugar}));
        EXPECT_TRUE(itemsets.contains({Milk, Butter, Sugar}));
        EXPECT_TRUE(itemsets.contains({Milk, Sugar, Flour}));
        EXPECT_TRUE(itemsets.contains({Bread, Cheese, Sugar}));
        EXPECT_TRUE(itemsets.contains({Bread, Cheese, Flour}));
        EXPECT_TRUE(itemsets.contains({Bread, Sugar, Flour}));
        EXPECT_TRUE(itemsets.contains({Cheese, Butter, Sugar}));
        EXPECT_TRUE(itemsets.contains({Cheese, Sugar, Flour}));
        EXPECT_TRUE(itemsets.contains({Butter, Sugar, Flour}));
        EXPECT_TRUE(itemsets.contains({Coffee, Sugar, Flour}));

        // 4-suffix
        EXPECT_TRUE(itemsets.contains({Bread, Cheese, Sugar, Flour}));
    }
};

TEST_F(AlgorithmTests, AlgorithmTest) {
//    const auto &algorithms = get_algoritmus();
//    for (const auto &[name, algorithm]: algorithms) {
//        validate_algorithm(algorithm);
//    }


//
//    const auto &itemsets = apriori_algorithm(db, min_support()).sort_each_itemset();
//
//    ASSERT_EQ(itemsets.size(), 35);
//
//    // 1-suffix
//    EXPECT_TRUE(itemsets.contains({Milk}));
//    EXPECT_TRUE(itemsets.contains({Bread}));
//    EXPECT_TRUE(itemsets.contains({Cheese}));
//    EXPECT_TRUE(itemsets.contains({Butter}));
//    EXPECT_TRUE(itemsets.contains({Coffee}));
//    EXPECT_TRUE(itemsets.contains({Sugar}));
//    EXPECT_TRUE(itemsets.contains({Flour}));
//
//    // 2-suffix
//    EXPECT_TRUE(itemsets.contains({Milk, Cheese}));
//    EXPECT_TRUE(itemsets.contains({Milk, Butter}));
//    EXPECT_TRUE(itemsets.contains({Milk, Coffee}));
//    EXPECT_TRUE(itemsets.contains({Milk, Sugar}));
//    EXPECT_TRUE(itemsets.contains({Milk, Flour}));
//    EXPECT_TRUE(itemsets.contains({Bread, Cheese}));
//    EXPECT_TRUE(itemsets.contains({Bread, Sugar}));
//    EXPECT_TRUE(itemsets.contains({Bread, Flour}));
//    EXPECT_TRUE(itemsets.contains({Cheese, Butter}));
//    EXPECT_TRUE(itemsets.contains({Cheese, Sugar}));
//    EXPECT_TRUE(itemsets.contains({Cheese, Flour}));
//    EXPECT_TRUE(itemsets.contains({Butter, Coffee}));
//    EXPECT_TRUE(itemsets.contains({Butter, Sugar}));
//    EXPECT_TRUE(itemsets.contains({Butter, Flour}));
//    EXPECT_TRUE(itemsets.contains({Coffee, Sugar}));
//    EXPECT_TRUE(itemsets.contains({Coffee, Flour}));
//    EXPECT_TRUE(itemsets.contains({Sugar, Flour}));
//
//    // 3-suffix
//    EXPECT_TRUE(itemsets.contains({Milk, Cheese, Sugar}));
//    EXPECT_TRUE(itemsets.contains({Milk, Butter, Sugar}));
//    EXPECT_TRUE(itemsets.contains({Milk, Sugar, Flour}));
//    EXPECT_TRUE(itemsets.contains({Bread, Cheese, Sugar}));
//    EXPECT_TRUE(itemsets.contains({Bread, Cheese, Flour}));
//    EXPECT_TRUE(itemsets.contains({Bread, Sugar, Flour}));
//    EXPECT_TRUE(itemsets.contains({Cheese, Butter, Sugar}));
//    EXPECT_TRUE(itemsets.contains({Cheese, Sugar, Flour}));
//    EXPECT_TRUE(itemsets.contains({Butter, Sugar, Flour}));
//    EXPECT_TRUE(itemsets.contains({Coffee, Sugar, Flour}));
//
//    // 4-suffix
//    EXPECT_TRUE(itemsets.contains({Bread, Cheese, Sugar, Flour}));
}

