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

// Beispielalgorithmus
void SortAlgorithm(std::vector<int>& v) {
    std::sort(v.begin(), v.end());
}

// Benchmark-Funktion
static void BM_SortAlgorithm(benchmark::State& state) {
    std::vector<int> v(state.range(0));
    for (auto _ : state) {
        // Vorbereitung: Erzeuge zufällige Daten für den Test
        std::generate(v.begin(), v.end(), std::rand);

        // Zu benchmarkender Algorithmus
        SortAlgorithm(v);

        // Ein optionaler Rücksetzpunkt, wenn du den Zustand zwischen den Iterationen bereinigen musst.
        benchmark::ClobberMemory();
    }
}

// Registriere die Benchmark-Funktion mit verschiedenen Eingabegrößen
BENCHMARK(BM_SortAlgorithm)->Range(1 << 10, 1 << 18);
BENCHMARK(BM_SortAlgorithm)->Range(1 << 15, 1 << 25);


BENCHMARK_MAIN();
