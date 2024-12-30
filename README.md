# Frequent Itemset Mining

This repository provides C++ implementations of popular frequent itemset mining algorithms:
**Apriori**, **FP-Growth**, **ECLAT**, and **RElim**. The goal of this project is to provide
solutions for mining frequent patterns from datasets in the context of association
rule learning.

The repository includes a suite of Google Unit Tests, a benchmarking tool to evaluate
algorithm performance, and a command-line interface (CLI) for easy usage.

## 1. Introduction

Frequent itemset mining is a crucial problem in data mining, used to discover
associations between items in large datasets. This repository offers
implementations of four well-known algorithms, each with its own strengths
and weaknesses.

## 2. Algorithms

### 2.1 Apriori Algorithm

The Apriori algorithm is one of the most popular algorithms for mining
frequent itemsets. It uses a level-wise search strategy where it generates
candidate itemsets of length $k$ from itemsets of length $k-1$, pruning the search
space based on the minimum support threshold.

### 2.2 FP-Growth Algorithm

FP-Growth (Frequent Pattern Growth) is a more efficient algorithm
compared to Apriori. It uses a compact tree structure (FP-tree) to represent
the dataset and avoid the need for candidate generation.

### 2.3 ECLAT Algorithm

ECLAT (Equivalence Class Transformation) is another algorithm used for
frequent itemset mining. It works by converting the problem into set
intersections, efficiently finding frequent itemsets through vertical
data representation.

### 2.4 RElim Algorithm

RElim (Recursive Elimination of Minimum Support) is an algorithm for frequent itemset mining. 
It recursively eliminates itemsets that do not meet the minimum support threshold, 
reducing the search space and improving efficiency. 
Unlike the FP-Growth algorithm, RElim uses a simpler data structure.

## 3. Command-Line Interface (CLI)

The repository includes a simple CLI for interacting with the algorithms and
running tests. The following table shows the available commands:

| Command             | Description                                                                                  |
|---------------------|----------------------------------------------------------------------------------------------|
| `--help`            | Show a help text.                                                                            |
| `--override`        | If set, the output file is overwritten if it already exists.                                 |
| `-i, --input`       | Path to the input file containing the database.                                              |
| `-o, --output`      | Path to the output file where the frequent itemsets will be saved.                           |
| `-s, --min-support` | Minimum support threshold for the frequent itemsets.                                         |
| `-a, --algorithm`   | Specifies which algorithm to use. Valid values are `apriori`, `fp-growth`, `eclat`, `relim`. |

### Example usage:

```
fim --input data_01.csv --output result_relim_01.csv --min-support 0.6 --algorithm relim --override
```

## 4. Conclusion

This repository provides implementations of popular frequent item set mining algorithms in C++.
You can test the algorithms on your own datasets using the CLI and measure the performance using the benchmarking tools.

We hope that this project will be useful for both learning purposes and practical applications in data mining.

## Author

Roland Abel

## License

This software is available under the following licenses:

See [MIT License (MIT)](LICENSE)
