/// @file apriori_benchmark.cpp
/// @brief Benchmark test for the Apriori algorithm.
///
/// @author Roland Abel
/// @date September 21, 2024
///
/// Copyright (c) 2024 Roland Abel
///
/// This software is released under the MIT License.
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// with the Software without restriction, including without limitation the rights
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

#include "benchmark/benchmark.h"
#include "reader.h"
#include "apriori.h"
#include "utils.h"

using namespace std;
using namespace fim::algorithm::apriori;

static void apriori_benchmark(benchmark::State &state, const std::string_view &filename) {
    const auto db = data::read_csv(filename).value();
    const size_t min_support = get_min_support(state, db.size());

    for ([[maybe_unused]] auto _: state) {
        apriori_algorithm(db, min_support);
    }
}

BENCHMARK_CAPTURE(apriori_benchmark, "mushroom", "data/mushroom.dat")
        ->Arg(60)
        ->Arg(80)
        ->Arg(90)
        ->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(apriori_benchmark, "retail", "data/retail.dat")
        ->Arg(60)
        ->Arg(80)
        ->Arg(90)
        ->Unit(benchmark::kMillisecond);

#ifdef NDEBUG
BENCHMARK_CAPTURE(apriori_benchmark, "chess", "data/chess.dat")
        ->Arg(80)
        ->Arg(90)
        ->Unit(benchmark::kMillisecond);
#endif
