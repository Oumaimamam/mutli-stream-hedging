import json
import matplotlib.pyplot as plt
from datetime import datetime

def plot_prices(file1_path, file2_path):
    # Charger les fichiers JSON
    with open(file1_path, 'r') as file1:
        data1 = json.load(file1)
    
    with open(file2_path, 'r') as file2:
        data2 = json.load(file2)
    
    # Extraire les dates et les prix
    dates1 = [datetime.strptime(entry['date'], '%Y-%m-%dT%H:%M:%S') for entry in data1]
    prices1 = [entry['price'] for entry in data1]

    dates2 = [datetime.strptime(entry['date'], '%Y-%m-%dT%H:%M:%S') for entry in data2]
    prices2 = [entry['price'] for entry in data2]
    
    # Tracer les courbes
    plt.figure(figsize=(10, 6))
    plt.plot(dates1, prices1, label='Fichier 1', marker='o')
    plt.plot(dates2, prices2, label='Fichier 2', marker='x')
    
    # Personnaliser le graphique
    plt.xlabel('Dates')
    plt.ylabel('Prices')
    plt.title('Comparison of Prices')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    
    # Afficher le graphique
    plt.show()

# Remplacez ces chemins par les chemins absolus de vos fichiers JSON
file1 = "C:\\Users\\oumai\\Downloads\\multi-stream-hedging-project\\Input\\Max_5_3_1\\max_5_3_1_portfolio.json"
file2 = "C:\\Users\\oumai\\Downloads\\multi-stream-hedging-project\\Output\\max5_3_1_output.json"

plot_prices(file1, file2)
