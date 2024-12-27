import random
from scipy.spatial import distance
import numpy as np
from PIL import Image
import matplotlib.pyplot as plt

# Variables
DEFAULT_WIDTH = 500
DEFAULT_HEIGHT = 500
NB_VILLE = 21

# Classe Point pour représenter les villes
class Point:
    def __init__(self, name="", x=0, y=0):
        self.name = name
        self.x = x
        self.y = y

# Liste de Points pour les villes
ville = []

# Fonction pour charger les villes depuis un fichier
def load_cities(filename):
    try:
        with open(filename, "r") as file:
            for line in file:
                parts = line.strip().split()
                if len(parts) == 3:
                    name = parts[0]
                    x = int(parts[1]) - 5  # Décalage pour ajuster la position
                    y = int(parts[2]) - 5
                    ville.append(Point(name, x, y))
        print("Villes chargées avec succès.")
    except FileNotFoundError:
        print(f"Erreur : le fichier '{filename}' est introuvable.")
    except Exception as e:
        print(f"Erreur lors du chargement des villes : {e}")

# Fonction pour lire une image PPM et retourner les données de l'image
def read_ppm(filename):
    try:
        img = Image.open(filename).convert("RGB")
        return np.array(img)
    except Exception as e:
        print(f"Erreur lors de la lecture du fichier PPM {filename} : {e}")
        return None

# Fonction pour afficher l'image avec les points des villes et les lignes
def display_map_with_cities(image_data, cities):
    if image_data is None:
        print("Erreur : les données de l'image sont manquantes.")
        return

    plt.figure(figsize=(12, 12))
    plt.imshow(image_data)
    plt.axis("off")  # Cache les axes

    x_coords = [city.x for city in cities]
    y_coords = [city.y for city in cities]
    plt.scatter(x_coords, y_coords, color="red", s=30, label="Villes")
    plt.plot(x_coords, y_coords, color="green", linewidth=1, label="Lignes entre villes")

    for city in cities:
        plt.text(city.x, city.y, city.name, fontsize=8, ha="right")

    plt.legend()
    plt.show()

# Définition des neurones
class Neuron:
    def __init__(self, i, j):
        self.weights = np.random.uniform(0, 200, 2)  # Poids initiaux (2D)
        self.i = i  # Indice de la position du neurone dans le réseau
        self.j = j

# Liste des données et des neurones
dataset = []
dataset_pop = []
neuron_set = [Neuron(i, 0) for i in range(20)]  # 20 neurones avec poids initiaux

# Fonction pour initialiser le dataset (points aléatoires)
def InitialiseSet():
    global dataset
    for i in range(20):
        x = random.randint(0, 200)
        y = random.randint(0, 200)
        dataset.append(Point(x=x, y=y))

# Fonction pour sélectionner aléatoirement une donnée
def SortData():
    random.shuffle(dataset)
    point = dataset.pop(0)
    dataset_pop.append(point)
    return np.array([point.x, point.y])

# Fonction pour calculer le potentiel (distance euclidienne) entre un vecteur d'entrée et un neurone
def calcul_potentiel(vecteur, neuron):
    return distance.euclidean(vecteur, neuron.weights)

# Calcul des activités basées sur les potentiels
def calcul_activite(potentiels):
    return [1 / (1 + pot) for pot in potentiels]

# Trouver le neurone gagnant (celui avec l'activité maximale)
def gagnant(liste_activite):
    return np.argmax(liste_activite)

# Mise à jour des poids des neurones (y compris les voisins)
def maj_poids(input_data, winner_index, learning_rate=0.1, lambda_coef=0.3, beta_coef=0.1):
    for i, neuron in enumerate(neuron_set):
        distance_topo = abs(i - winner_index)
        if distance_topo == 0:
            phi = 1  # Neurone gagnant
        elif distance_topo == 1:
            phi = lambda_coef  # Voisin proche
        elif distance_topo == 2:
            phi = -beta_coef  # Voisin éloigné
        else:
            phi = 0  # Neurone trop éloigné

        # Mise à jour des poids
        neuron.weights += learning_rate * phi * (input_data - neuron.weights)


def display_training_step(dataset, neurons):
    plt.figure(figsize=(12, 12))

    dataset_x = [point.x for point in dataset]
    dataset_y = [point.y for point in dataset]
    plt.scatter(dataset_x, dataset_y, color="red", s=30, label="Points du dataset")

    dataset_pop_x = [point.x for point in dataset_pop]
    dataset_pop_y = [point.y for point in dataset_pop]
    plt.scatter(dataset_pop_x, dataset_pop_y, color="red", s=30, label="Points du dataset")

    neuron_x = [neuron.weights[0] for neuron in neurons]
    neuron_y = [neuron.weights[1] for neuron in neurons]
    plt.scatter(neuron_x, neuron_y, color="blue", s=50, label="Neurones (Kohonen)")

    plt.plot(neuron_x, neuron_y, color="blue", linestyle='--', linewidth=1, label="Connexion des Neurones")

    plt.legend()

    plt.pause(0.1)
    plt.close()


# Fonction principale
def main():
    # Charger les villes et l'image
    load_cities("Villes_et_positions_dans_image.txt")
    img_data = read_ppm("carte_france_vierge.ppm")

    # Initialisation du dataset
    InitialiseSet()

    # Boucle principale d'apprentissage
    while True:
        if not dataset:
            break
        # Sélectionner une donnée aléatoirement
        input_data = SortData()

        # Calculer les potentiels pour chaque neurone
        potentiels = [calcul_potentiel(input_data, neuron) for neuron in neuron_set]

        # Calculer les activités des neurones
        activites = calcul_activite(potentiels)

        # Trouver le neurone gagnant
        winner_index = gagnant(activites)

        # Mettre à jour les poids des neurones
        maj_poids(input_data, winner_index)

        # Afficher l'état actuel des neurones pendant le training
        display_training_step(dataset, neuron_set)

    plt.show()  # Pour conserver l'affichage à la fin de l'apprentissage

if __name__ == "__main__":
    main()
