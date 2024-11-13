/// @file relim_tests.cpp
/// @brief Unit test for the RElim algorithm.
///
/// @author Roland Abel
/// @date September 8, 2024
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
#include "relim.h"

using namespace fim;
using namespace fim::algorithms::relim;

using std::views::transform;

class RelimTests : public ::testing::Test {
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

    static size_t get_min_support() { return 4; }

    static auto get_database() -> database_t {
        return database_t{
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

TEST_F(RelimTests, PreprocessingDatabaseTest) {
    database_t db = get_database();
    const auto item_count = preprocessing(db, get_min_support());
    const auto comp = item_count.get_item_comparer();

    db.sort_lexicographically(comp);

    ASSERT_EQ(db.size(), 9);

    EXPECT_EQ(db[0], itemset_t({Bread, Cheese, Coffee, Sugar, Flour, Butter, Milk}));
    EXPECT_EQ(db[1], itemset_t({Bread, Cheese, Coffee, Sugar, Flour, Butter}));
    EXPECT_EQ(db[2], itemset_t({Bread, Cheese, Coffee, Sugar, Flour, Milk}));
    EXPECT_EQ(db[3], itemset_t({Bread, Cheese, Sugar, Flour, Butter, Milk}));
    EXPECT_EQ(db[4], itemset_t({Cheese, Sugar, Butter, Milk}));
    EXPECT_EQ(db[5], itemset_t({Coffee, Sugar, Flour, Butter, Milk}));
    EXPECT_EQ(db[6], itemset_t({Coffee, Butter, Milk}));
    EXPECT_EQ(db[7], itemset_t({Flour, Milk}));
    EXPECT_EQ(db[8], itemset_t({Butter, Milk}));
}

TEST_F(RelimTests, RelimAlgorithmTest) {
    const auto min_support = get_min_support();
    database_t db = get_database();

//    const auto item_count = preprocessing(db, min_support);
//    const auto comp = item_count.get_item_comparer();
//
//    db.sort_lexicographically(comp);


   // fim::algorithms::relim::relim_algorithm(db, min_support);

//    const auto freq_items = item_count.get_frequent_items(min_support);
//    auto conditional_db = conditional_database_t::create_initial_database(db, freq_items, comp);
//
//    const auto header = conditional_db.header;
//    ASSERT_EQ(header.size(), 7);


//    const auto &prefix_db = conditional_db.get_prefix_database();
//    const auto prefix = conditional_db.eliminate(prefix_db);






//    auto sort_each_itemset = [](itemset_t &x) -> itemset_t { return x.sort_itemset(); };
//
//    const auto min_support = 4;
//    const auto &database = get_database();
//


//    const auto &suffix = relim_algorithm(db, min_support).sort_each_itemset();

//    EXPECT_EQ(suffix.size(), 35);

    // TODO

//    // 1-suffix
//    EXPECT_TRUE(suffix.contains({Milk}));
//    EXPECT_TRUE(suffix.contains({Bread}));
//    EXPECT_TRUE(suffix.contains({Cheese}));
//    EXPECT_TRUE(suffix.contains({Butter}));
//    EXPECT_TRUE(suffix.contains({Coffee}));
//    EXPECT_TRUE(suffix.contains({Sugar}));
//    EXPECT_TRUE(suffix.contains({Flour}));
//
//    // 2-suffix
//    EXPECT_TRUE(suffix.contains({Milk, Cheese}));
//    EXPECT_TRUE(suffix.contains({Milk, Butter}));
//    EXPECT_TRUE(suffix.contains({Milk, Coffee}));
//    EXPECT_TRUE(suffix.contains({Milk, Sugar}));
//    EXPECT_TRUE(suffix.contains({Milk, Flour}));
//    EXPECT_TRUE(suffix.contains({Bread, Cheese}));
//    EXPECT_TRUE(suffix.contains({Bread, Sugar}));
//    EXPECT_TRUE(suffix.contains({Bread, Flour}));
//    EXPECT_TRUE(suffix.contains({Cheese, Butter}));
//    EXPECT_TRUE(suffix.contains({Cheese, Sugar}));
//    EXPECT_TRUE(suffix.contains({Cheese, Flour}));
//    EXPECT_TRUE(suffix.contains({Butter, Coffee}));
//    EXPECT_TRUE(suffix.contains({Butter, Sugar}));
//    EXPECT_TRUE(suffix.contains({Butter, Flour}));
//    EXPECT_TRUE(suffix.contains({Coffee, Sugar}));
//    EXPECT_TRUE(suffix.contains({Coffee, Flour}));
//    EXPECT_TRUE(suffix.contains({Sugar, Flour}));
//
//    // 3-suffix
//    EXPECT_TRUE(suffix.contains({Milk, Cheese, Sugar}));
//    EXPECT_TRUE(suffix.contains({Milk, Butter, Sugar}));
//    EXPECT_TRUE(suffix.contains({Milk, Sugar, Flour}));
//    EXPECT_TRUE(suffix.contains({Bread, Cheese, Sugar}));
//    EXPECT_TRUE(suffix.contains({Bread, Cheese, Flour}));
//    EXPECT_TRUE(suffix.contains({Bread, Sugar, Flour}));
//    EXPECT_TRUE(suffix.contains({Cheese, Butter, Sugar}));
//    EXPECT_TRUE(suffix.contains({Cheese, Sugar, Flour}));
//    EXPECT_TRUE(suffix.contains({Butter, Sugar, Flour}));
//    EXPECT_TRUE(suffix.contains({Coffee, Sugar, Flour}));
//
//    // 4-suffix
//    EXPECT_TRUE(suffix.contains({Bread, Cheese, Sugar, Flour}));
}

