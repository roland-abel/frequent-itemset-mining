import csv
from apyori import apriori

# Funktion zum Laden der CSV-Datei
def load_transactions_from_csv(csv_file_path):
    transactions = []
    with open(csv_file_path, 'r', newline='', encoding='utf-8') as file:
        reader = csv.reader(file)
        for row in reader:
            transactions.append(row)  # Jede Zeile ist eine Transaktion
    return transactions

# Funktion zur Anwendung des Apriori-Algorithmus
def find_frequent_itemsets(transactions, min_support=0.5):
    # Finde die häufigen Itemsets ohne Assoziationsregeln (nur Support)
    frequent_itemsets = apriori(transactions, min_support=min_support)
    return list(frequent_itemsets)

# Funktion zum Speichern der Ergebnisse in einer CSV-Datei
def save_frequent_itemsets_to_csv(frequent_itemsets, output_csv_path):
    seen_itemsets = set()  # Set, um doppelte Itemsets zu vermeiden
    with open(output_csv_path, 'w', newline='', encoding='utf-8') as file:
        writer = csv.writer(file)
        # Schreibe die Header-Zeile
        writer.writerow(['Itemsets', 'Support'])

        for itemset in frequent_itemsets:
            items = frozenset(itemset.items)  # Frozenset wird verwendet, um Duplikate zu vermeiden
            if items not in seen_itemsets:
                seen_itemsets.add(items)
                support = itemset.support
                # Schreibe das Itemset und den Support
                writer.writerow([', '.join(items), support])

# Hauptteil des Programms
def main():
    input_csv = 'data_01.csv'  # Pfad zur Eingabedatei
    output_csv = 'result_01.csv'  # Pfad zur Ausgabedatei

    # Lade die Transaktionen aus der CSV-Datei
    transactions = load_transactions_from_csv(input_csv)

    # Finde die häufigen Itemsets
    frequent_itemsets = find_frequent_itemsets(transactions, min_support=0.6)

    # Speichere die Ergebnisse in eine CSV-Datei
    save_frequent_itemsets_to_csv(frequent_itemsets, output_csv)

if __name__ == "__main__":
    main()
