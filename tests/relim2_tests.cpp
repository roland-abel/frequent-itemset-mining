/// @file relim2_tests.cpp
/// @brief Unit test for the RElim algorithm.
/// @ee https://borgelt.net/papers/simple.pdf
///
/// @author Roland Abel
/// @date September 29, 2024
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

class Relim2Tests : public ::testing::Test {
protected:
    static size_t min_support() { return 2; }

    static auto get_database() -> database_t {
        return database_t{
                {'a', 'd'},
                {'a', 'c', 'd', 'e'},
                {'b', 'd'},
                {'b', 'c', 'd'},
                {'b', 'c'},
                {'a', 'b', 'd'},
                {'b', 'd', 'e'},
                {'b', 'c', 'd', 'e'},
                {'b', 'c'},
                {'a', 'b', 'd'}
        };
    }
};

TEST_F(Relim2Tests, PreprocessingDatabaseTest) {
    database_t db = get_database();
    const auto item_count = preprocessing(db, min_support());

    EXPECT_EQ(item_count.at('e'), 3);
    EXPECT_EQ(item_count.at('a'), 4);
    EXPECT_EQ(item_count.at('c'), 5);
    EXPECT_EQ(item_count.at('b'), 8);
    EXPECT_EQ(item_count.at('d'), 8);

    ASSERT_EQ(db.size(), 10);

    EXPECT_EQ(db[0], itemset_t({'a', 'd'}));
    EXPECT_EQ(db[1], itemset_t({'e', 'a', 'c', 'd'}));
    EXPECT_EQ(db[2], itemset_t({'b', 'd'}));
    EXPECT_EQ(db[3], itemset_t({'c', 'b', 'd'}));
    EXPECT_EQ(db[4], itemset_t({'c', 'b'}));
    EXPECT_EQ(db[5], itemset_t({'a', 'b', 'd'}));
    EXPECT_EQ(db[6], itemset_t({'e', 'b', 'd'}));
    EXPECT_EQ(db[6], itemset_t({'e', 'b', 'd'}));
    EXPECT_EQ(db[7], itemset_t({'e', 'c', 'b', 'd'}));
    EXPECT_EQ(db[8], itemset_t({'c', 'b'}));
    EXPECT_EQ(db[9], itemset_t({'a', 'b', 'd'}));
}

TEST_F(Relim2Tests, SortLexicographicallyTest) {
    database_t db = get_database();
    const auto count = preprocessing(db, min_support());

    db.sort_lexicographically(count.get_item_comparer());
    ASSERT_EQ(db.size(), 10);

    EXPECT_EQ(db[0], itemset_t({'e', 'a', 'c', 'd'}));
    EXPECT_EQ(db[1], itemset_t({'e', 'c', 'b', 'd'}));
    EXPECT_EQ(db[2], itemset_t({'e', 'b', 'd'}));
    EXPECT_EQ(db[3], itemset_t({'a', 'b', 'd'}));
    EXPECT_EQ(db[4], itemset_t({'a', 'b', 'd'}));
    EXPECT_EQ(db[5], itemset_t({'a', 'd'}));
    EXPECT_EQ(db[6], itemset_t({'c', 'b', 'd'}));
    EXPECT_EQ(db[7], itemset_t({'c', 'b'}));
    EXPECT_EQ(db[8], itemset_t({'c', 'b'}));
    EXPECT_EQ(db[9], itemset_t({'b', 'd'}));
}

TEST_F(Relim2Tests, CreateInitialDatabase) {
    database_t db = get_database();
    const auto item_count = preprocessing(db, min_support());
    db.sort_lexicographically(item_count.get_item_comparer());

    auto initial_db = create_initial_database(db);

    auto [count_a, list] = initial_db.get('a');
    EXPECT_EQ(count_a, 3);
    EXPECT_EQ(list.size(), 2);

    auto it = list.begin();
    EXPECT_EQ(it->count, 2);
    EXPECT_EQ(it->itemset, itemset_t({'b', 'd'}));

    it++;
    EXPECT_EQ(it->count, 1);
    EXPECT_EQ(it->itemset, itemset_t({'d'}));


}

TEST_F(Relim2Tests, RelimAlgorithmTest) {
    const auto &database = get_database();
    const auto &itemsets = relim_algorithm(database, min_support()).sort_each_itemset();

}

