from controller import Robot
import math
import numpy as np
# from matplotlib import pyplot as plt

# Constantes
MOTOR_BASE_SPEED = 1.0

def fuzzification(entre):
    # loin - moyen - proche
    fuzzi_var = [loin(entre), moyen(entre), proche(entre)]
    return fuzzi_var

def proche(x):
    return 1/(1 + np.exp(0.08 * (-x + 160)))

def loin(x):
    return 1/(1 + np.exp(0.2 * (x - 65)))

def moyen(x):
    return (1/(0.4 * np.sqrt(2 * np.pi))) * np.exp(-0.0002 * ((x - 100)/0.4)**2)

def inference_vitesse(fuzzi_gauche, fuzzi_droite):
    # les trois règles
    # lent - moyen - rapide
    result_array = []

    #Regle 1 :
    result_array.append(max(fuzzi_gauche[2], fuzzi_droite[2]))
    #Regle 2 :
    result_array.append(min(fuzzi_gauche[1], fuzzi_droite[1]))
    #Regle 3:
    result_array.append(min(fuzzi_gauche[0], fuzzi_droite[0]))

    return result_array


def inference_angle(fuzzi_gauche, fuzzi_droite):
    # les trois règles d'angle
    # gauche - droit - droite
    resultat_inference = []

    # Règle 1:
    regle_1_gauche = max(fuzzi_gauche[0], fuzzi_droite[2])
    resultat_inference.append(regle_1_gauche)

    # Règle 2:
    regle_2_droit = min(fuzzi_gauche[1], fuzzi_droite[0])
    resultat_inference.append(regle_2_droit)

    # Règle 3:
    regle_3_droite = min(fuzzi_gauche[2], fuzzi_droite[0])
    resultat_inference.append(regle_3_droite)
    return resultat_inference


def point_function(funct, x_values, degree_ver):
    func_cut = []
    for x in x_values:
        func_cut.append(min(funct(x), degree_ver))
    return func_cut


def aggreg_veracity_func(function_values):
    final_values = []
    for i in range(len(function_values[0])):
        final_values.append(max(function_values[0][i], function_values[1][i], function_values[2][i]))
    return final_values


def centroid_discrete(x, y):
    """
    Calcule les coordonnées du centroïde d'une forme définie par des points discrets.

    :param x: Liste des abscisses (x[i]).
    :param y: Liste des ordonnées correspondantes (y[i]).
    :return: Tuple (x_c, y_c) des coordonnées du centroïde.
    """
    if len(x) != len(y):
        raise ValueError("Les listes x et y doivent avoir la même longueur.")

    n = len(x)
    area = 0
    x_moment = 0
    y_moment = 0

    for i in range(n - 1):
        # Différence entre deux points consécutifs
        dx = x[i + 1] - x[i]
        # Moyenne des hauteurs des trapèzes
        avg_y = (y[i] + y[i + 1]) / 2
        # Contribution à l'aire
        area += avg_y * dx
        # Contribution au moment en x
        x_moment += ((x[i] + x[i + 1]) / 2) * avg_y * dx
        # Contribution au moment en y
        y_moment += ((y[i] ** 2 + y[i] * y[i + 1] + y[i + 1] ** 2) / 3) * dx

    # Calcul des coordonnées du centroïde
    x_c = x_moment / area
    y_c = y_moment / (2 * area)

    return x_c, y_c


def gauche_angle(x):
    return 1/(1 + np.exp((x + 20) * 0.1))


def droit_angle(x):
    return (1/(0.4 * np.sqrt(2 * np.pi))) * np.exp(-0.00018 * (x/0.2)**2)


def droite_angle(x):
    return 1/(1 + np.exp((-x + 20) * 0.1))


def lente_vitesse(x):
    return 1/(1 + np.exp((x - 3) * 12))


def normal_vitesse(x):
    if 2.5 <= x < 3:
        return (x - 2.5)/0.5
    elif 3 <= x < 4:
        return 1
    elif 4 <= x < 4.5:
        return (4.5 - x)/0.5
    else:
        return 0


def vite_vitesse(x):
    return (1/(0.4 * np.sqrt(2 * np.pi))) * np.exp(-0.1 * (x-4.5/0.2)**2)


