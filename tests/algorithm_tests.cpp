/// @file algorithm_tests.cpp
/// @brief
///
/// @author Roland Abel
/// @date December 8, 2024
///
/// Copyright (c) 2024 Roland Abel
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
#include "algorithms.h"
#include "reader.h"
#include "writer.h"

using namespace fim;
using namespace fim::algorithm;

class AlgorithmTest : public testing::TestWithParam<algorithm_function_t> {
public:
    AlgorithmTest() = default;

protected:
    static constexpr char separator = ',';

    static algorithm_function_t get_algorthm() { return GetParam(); }

    data::read_result_t read_csv() const {
        return data::read_csv(input_path_, data::read_csv_config_t{
                                  .skip_rows = 0,
                                  .separator = separator
                              });
    };

    data::result_t read_excepted_frequent_itemsets() const {
        return data::read_result_csv(result_file_, data::read_csv_config_t{
                                         .skip_rows = 1,
                                         .separator = separator
                                     });
    };

    size_t get_min_support(const size_t db_size) const {
        return min_support_ * db_size;
    };

private:
    const float min_support_ = 0.5;
    const std::string input_path_ = "./data/data_01.csv";
    const std::string result_file_ = "./data/result_01.csv";
};

TEST_P(AlgorithmTest, ApplyAlgorithm) {
    // auto apply_algorithm = [&](const database_t &database) -> itemsets_t {
    //     auto &db = const_cast<database_t &>(database);
    //     const size_t min_support = get_min_support(db.size());
    //
    //     const auto & algorithm = get_algorthm();
    //     return algorithm(db, min_support);
    // };
    //
    // // read the expected result items set
    // const auto expected_freq_itemsets = read_excepted_frequent_itemsets();
    //
    // // read data and apply algorithm
    // const auto output = read_csv().transform(apply_algorithm);
    //
    // // verify result
    // EXPECT_TRUE(output.has_value());
}

INSTANTIATE_TEST_SUITE_P(
    AlgorithmTest,
    AlgorithmTest,
    ::testing::Values(
        apriori::apriori_algorithm,
        fp_growth::fp_growth_algorithm,
        eclat::eclat_algorithm,
        relim::relim_algorithm)
);
