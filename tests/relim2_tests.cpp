/// @file relim2_tests.cpp
/// @brief Unit test for the RElim algorithm.
/// @see https://borgelt.net/papers/simple.pdf
/// @see https://borgelt.net/papers/relim.pdf
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
using namespace fim::algorithm::relim;

using std::views::transform;
using std::ranges::to;

class Relim2Tests : public ::testing::Test {
protected:
    static size_t min_support() { return 2; }

    static auto get_database() -> database_t {
        return database_t{
                {'a', 'd'},
                {'a', 'c', 'd', 'e'},
                {'b', 'd'},
                {'b', 'c', 'd'},
                {'b', 'c', 'f'},
                {'a', 'b', 'd'},
                {'b', 'd', 'e'},
                {'b', 'c', 'd', 'e'},
                {'g', 'b', 'c'},
                {'a', 'b', 'd'}
        };
    }

    static auto validate_suffixes(const suffixes_t &suffixes, const suffixes_t &expected_suffixes) -> void {
        auto it = suffixes.begin();
        auto it_expected = expected_suffixes.begin();

        while (it != suffixes.end()) {
            ASSERT_EQ(it->count, it_expected->count);
            ASSERT_EQ(it->itemset, it_expected->itemset);

            it++;
            it_expected++;
        }
    }

    static auto validate_header(const header_element_t &header, const header_element_t &expected_header) -> void {
        ASSERT_EQ(header.count, expected_header.count);
        ASSERT_EQ(header.prefix, expected_header.prefix);
        ASSERT_EQ(header.suffixes.size(), expected_header.suffixes.size());

        validate_suffixes(header.suffixes, expected_header.suffixes);
    }
};

TEST_F(Relim2Tests, ItemComparerTest) {
    const auto db = get_database();
    const auto &counts = item_counts_t::get_item_counts(db);
    const auto comp = counts.get_item_comparer();

    // item order: e < a < c < b < d
    EXPECT_TRUE(comp('e', 'a'));
    EXPECT_TRUE(comp('e', 'c'));
    EXPECT_TRUE(comp('e', 'b'));
    EXPECT_TRUE(comp('e', 'd'));
    EXPECT_TRUE(comp('a', 'c'));
    EXPECT_TRUE(comp('a', 'b'));
    EXPECT_TRUE(comp('a', 'd'));
    EXPECT_TRUE(comp('c', 'b'));
    EXPECT_TRUE(comp('c', 'd'));

    EXPECT_FALSE(comp('d', 'c'));
    EXPECT_FALSE(comp('d', 'a'));
    EXPECT_FALSE(comp('d', 'e'));
    EXPECT_FALSE(comp('b', 'a'));
    EXPECT_FALSE(comp('b', 'e'));
    EXPECT_FALSE(comp('c', 'a'));
    EXPECT_FALSE(comp('c', 'e'));
    EXPECT_FALSE(comp('a', 'e'));
}

TEST_F(Relim2Tests, LexicographicalCompareTest) {
    const auto &counts = item_counts_t::get_item_counts(get_database());
    const auto comp = counts.get_item_comparer();

    EXPECT_TRUE(lexicographical_compare(itemset_t{'a', 'd'}, itemset_t{'a'}, comp));
    EXPECT_TRUE(lexicographical_compare(itemset_t{'e', 'd'}, itemset_t{'a'}, comp));
    EXPECT_TRUE(lexicographical_compare(itemset_t{'e', 'd'}, itemset_t{'a'}, comp));
    EXPECT_TRUE(lexicographical_compare(itemset_t{'a', 'c'}, itemset_t{'a', 'b'}, comp));
    EXPECT_TRUE(lexicographical_compare(itemset_t{'a', 'b'}, itemset_t{'a', 'd'}, comp));

    EXPECT_FALSE(lexicographical_compare(itemset_t{'a', 'c'}, itemset_t{'a', 'c', 'b'}, comp));
    EXPECT_FALSE(lexicographical_compare(itemset_t{'e', 'b', 'c'}, itemset_t{'e', 'b', 'c'}, comp));
    EXPECT_FALSE(lexicographical_compare(itemset_t{'c', 'b', 'd'}, itemset_t{'c', 'b', 'a'}, comp));
}

TEST_F(Relim2Tests, AddSuffixItemsetTest) {
    const auto &counts = item_counts_t::get_item_counts(get_database());
    const auto comp = counts.get_item_comparer();

    suffixes_t suffixes{};

    auto add_itemset = [&](itemset_t suffix) -> void {
        // item order: e < a < c < b < d
        suffixes.add_itemset(suffix.sort_itemset(comp), comp);
    };

    add_itemset(itemset_t{'a', 'd'});
    add_itemset(itemset_t{'d'});
    add_itemset(itemset_t{'b', 'a'});
    add_itemset(itemset_t{'b', 'e', 'a'});
    add_itemset(itemset_t{'d'});
    add_itemset(itemset_t{'a', 'e', 'b'});
    add_itemset(itemset_t{'d'});
    add_itemset(itemset_t{'d'});
    add_itemset(itemset_t{'a', 'd'});
    add_itemset(itemset_t{'b', 'd'});

    validate_suffixes(suffixes, suffixes_t{
            suffix_t{2, itemset_t({'e', 'a', 'b'})},
            suffix_t{1, itemset_t({'a', 'b'})},
            suffix_t{2, itemset_t({'a', 'd'})},
            suffix_t{1, itemset_t({'b', 'd'})},
            suffix_t{4, itemset_t({'d'})},
    });
}

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

