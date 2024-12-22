"""
This script runs the Apriori algorithm to find frequent itemsets in transactions and saves the results in a CSV file.

Usage:
    python run_apriori.py <input_file> <output_file> <min_support>

Arguments:
    input_file: Path to the input CSV file containing the transactions.
    output_file: Path to the output CSV file where the frequent itemsets will be saved.
    min_support: Minimum support threshold for the Apriori algorithm.

Functions:
    load_transactions_from_csv: Loads transactions from a CSV file.
    find_frequent_itemsets: Finds frequent itemsets using the Apriori algorithm.
    save_frequent_itemsets_to_csv: Saves the frequent itemsets to a CSV file.
    main: Main function that controls the entire process.
"""

import csv
import sys
from typing import List

from apyori import apriori, RelationRecord


def load_transactions_from_csv(csv_file_path: str) -> List[List[str]]:
    """
    Load transactions from a CSV file.

    Parameters
    ----------
    csv_file_path : str
        The path to the input CSV file containing transactions.
        Each row in the file represents a transaction.

    Returns
    -------
    List[List[str]]
        A list of transactions, where each transaction is a list of items.
    """
    transactions = []

    with open(csv_file_path, 'r', newline='', encoding='utf-8') as file:
        reader = csv.reader(file)
        for row in reader:
            transactions.append(row)  # Each row is treated as a transaction

    return transactions


def find_frequent_itemsets(transactions: List[List[str]], min_support: float = 0.5) -> List[RelationRecord]:
    """
    Find frequent itemsets using the Apriori algorithm.

    Parameters
    ----------
    transactions : List[List[str]]
        A list of transactions where each transaction is a list of items.
    min_support : float, optional
        The minimum support threshold for an itemset to be considered frequent.
        Default is 0.5.

    Returns:
        List[RelationRecord]: A list of `apyori.RelationRecord` objects representing the frequent itemsets and their support.
    """
    # Find frequent itemsets without generating association rules
    frequent_itemsets = apriori(transactions, min_support=min_support)
    return list(frequent_itemsets)


def save_frequent_itemsets_to_csv(frequent_itemsets: List[RelationRecord], output_csv_path: str) -> None:
    """
    Save the frequent itemsets to a CSV file.

    The output CSV will include the following columns:
    - `length`: The number of items in the itemset.
    - `itemsets`: A string representation of the items in the itemset.
    - `support`: The support value of the itemset.

    Parameters
    ----------
    frequent_itemsets : List[RelationRecord]
        A list of `apyori.RelationRecord` objects representing the frequent itemsets.
    output_csv_path : str
        The path to the output CSV file.
    """
    seen_itemsets = set()

    with open(output_csv_path, 'w', newline='', encoding='utf-8') as file:
        writer = csv.writer(file, delimiter=';')
        writer.writerow(['length', 'itemsets', 'support'])

        for itemset in frequent_itemsets:
            items = frozenset(itemset.items)
            if items not in seen_itemsets:
                seen_itemsets.add(items)
                support = itemset.support
                length = len(items)
                writer.writerow([length, ' '.join(items), support])


def main() -> None:
    """
    Main function to run the Apriori algorithm and save the results.

    This function expects three command-line arguments:
    1. Path to the input CSV file containing transactions.
    2. Path to the output CSV file for saving frequent itemsets.
    3. Minimum support threshold for the Apriori algorithm.

    Example usage:
        python script.py input.csv output.csv 0.5
    """
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    min_support = float(sys.argv[3])

    # Load transactions from the CSV file
    transactions = load_transactions_from_csv(input_file)

    # Find the frequent itemsets
    frequent_itemsets = find_frequent_itemsets(transactions, min_support)

    # Save the results to a CSV file
    save_frequent_itemsets_to_csv(frequent_itemsets, output_file)


if __name__ == "__main__":
    main()
