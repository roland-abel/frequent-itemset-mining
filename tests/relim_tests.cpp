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
using namespace fim::relim;

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
    const auto min_support = 4;
    database_t db = get_database();
    preprocessing(db, min_support);

    ASSERT_EQ(db.size(), 9);

    EXPECT_EQ(db[0], itemset_t({Bread, Cheese, Sugar, Flour, Butter, Milk}));
    EXPECT_EQ(db[1], itemset_t({Bread, Cheese, Coffee, Sugar, Flour, Butter}));
    EXPECT_EQ(db[2], itemset_t({Coffee, Sugar, Flour, Butter, Milk}));
    EXPECT_EQ(db[3], itemset_t({Butter, Milk}));
    EXPECT_EQ(db[4], itemset_t({Coffee, Butter, Milk}));
    EXPECT_EQ(db[5], itemset_t({Flour, Milk}));
    EXPECT_EQ(db[6], itemset_t({Bread, Cheese, Coffee, Sugar, Flour, Butter, Milk}));
    EXPECT_EQ(db[7], itemset_t({Cheese, Sugar, Butter, Milk}));
    EXPECT_EQ(db[8], itemset_t({Bread, Cheese, Coffee, Sugar, Flour, Milk}));
}

TEST_F(RelimTests, CreateinitialDatabase) {
    const auto min_support = 4;
    database_t db = get_database();
    preprocessing(db, min_support);

    auto data = create_initial_database(db);


//    relim_data_t data = {
//            {Milk,  {2, {Bread,  Cheese, Flour, Butter}}},
//            {Flour, {5, {Coffee, Butter, Milk}}},
//    };
//
//    EXPECT_EQ(data.get_count(Milk), 2);
//    EXPECT_EQ(data.get_itemset(Milk), itemset_t({Bread, Cheese, Flour, Butter}));
//
//    EXPECT_EQ(data.get_count(Flour), 5);
//    EXPECT_EQ(data.get_itemset(Flour), itemset_t({Coffee, Butter, Milk}));
}

TEST_F(RelimTests, ProjectionDatabaseTest) {
    const auto min_support = 4;
    database_t db = get_database();
    preprocessing(db, min_support);

    const auto &proj_db = projection_database(db, Cheese);

    ASSERT_EQ(proj_db.size(), 5);

    EXPECT_EQ(proj_db[0], itemset_t({Milk, Bread, Butter, Sugar, Flour, Cream}));
    EXPECT_EQ(proj_db[1], itemset_t({Bread, Butter, Coffee, Sugar, Flour}));
    EXPECT_EQ(proj_db[2], itemset_t({Milk, Bread, Butter, Coffee, Sugar, Flour}));
    EXPECT_EQ(proj_db[3], itemset_t({Milk, Butter, Sugar}));
    EXPECT_EQ(proj_db[4], itemset_t({Milk, Bread, Coffee, Sugar, Flour}));
}

TEST_F(RelimTests, RelimAlgorithmTest) {
    auto sort_each_itemset = [](itemset_t &x) -> itemset_t { return x.sort(); };

    const auto min_support = 4;
    const auto &database = get_database();

    const auto &itemsets = relim_algorithm(database, min_support)
                           | transform(sort_each_itemset)
                           | std::ranges::to<itemsets_t>();

    EXPECT_EQ(itemsets.size(), 35);

    // TODO

//    // 1-itemsets
//    EXPECT_TRUE(itemsets.contains({Milk}));
//    EXPECT_TRUE(itemsets.contains({Bread}));
//    EXPECT_TRUE(itemsets.contains({Cheese}));
//    EXPECT_TRUE(itemsets.contains({Butter}));
//    EXPECT_TRUE(itemsets.contains({Coffee}));
//    EXPECT_TRUE(itemsets.contains({Sugar}));
//    EXPECT_TRUE(itemsets.contains({Flour}));
//
//    // 2-itemsets
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
//    // 3-itemsets
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
//    // 4-itemsets
//    EXPECT_TRUE(itemsets.contains({Bread, Cheese, Sugar, Flour}));
}

