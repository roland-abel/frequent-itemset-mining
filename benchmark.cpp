/// @file benchmark.cpp
/// @brief
///
/// @author Roland Abel
/// @date August 30, 2004
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

#include <benchmark/benchmark.h>
#include <vector>
#include <algorithm>
#include <random>
#include "reader.h"
#include "apriori.h"
#include "fp_growth.h"
#include "eclat.h"
#include "relim.h"

using namespace std;
using namespace rules;

namespace {
    static auto generate_test_transactions(size_t num_transactions, size_t max_items) -> transactions_t {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dist(0, max_items - 1);
        std::uniform_int_distribution<size_t> size_dist(1, max_items);

        auto fill_itemset = [&](itemset_t &itemset) -> itemset_t {
            size_t num_items = size_dist(gen);
            for (size_t j = 0; j < num_items; ++j) {
                itemset.insert(dist(gen));
            }
            return itemset;
        };

        return transactions_t(num_transactions, itemset_t{})
               | std::views::transform(fill_itemset)
               | std::ranges::to<transactions_t>();
    }
}

class BenchmarkFixture : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State &state) override {
        const std::string_view filename = "data/mushroom.dat";
        transactions_ = rules::io::read_csv(filename).value();
        min_support_ = 0.95;
    }

    const transactions_t &get_transactions() const {
        return transactions_;
    };

    const size_t get_min_support() const {
        return static_cast<size_t>(min_support_ * transactions_.size());
    };

private:
    transactions_t transactions_{};
    float min_support_{};
};

BENCHMARK_DEFINE_F(BenchmarkFixture, AprioriBenchmark)(benchmark::State &state) {
    for (auto _: state) {
        rules::apriori::apriori_algorithm(get_transactions(), get_min_support());
    }
}

BENCHMARK_DEFINE_F(BenchmarkFixture, FPGrowthBenchmark)(benchmark::State &state) {
    for (auto _: state) {
        rules::fp_growth::fp_growth_algorithm(get_transactions(), get_min_support());
    }
}

BENCHMARK_DEFINE_F(BenchmarkFixture, EclatBenchmark)(benchmark::State &state) {
    for (auto _: state) {
        rules::eclat::eclat_algorithm(get_transactions(), get_min_support());
    }
}

BENCHMARK_DEFINE_F(BenchmarkFixture, RelimBenchmark)(benchmark::State &state) {
    for (auto _: state) {
        rules::relim::relim_algorithm(get_transactions(), get_min_support());
    }
}

BENCHMARK_REGISTER_F(BenchmarkFixture, AprioriBenchmark)
        ->Unit(benchmark::kMillisecond)
        ->MinTime(1.0);

BENCHMARK_REGISTER_F(BenchmarkFixture, FPGrowthBenchmark)
        ->Unit(benchmark::kMillisecond)
        ->MinTime(1.0);

BENCHMARK_REGISTER_F(BenchmarkFixture, EclatBenchmark)
        ->Unit(benchmark::kMillisecond)
        ->MinTime(1.0);

BENCHMARK_REGISTER_F(BenchmarkFixture, RelimBenchmark)
        ->Unit(benchmark::kMillisecond)
        ->MinTime(1.0);

BENCHMARK_MAIN();