def defuzzification(resultat_inference, x_values, funct1, funct2, func3, name_png):

    funct_values_1 = point_function(funct1, x_values, resultat_inference[0])
    funct_values_2 = point_function(funct2, x_values, resultat_inference[1])
    funct_values_3 = point_function(func3, x_values, resultat_inference[2])

    print(len(funct_values_1) == len(funct_values_2) == len(funct_values_3) == len(x_values))

    final_function = aggreg_veracity_func([funct_values_1, funct_values_2, funct_values_3])

    print(len(final_function) == len(x_values))

    x_c, y_c = centroid_discrete(x_values, final_function)
    """
    # Affichage avec Matplotlib
    plt.figure(figsize=(8, 6))

    # Dessiner la forme (trapèzes)
    plt.fill_between(x_values, final_function, color="skyblue", alpha=0.5, label="Forme")
    plt.plot(x_values, final_function, color="blue", linestyle="--", marker="o", label="Contour")
    plt.scatter(x_c, y_c, color="red", label="Centroïde", zorder=5)
    plt.text(x_c, y_c, f"  ({x_c:.2f}, {y_c:.2f})", color="red", fontsize=10)

    # Configuration du graphique
    plt.title("Visualisation de la Forme et du Centroïde")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.axhline(0, color="black", linewidth=0.5, linestyle="--")
    plt.axvline(0, color="black", linewidth=0.5, linestyle="--")
    plt.legend()
    plt.grid(alpha=0.3)
    plt.savefig(name_png)
    """
    return x_c


def calculer_vitesse_roues_correction(angle, vitesse):
    # Limiter l'angle entre -45 et 45
    angle = max(min(angle, 45), -45)

    # Calcul corrigé des vitesses des roues gauche et droite
    vitesse_gauche = min(vitesse * (1 + angle / 45), 6.0)
    vitesse_droite = min(vitesse * (1 - angle / 45), 6.0)

    return vitesse_gauche, vitesse_droite

# Initialisation des actionneurs et capteurs
robot = Robot()
timestep = int(robot.getBasicTimeStep())
motor_l = robot.getDevice('left wheel motor')
motor_r = robot.getDevice('right wheel motor')
motor_l.setPosition(float('inf'))
motor_r.setPosition(float('inf'))
motor_l.setVelocity(0)
motor_r.setVelocity(0)

# Initialisation des capteurs de distance
ds = []
num_captors = [1, 2, 5, 6]  # Capteurs sélectionnés
for i in range(len(num_captors)):
    s_name = 'ps{:d}'.format(num_captors[i])
    ds.append(robot.getDevice(s_name))
    ds[i].enable(timestep)

# Boucle principale
while robot.step(timestep) != -1:
    # Lire les valeurs des capteurs
    ds_r_reading_1 = ds[0].getValue()
    ds_r_reading_2 = ds[1].getValue()
    ds_l_reading_5 = ds[2].getValue()
    ds_l_reading_6 = ds[3].getValue()
    
    ds_r = (ds_r_reading_1 + ds_r_reading_2)/2
    ds_l = (ds_l_reading_5 + ds_l_reading_6)/2
    
    x_angle = np.linspace(-45, 45, 500)
    x_vitesse = np.linspace(0, 6, 100)

    fuzzication_gauche = fuzzification(ds_l)
    fuzzication_droite = fuzzification(ds_r)

    print(f"fuzzi gauche = {fuzzication_gauche}")
    print(f"fuzzi droite = {fuzzication_droite}")

    inf_angle = inference_angle(fuzzication_gauche, fuzzication_droite)
    inf_vitesse = inference_vitesse(fuzzication_gauche, fuzzication_droite)

    print(f"Inference angle = {inf_angle}")
    print(f"Inference vitesse = {inf_vitesse}")


    vitesse = defuzzification(inf_vitesse, x_vitesse, lente_vitesse, normal_vitesse, vite_vitesse, "vitesse_forme.png")
    print(f"Vitesse = {vitesse}")
    angle = defuzzification(inf_angle, x_angle, gauche_angle, droit_angle, droite_angle, "angle_forme.png")
    print(f"Angle = {angle}")

    roue_gauche, roue_droite = calculer_vitesse_roues_correction(angle, vitesse)
    print(f"Roue gauche = {roue_gauche}")
    print(f"Roue droite = {roue_droite}")
    
    # Appliquer les vitesses aux moteurs
    motor_l.setVelocity(roue_gauche)
    motor_r.setVelocity(roue_droite)