TEST_F(Relim2Tests, CreateInitialDatabaseTest) {
    database_t db = get_database();

    const auto item_count = preprocessing(db, min_support());
    const auto comp = item_count.get_item_comparer();

    db.sort_lexicographically(comp);

    const auto freq_items = item_count.get_frequent_items(min_support());
    auto conditional_db = conditional_database_t::create_initial_database(db, freq_items, comp);

    const auto header = conditional_db.header;
    ASSERT_EQ(header.size(), 5);

    validate_header(header[0], header_element_t{
            0, 'd', suffixes_t{}
    });

    validate_header(header[1], header_element_t{
            1, 'b', suffixes_t{
                    suffix_t{1, itemset_t({'d'})}
            }
    });

    validate_header(header[2], header_element_t{
            3, 'c', suffixes_t{
                    suffix_t{1, itemset_t({'b', 'd'})},
                    suffix_t{2, itemset_t({'b'})}
            }
    });

    validate_header(header[3], header_element_t{
            3, 'a', suffixes_t{
                    suffix_t{2, itemset_t({'b', 'd'})},
                    suffix_t{1, itemset_t({'d'})}
            }
    });

    validate_header(header[4], header_element_t{
            3, 'e', suffixes_t{
                    suffix_t{1, itemset_t({'a', 'c', 'd'})},
                    suffix_t{1, itemset_t({'c', 'b', 'd'})},
                    suffix_t{1, itemset_t({'b', 'd'})}
            }
    });
}

TEST_F(Relim2Tests, GetPrefixDatabaseTest) {
    database_t db = get_database();

    const auto item_count = preprocessing(db, min_support());
    const auto comp = item_count.get_item_comparer();

    db.sort_lexicographically(comp);

    const auto freq_items = item_count.get_frequent_items(min_support());
    auto conditional_db = conditional_database_t::create_initial_database(db, freq_items, comp);

    const auto &prefix_db = conditional_db.get_prefix_database();

    const auto header = prefix_db.header;
    ASSERT_EQ(header.size(), 4);

    validate_header(header[0], header_element_t{
            0, 'd', suffixes_t{}
    });

    validate_header(header[1], header_element_t{
            1, 'b', suffixes_t{
                    suffix_t{1, itemset_t({'d'})}
            }
    });

    validate_header(header[2], header_element_t{
            1, 'c', suffixes_t{
                    suffix_t{1, itemset_t({'b', 'd'})},
            }
    });

    validate_header(header[3], header_element_t{
            1, 'a', suffixes_t{
                    suffix_t{1, itemset_t({'c', 'd'})}
            }
    });
}

TEST_F(Relim2Tests, EliminateTest) {
    database_t db = get_database();

    const auto item_count = preprocessing(db, min_support());
    const auto comp = item_count.get_item_comparer();

    db.sort_lexicographically(comp);

    const auto freq_items = item_count.get_frequent_items(min_support());
    auto conditional_db = conditional_database_t::create_initial_database(db, freq_items, comp);

    // eliminated prefix 'e'
    auto prefix = conditional_db.eliminate(conditional_db.get_prefix_database());
    EXPECT_EQ(prefix, 'e');

    auto header = conditional_db.header;
    ASSERT_EQ(header.size(), 4);

    validate_header(header[0], header_element_t{
            0, 'd', suffixes_t{}
    });

    validate_header(header[1], header_element_t{
            2, 'b', suffixes_t{
                    suffix_t{2, itemset_t({'d'})}
            }
    });

    validate_header(header[2], header_element_t{
            4, 'c', suffixes_t{
                    suffix_t{2, itemset_t({'b', 'd'})},
                    suffix_t{2, itemset_t({'b'})}
            }
    });

    validate_header(header[3], header_element_t{
            4, 'a', suffixes_t{
                    suffix_t{1, itemset_t({'c', 'd'})},
                    suffix_t{2, itemset_t({'b', 'd'})},
                    suffix_t{1, itemset_t({'d'})}
            }
    });

    // eliminated prefix 'a'
    prefix = conditional_db.eliminate(conditional_db.get_prefix_database());
    EXPECT_EQ(prefix, 'a');

    header = conditional_db.header;
    ASSERT_EQ(header.size(), 3);

    validate_header(header[0], header_element_t{
            1, 'd', suffixes_t{}
    });

    validate_header(header[1], header_element_t{
            4, 'b', suffixes_t{
                    suffix_t{4, itemset_t({'d'})}
            }
    });

    validate_header(header[2], header_element_t{
            5, 'c', suffixes_t{
                    suffix_t{2, itemset_t({'b', 'd'})},
                    suffix_t{2, itemset_t({'b'})},
                    suffix_t{1, itemset_t({'d'})},
            }
    });

    // eliminated prefix 'c'
    prefix = conditional_db.eliminate(conditional_db.get_prefix_database());
    EXPECT_EQ(prefix, 'c');

    header = conditional_db.header;
    EXPECT_EQ(header.size(), 2);

    // eliminated prefix 'b'
    prefix = conditional_db.eliminate(conditional_db.get_prefix_database());
    EXPECT_EQ(prefix, 'b');

    header = conditional_db.header;
    EXPECT_EQ(header.size(), 1);
}

TEST_F(Relim2Tests, RelimAlgorithmTest) {
    database_t db = get_database();

    const auto freq_items = relim_algorithm(db, min_support()).sort_each_itemset();
    ASSERT_EQ(freq_items.size(), 17);
//
//    EXPECT_EQ(freq_items[0], itemset_t{'e'});
//    EXPECT_EQ(freq_items[1], itemset_t{'a'});
//    EXPECT_EQ(freq_items[2], itemset_t{'c'});
//    EXPECT_EQ(freq_items[3], itemset_t{'b'});
//    EXPECT_EQ(freq_items[4], itemset_t{'d'});

    // ...


}

