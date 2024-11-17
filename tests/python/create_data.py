import csv
import random

def generate_frequent_itemsets(items_range, num_frequent_itemsets, max_size):
    """
    Generates a list of frequent itemsets with random items.

    :param items_range: The item's value range.
    :param num_frequent_itemsets: The number of frequent itemsets to generate.
    :param max_size: The maximum size of an itemset.
    :return: A list of unique frequent itemsets.
    """
    frequent_itemsets = set()

    while len(frequent_itemsets) < num_frequent_itemsets:
        size = random.randint(2, max_size)
        itemset = tuple(sorted(random.sample(items_range, size)))
        frequent_itemsets.add(itemset)

    return list(frequent_itemsets)

def generate_transactions(num_transactions, min_len, max_len, frequent_itemsets, items_range, min_support):
    """
    Generates a list of transactions with random lengths and itemsets.

    :param min_support: The minimal support value.
    :param num_transactions: The number of transactions to generate.
    :param min_len: The minimum length of a transaction.
    :param max_len: The maximum length of a transaction.
    :param frequent_itemsets: A list of frequent itemsets to potentially include in transactions.
    :param items_range: The range of possible items for random selection.
    :return: A list of transactions.
    """
    transactions = []

    for _ in range(num_transactions):
        trans_len = random.randint(min_len, max_len)
        transaction = []

        for itemset in frequent_itemsets:
            if len(transaction) + len(itemset) <= trans_len and random.random() < min_support:
                transaction.extend(itemset)

        while len(transaction) < trans_len:
            transaction.append(random.choice(items_range))

        random.shuffle(transaction)
        transactions.append(transaction)

    return transactions


def save_transactions_to_csv(transactions, filename):
    """
    Save a list of transactions to a CSV file.

    :param transactions: The list of transactions to save.
    :param filename: The name of the file where the transactions will be saved.
    """
    with open(filename, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerows(transactions)


def save_itemsets_to_file(frequent_itemsets, filename):
    """
    Save the list of frequent itemsets to a file.

    :param frequent_itemsets: The list of frequent itemsets to save.
    :param filename: The name of the file where the itemsets will be saved.
    """
    with open(filename, 'w') as file:
        for itemset in frequent_itemsets:
            file.write(','.join(map(str, itemset)) + '\n')

def main():
    # Parameters for generating frequent itemsets
    min_support = 0.8
    min_item = 1
    max_item = 100
    items_range = range(min_item, max_item + 1)

    max_size = 6
    num_frequent_itemsets = 10
    num_transactions = 5000

    min_len = 5
    max_len = 20

    # Generate frequent itemsets and transactions
    frequent_itemsets = generate_frequent_itemsets(items_range, num_frequent_itemsets, max_size)
    transactions = generate_transactions(num_transactions, min_len, max_len, frequent_itemsets, items_range, min_support)

    # Define the filename for saving the itemsets
    output_file = 'data_01.csv'

    # Save transactions to CSV
    save_transactions_to_csv(transactions, output_file)
    print(f"Generated {num_frequent_itemsets} frequent itemsets and saved them to {output_file}")

if __name__ == "__main__":
    main()
