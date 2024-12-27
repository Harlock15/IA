//
// Created by boche on 09/11/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define NB_LIGNES 10
#define NB_COLONNES 10
#define INFINI 10000

/*#define DEBUG*/


typedef struct pion_s
{
    int couleur;
    int valeur;
}Pion;

Pion *plateauDeJeu;

void f_affiche_plateau(Pion *plateau);
int f_convert_char2int(char c);
char f_convert_int2char(int i);
int f_eval(Pion *jeu, int joueur);
Pion *f_init_plateau();
void f_copie_plateau(Pion *source, Pion *destination);
Pion *f_raz_plateau();
int f_bouge_piece(Pion *plateau, int l1, int c1, int l2, int c2, int couleur);
int f_test_mouvement(Pion *plateau, int l1, int c1, int l2, int c2, int couleur);
double f_max(int profondeur, Pion *jeu, int joueur, int *nombre_noeud);
double f_min(int profondeur, Pion *jeu, int joueur, int *nombre_noeud);

int f_convert_char2int(char c)
{
#ifdef DEBUG
    printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
    if(c>='A' && c<='Z')
        return (int)(c-'A');
    if(c>='a' && c<='z')
        return (int)(c-'a');
    return -1;
}

char f_convert_int2char(int i)
{
#ifdef DEBUG
    printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

    return (char)i+'A';
}

Pion *f_init_plateau()
{
    int i, j;
    Pion *plateau=NULL;


#ifdef DEBUG
    printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

    plateau = (Pion *)malloc(NB_LIGNES*NB_COLONNES*sizeof(Pion));
    if(plateau == NULL)
    {
        printf("error: unable to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    for(i=0; i<NB_LIGNES; i++)
    {
        for(j=0; j<NB_COLONNES; j++)
        {
            plateau[i*NB_COLONNES+j].couleur = 0;
            plateau[i*NB_COLONNES+j].valeur = 0;
        }
    }

    plateau[9*NB_COLONNES+5].couleur = 1;
    plateau[9*NB_COLONNES+5].valeur = 1;

    plateau[9*NB_COLONNES+6].couleur = 1;
    plateau[9*NB_COLONNES+6].valeur = 2;

    plateau[9*NB_COLONNES+7].couleur = 1;
    plateau[9*NB_COLONNES+7].valeur = 3;

    plateau[9*NB_COLONNES+8].couleur = 1;
    plateau[9*NB_COLONNES+8].valeur = 2;

    plateau[9*NB_COLONNES+9].couleur = 1;
    plateau[9*NB_COLONNES+9].valeur = 1;

    plateau[8*NB_COLONNES+0].couleur = 1;
    plateau[8*NB_COLONNES+0].valeur = 1;

    plateau[8*NB_COLONNES+1].couleur = 1;
    plateau[8*NB_COLONNES+1].valeur = 3;

    plateau[8*NB_COLONNES+2].couleur = 1;
    plateau[8*NB_COLONNES+2].valeur = 3;

    plateau[8*NB_COLONNES+3].couleur = 1;
    plateau[8*NB_COLONNES+3].valeur = 1;

    plateau[8*NB_COLONNES+6].couleur = 1;
    plateau[8*NB_COLONNES+6].valeur = 1;

    plateau[8*NB_COLONNES+7].couleur = 1;
    plateau[8*NB_COLONNES+7].valeur = 1;

    plateau[8*NB_COLONNES+8].couleur = 1;
    plateau[8*NB_COLONNES+8].valeur = 1;

    plateau[7*NB_COLONNES+1].couleur = 1;
    plateau[7*NB_COLONNES+1].valeur = 1;

    plateau[7*NB_COLONNES+2].couleur = 1;
    plateau[7*NB_COLONNES+2].valeur = 1;

    plateau[2*NB_COLONNES+7].couleur = -1;
    plateau[2*NB_COLONNES+7].valeur = 1;

    plateau[2*NB_COLONNES+8].couleur = -1;
    plateau[2*NB_COLONNES+8].valeur = 1;

    plateau[1*NB_COLONNES+1].couleur = -1;
    plateau[1*NB_COLONNES+1].valeur = 1;

    plateau[1*NB_COLONNES+2].couleur = -1;
    plateau[1*NB_COLONNES+2].valeur = 1;

    plateau[1*NB_COLONNES+3].couleur = -1;
    plateau[1*NB_COLONNES+3].valeur = 1;

    plateau[1*NB_COLONNES+6].couleur = -1;
    plateau[1*NB_COLONNES+6].valeur = 1;

    plateau[1*NB_COLONNES+7].couleur = -1;
    plateau[1*NB_COLONNES+7].valeur = 3;

    plateau[1*NB_COLONNES+8].couleur = -1;
    plateau[1*NB_COLONNES+8].valeur = 3;

    plateau[1*NB_COLONNES+9].couleur = -1;
    plateau[1*NB_COLONNES+9].valeur = 1;

    plateau[0*NB_COLONNES+0].couleur = -1;
    plateau[0*NB_COLONNES+0].valeur = 1;

    plateau[0*NB_COLONNES+1].couleur = -1;
    plateau[0*NB_COLONNES+1].valeur = 2;

    plateau[0*NB_COLONNES+2].couleur = -1;
    plateau[0*NB_COLONNES+2].valeur = 3;

    plateau[0*NB_COLONNES+3].couleur = -1;
    plateau[0*NB_COLONNES+3].valeur = 2;

    plateau[0*NB_COLONNES+4].couleur = -1;
    plateau[0*NB_COLONNES+4].valeur = 1;

#ifdef DEBUG
    printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif

    return plateau;
}

void f_affiche_plateau(Pion *plateau)
{
    int i,j,k;


#ifdef DEBUG
    printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

    printf("\n    ");
    for(k=0; k<NB_COLONNES; k++)
        printf("%2c ",f_convert_int2char(k));
    printf("\n    ");
    for(k=0; k<NB_COLONNES; k++)
        printf("-- ");
    printf("\n");
    for(i=NB_LIGNES-1; i>=0; i--)
    {
        printf("%2d ", i);
        for(j=0; j<NB_COLONNES; j++)
        {
            printf("|");
            switch(plateau[i*NB_COLONNES+j].couleur)
            {
                case -1:
                    printf("%do",plateau[i*NB_COLONNES+j].valeur);
                    break;
                case 1:
                    printf("%dx",plateau[i*NB_COLONNES+j].valeur);
                    break;
                default:
                    printf("  ");
            }
        }
        printf("|\n    ");
        for(k=0; k<NB_COLONNES; k++)
            printf("-- ");
        printf("\n");
    }
    printf("    ");

#ifdef DEBUG
    printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
}


int f_gagnant()
{
    int i, j, somme1=0, somme2=0;


#ifdef DEBUG
    printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

    //Quelqu'un est-il arrive sur la ligne de l'autre
    for(i=0; i<NB_COLONNES; i++)
    {
        if(plateauDeJeu[i].couleur == 1)
            return 1;
        if(plateauDeJeu[(NB_LIGNES-1)*NB_COLONNES+i].couleur == -1)
            return -1;
    }

    //taille des armees
    for(i=0; i<NB_LIGNES; i++)
    {
        for(j=0; j<NB_COLONNES; j++)
        {
            if(plateauDeJeu[i*NB_COLONNES+j].couleur == 1)
                somme1++;
            if(plateauDeJeu[i*NB_COLONNES+j].couleur == -1)
                somme2++;
        }
    }
    if(somme1==0)
        return -1;
    if(somme2==0)
        return 1;

#ifdef DEBUG
    printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
    return 0;
}


/**
 * Prend comme argument la ligne et la colonne de la case
 * 	pour laquelle la bataille a lieu
 * Renvoie le couleur du gagnant
 * */
int f_bataille(int l, int c)
{
    int i, j, mini, maxi, minj, maxj;
    int somme=0;

#ifdef DEBUG
    printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
    mini = l-1<0?0:l-1;
    maxi = l+1>NB_LIGNES-1?NB_LIGNES-1:l+1;
    minj = c-1<0?0:c-1;
    maxj = c+1>NB_COLONNES-1?NB_COLONNES-1:c+1;

    for(i=mini; i<=maxi; i++)
    {
        for(j=minj; j<=maxj; j++)
        {
            somme += plateauDeJeu[i*NB_COLONNES+j].couleur*plateauDeJeu[i*NB_COLONNES+j].valeur;
        }
    }
    somme -= plateauDeJeu[l*NB_COLONNES+c].couleur*plateauDeJeu[l*NB_COLONNES+c].valeur;

#ifdef DEBUG
    printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
    if(somme < 0)
        return -1;
    if(somme > 0)
        return 1;

    return plateauDeJeu[l*NB_COLONNES+c].couleur;
}


/**
 * Prend la ligne et colonne de la case d'origine
 * 	et la ligne et colonne de la case de destination
 * Renvoie 1 en cas d'erreur
 * Renvoie 0 sinon
 * */
int f_test_mouvement(Pion *plateau, int l1, int c1, int l2, int c2, int couleur)
{
#ifdef DEBUG
    printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
    printf("de (%d,%d) vers (%d,%d)\n", l1, c1, l2, c2);
#endif
    /* Erreur, hors du plateau */
    if(l1 < 0 || l1 >= NB_LIGNES || l2 < 0 || l2 >= NB_LIGNES ||
       c1 < 0 || c1 >= NB_COLONNES || c2 < 0 || c2 >= NB_COLONNES)
        return 1;
    /* Erreur, il n'y a pas de pion a deplacer ou le pion n'appartient pas au joueur*/
    if(plateau[l1*NB_COLONNES+c1].valeur == 0 || plateau[l1*NB_COLONNES+c1].couleur != couleur)
        return 1;
    /* Erreur, tentative de tir fratricide */
    if(plateau[l2*NB_COLONNES+c2].couleur == plateau[l1*NB_COLONNES+c1].couleur)
        return 1;

    if(l1-l2 >1 || l2-l1 >1 || c1-c2 >1 || c2-c1 >1 || (l1==l2 && c1==c2))
        return 1;
#ifdef DEBUG
    printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
    return 0;
}


/**
 * Prend la ligne et colonne de la case d'origine
 * 	et la ligne et colonne de la case de destination
 *  et effectue le trantement de l'operation demandée
 * Renvoie 1 en cas d'erreur
 * Renvoie 0 sinon
 * */
int f_bouge_piece(Pion *plateau, int l1, int c1, int l2, int c2, int couleur)
{
    int gagnant=0;


#ifdef DEBUG
    printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

    if(f_test_mouvement(plateau, l1, c1, l2, c2, couleur) != 0)
        return 1;


    /* Cas ou il n'y a personne a l'arrivee */
    if(plateau[l2*NB_COLONNES+c2].valeur == 0)
    {
        plateau[l2*NB_COLONNES+c2].couleur = plateau[l1*NB_COLONNES+c1].couleur;
        plateau[l2*NB_COLONNES+c2].valeur = plateau[l1*NB_COLONNES+c1].valeur;
        plateau[l1*NB_COLONNES+c1].couleur = 0;
        plateau[l1*NB_COLONNES+c1].valeur = 0;
    }
    else
    {
        gagnant=f_bataille(l2, c2);
        /* victoire */
        if(gagnant == couleur)
        {
            plateau[l2*NB_COLONNES+c2].couleur = plateau[l1*NB_COLONNES+c1].couleur;
            plateau[l2*NB_COLONNES+c2].valeur = plateau[l1*NB_COLONNES+c1].valeur;
            plateau[l1*NB_COLONNES+c1].couleur = 0;
            plateau[l1*NB_COLONNES+c1].valeur = 0;
        }
            /* defaite */
        else if(gagnant != 0)
        {
            plateau[l1*NB_COLONNES+c1].couleur = 0;
            plateau[l1*NB_COLONNES+c1].valeur = 0;
        }
    }

#ifdef DEBUG
    printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
    return 0;
}

//Calcul du nombre de pions sur le plateau du joueur
int f_nbPions(Pion* jeu, int joueur)
{
    int nbPion=0;
    int i, j;
    for (i = 0; i < NB_COLONNES; ++i)
    {
        for (j = 0; j < NB_LIGNES; ++j)
        {
            if (jeu[i * NB_COLONNES + j].couleur == joueur)
            {
                ++nbPion;
            }
        }
    }
    return nbPion;
}

//Calcul de la valeur de tous les pions du joueur
int f_valeur(Pion* jeu, int joueur)
{
    int i, j;
    int valeur=0;
    for (i = 0; i < NB_COLONNES; ++i)
    {
        for (j = 0; j < NB_LIGNES; ++j)
        {
            if (jeu[i * NB_COLONNES + j].couleur == joueur)
            {
                valeur += jeu[i * NB_COLONNES + j].valeur;
            }
        }
    }
    return valeur;
}

int f_eval(Pion *jeu, int joueur) {
    int score = 0;
    int i, j;

    int nb_pions_1_sur_ligne_3 = 0;
    int pions_2_corrects = 0;
    int pions_3_corrects = 0;

    // Parcours du plateau pour évaluer la position de chaque pion du joueur
    for (i = 0; i < NB_LIGNES; i++) {
        for (j = 0; j < NB_COLONNES; j++) {
            Pion p = jeu[i * NB_COLONNES + j];

            if (p.couleur == joueur) {
                // Pions de ligne 3 : doivent être de valeur 1
                if (i == 2 && p.valeur == 1) {
                    nb_pions_1_sur_ligne_3++;
                } else if (i == 2 && p.valeur != 1) {
                    score -= 200; // Pénalité pour mauvais type de pion sur la ligne 3
                }

                // Pions de ligne 2 : valeur 2 à colonnes 3 et 7
                if (i == 1 && p.valeur == 2 && (j == 2 || j == 6)) {
                    pions_2_corrects++;
                } else if (i == 1 && p.valeur == 2 && (j != 2 && j != 6)) {
                    score -= 100; // Pénalité pour pion 2 mal placé
                }

                // Pions de valeur 3 aux colonnes 2, 5, et 8
                if (i == 1 && p.valeur == 3 && (j == 1 || j == 4 || j == 7)) {
                    pions_3_corrects++;
                } else if (i == 1 && p.valeur == 3 && (j != 1 && j != 4 && j != 7)) {
                    score -= 150; // Pénalité pour pion 3 mal placé
                }

                // Récompenser légèrement l'avancement (plus faible que la défense)
                if (joueur == 1) {
                    score += i * 5;
                } else if (joueur == -1) {
                    score += (NB_LIGNES - 1 - i) * 5;
                }
            }
        }
    }

    // Bonus pour la formation parfaite
    if (nb_pions_1_sur_ligne_3 == NB_COLONNES) {
        score += 1000;
    } else {
        score += nb_pions_1_sur_ligne_3 * 50;
    }

    if (pions_2_corrects == 2) {
        score += 500;
    } else {
        score += pions_2_corrects * 100;
    }

    if (pions_3_corrects == 3) {
        score += 750;
    } else {
        score += pions_3_corrects * 150;
    }

    return score;
}







//copie du plateau
void f_copie_plateau(Pion* source, Pion* destination)
{
    int i, j;
    for (i = 0; i < NB_LIGNES; i++)
    {
        for (j = 0; j < NB_COLONNES; j++)
        {
            destination[i * NB_COLONNES + j].couleur = source[i * NB_COLONNES + j].couleur;
            destination[i * NB_COLONNES + j].valeur = source[i * NB_COLONNES + j].valeur;
        }
    }
}

//mise a zero du plateau
Pion* f_raz_plateau()
{
    Pion* jeu = NULL;
    int i, j;
    jeu = (Pion *) malloc(NB_LIGNES * NB_COLONNES * sizeof (Pion));
    for (i = 0; i < NB_LIGNES; i++)
    {
        for (j = 0; j < NB_COLONNES; j++)
        {
            jeu[i * NB_COLONNES + j].couleur = 0;
            jeu[i * NB_COLONNES + j].valeur = 0;
        }
    }
    return jeu;
}

// Fonction min qui trouve le minimum des noeuds fils
double f_min(int profondeur, Pion* jeu, int joueur, int *nombre_noeud) {
    (*nombre_noeud)++;
    if (profondeur == 3) {
        return f_eval(jeu, joueur);
    } else {
        double valeur = 100000000000000;
        int i, j;
        for (i = 0; i < NB_LIGNES; i++) {
            for (j = 0; j < NB_COLONNES; j++) {
                if (jeu[i * NB_COLONNES + j].couleur == -joueur) {
                    // Essaye de déplacer le pion dans toutes les directions
                    for (int di = -1; di <= 1; di++) {
                        for (int dj = -1; dj <= 1; dj++) {
                            // Crée un plateau temporaire pour simuler les mouvements
                            Pion* plateauTemp = f_raz_plateau();
                            f_copie_plateau(jeu, plateauTemp);
                            int l2 = i + di, c2 = j + dj;
                            if (f_test_mouvement(plateauTemp, i, j, l2, c2, -joueur) == 0) {
                                f_bouge_piece(plateauTemp, i, j, l2, c2, -joueur);
                                double temp_valeur = f_max(profondeur + 1, plateauTemp, joueur, nombre_noeud);
                                valeur = (valeur < temp_valeur) ? valeur : temp_valeur;
                            }
                            free(plateauTemp);  // Libère la mémoire du plateau temporaire
                        }
                    }
                }
            }
        }
        return valeur;
    }
}

// Fonction max qui trouve le maximum des noeuds fils
double f_max(int profondeur, Pion* jeu, int joueur, int *nombre_noeud) {
    (*nombre_noeud)++;
    if (profondeur == 3) {
        return f_eval(jeu, joueur);  // Évaluation du plateau si on atteint la profondeur maximale
    } else {
        double valeur = -100000000000000;
        int i, j;
        for (i = 0; i < NB_LIGNES; i++) {
            for (j = 0; j < NB_COLONNES; j++) {
                if (jeu[i * NB_COLONNES + j].couleur == joueur) {

                    // Essaye de déplacer le pion dans toutes les directions
                    for (int di = -1; di <= 1; di++) {
                        for (int dj = -1; dj <= 1; dj++) {
                            // Crée un plateau temporaire pour simuler les mouvements
                            Pion* plateauTemp = f_raz_plateau();
                            f_copie_plateau(jeu, plateauTemp);
                            int l2 = i + di, c2 = j + dj;
                            if (f_test_mouvement(plateauTemp, i, j, l2, c2, joueur) == 0) {
                                f_bouge_piece(plateauTemp, i, j, l2, c2, joueur);
                                double temp_valeur = f_min(profondeur + 1, plateauTemp, joueur, nombre_noeud);
                                valeur = (valeur > temp_valeur) ? valeur : temp_valeur;
                            }
                            free(plateauTemp);  // Libère la mémoire du plateau temporaire
                        }
                    }
                }
            }
        }
        return valeur;
    }
}

// Fonction NegaMax pour trouver le meilleur coup
double f_negamax(int profondeur, Pion* jeu, int joueur, int joueur_evalue, int *nombre_noeud) {
    (*nombre_noeud)++;
    if (profondeur == 3) {
        return joueur * f_eval(jeu, joueur_evalue);  // Évaluation du plateau si on atteint la profondeur maximale
    } else {
        double valeur = -INFINI;  // Initialiser la valeur au minimum
        int i, j;

        for (i = 0; i < NB_LIGNES; i++) {
            for (j = 0; j < NB_COLONNES; j++) {
                if (jeu[i * NB_COLONNES + j].couleur == joueur) {
                    // Essaye de déplacer le pion dans toutes les directions
                    for (int di = -1; di <= 1; di++) {
                        for (int dj = -1; dj <= 1; dj++) {
                            int l2 = i + di, c2 = j + dj;
                            // Crée un plateau temporaire pour simuler les mouvements
                            Pion* plateauTemp = f_raz_plateau();
                            f_copie_plateau(jeu, plateauTemp);

                            if (f_test_mouvement(plateauTemp, i, j, l2, c2, joueur) == 0) {
                                f_bouge_piece(plateauTemp, i, j, l2, c2, joueur);
                                // Calcul récursif avec changement de signe pour le joueur opposé
                                double temp_valeur = -f_negamax(profondeur + 1, plateauTemp, -joueur, joueur_evalue, nombre_noeud);
                                valeur = (valeur > temp_valeur) ? valeur : temp_valeur;  // Prend le max
                            }
                            free(plateauTemp);  // Libère la mémoire du plateau temporaire
                        }
                    }
                }
            }
        }
        return valeur;
    }
}

int f_proximite_formation(int l1, int c1, int l2, int c2, Pion *jeu, int joueur) {

    // Définit la position des pions cibles pour le joueur 1
    if (joueur == 1) {
        if (l2 == 2 && jeu[l1 * NB_COLONNES + c1].valeur == 1) {
            return 100;  // Bonus pour amener un pion de valeur 1 sur la ligne 3
        }
        if (l2 == 1 && jeu[l1 * NB_COLONNES + c1].valeur == 2 && (c2 == 2 || c2 == 6)) {
            return 150;  // Bonus pour amener un pion de valeur 2 à la bonne colonne sur ligne 2
        }
        if (l2 == 1 && jeu[l1 * NB_COLONNES + c1].valeur == 3 && (c2 == 1 || c2 == 4 || c2 == 7)) {
            return 200;  // Bonus pour placer un pion de valeur 3 correctement
        }
    }
    return 0;  // Aucun bonus sinon
}


// Fonction NegaMax avec coupures Alpha-Bêta
double f_negamax_alpha_beta(int profondeur, Pion* jeu, int joueur, int joueur_evalue, double alpha, double beta, int *nombre_noeud) {
    (*nombre_noeud)++;
    if (profondeur == 3) {
        return joueur * f_eval(jeu, joueur_evalue);  // Évaluation du plateau si on atteint la profondeur maximale
    } else {
        double valeur = -INFINI;  // Initialiser la valeur au minimum
        int i, j;

        // Liste des coups possibles triés (en cas de tri)
        for (i = 0; i < NB_LIGNES; i++) {
            for (j = 0; j < NB_COLONNES; j++) {
                if (jeu[i * NB_COLONNES + j].couleur == joueur) {
                    // Essaye de déplacer le pion dans toutes les directions
                    for (int di = -1; di <= 1; di++) {
                        for (int dj = -1; dj <= 1; dj++) {
                            int l2 = i + di, c2 = j + dj;
                            Pion* plateauTemp = f_raz_plateau();
                            f_copie_plateau(jeu, plateauTemp);

                            // Teste le mouvement, si valide, continue la recherche
                            if (f_test_mouvement(plateauTemp, i, j, l2, c2, joueur) == 0) {
                                f_bouge_piece(plateauTemp, i, j, l2, c2, joueur);

                                // Appliquer bonus si le pion se rapproche de la formation défensive
                                int bonus_proximite = f_proximite_formation(i, j, l2, c2, jeu, joueur);

                                nombre_noeud++;
                                double temp_valeur = -f_negamax_alpha_beta(profondeur + 1, plateauTemp, -joueur, joueur_evalue, -beta, -alpha, nombre_noeud) + bonus_proximite;
                                //double temp_valeur = -f_negamax(profondeur + 1, plateauTemp, -joueur, joueur_evalue, -beta, -alpha, nombre_noeud) + bonus_proximite;

                                // Mise à jour de la meilleure valeur
                                valeur = (valeur > temp_valeur) ? valeur : temp_valeur;
                                alpha = (alpha > valeur) ? alpha : valeur;  // Met à jour alpha si nécessaire

                                // Coupure Alpha-Bêta
                                if (alpha >= beta) {
                                    free(plateauTemp);
                                    return valeur;  // Arrête la recherche
                                }
                            }
                            free(plateauTemp);  // Libère la mémoire du plateau temporaire
                        }
                    }
                }
            }
        }
        return valeur;
    }
}







/**
 * Calcule et joue le meilleur cout
 * */
void f_IA(int joueur) {
    int nombre_noeud = 0;
    double meilleurScore = -INFINI;
    int meilleurL1 = -1, meilleurC1 = -1, meilleurL2 = -1, meilleurC2 = -1;
    int i, j;

    // Parcours de toutes les cases du plateau
    for (i = 0; i < NB_LIGNES; i++) {
        for (j = 0; j < NB_COLONNES; j++) {
            if (plateauDeJeu[i * NB_COLONNES + j].couleur == joueur) {
                // Essaye de déplacer le pion dans toutes les directions
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {
                        int l2 = i + di, c2 = j + dj;

                        // Vérifie si le mouvement est valide
                        if (f_test_mouvement(plateauDeJeu, i, j, l2, c2, joueur) == 0) {
                            // Crée un plateau temporaire pour simuler le mouvement
                            Pion *plateauTemp = f_raz_plateau();
                            f_copie_plateau(plateauDeJeu, plateauTemp);
                            f_bouge_piece(plateauTemp, i, j, l2, c2, joueur);

                            // Appel à f_negamax_alpha_beta avec coupures Alpha-Beta
                            double score = -f_negamax_alpha_beta(1, plateauTemp, -joueur, joueur, -INFINI, INFINI, &nombre_noeud);
                            // double score = -f_negamax(1, plateauTemp, -joueur, joueur, &nombre_noeud);
                            // double score = f_min(1, plateauTemp, -joueur, &nombre_noeud);

                            // Mise à jour du meilleur score et des meilleurs mouvements
                            if (score > meilleurScore) {
                                meilleurScore = score;
                                meilleurL1 = i;
                                meilleurC1 = j;
                                meilleurL2 = l2;
                                meilleurC2 = c2;
                            } else if (score == meilleurScore) {
                                // Si les scores sont égaux, choisir aléatoirement parmi les meilleurs
                                if (rand() % 2 == 0) {
                                    meilleurL1 = i;
                                    meilleurC1 = j;
                                    meilleurL2 = l2;
                                    meilleurC2 = c2;
                                }
                            }

                            free(plateauTemp);  // Libère la mémoire du plateau temporaire
                        }
                    }
                }
            }
        }
    }

    // Exécute le meilleur mouvement trouvé
    if (meilleurL1 != -1 && meilleurC1 != -1 && meilleurL2 != -1 && meilleurC2 != -1) {
        f_bouge_piece(plateauDeJeu, meilleurL1, meilleurC1, meilleurL2, meilleurC2, joueur);
        printf("IA joue : l1 = %d, c1 = %d, l2 = %d, c2 = %d\n", meilleurL1, meilleurC1, meilleurL2, meilleurC2);
    }
    printf("Nombre de noeud = %d", nombre_noeud);
}




/**
 * Demande le choix du joueur humain et calcule le coup demande
 * */
void f_humain(int joueur)
{
    char c1, c2;
    char buffer[32];
    int l1, l2;


#ifdef DEBUG
    printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif

    printf("joueur ");
    switch(joueur)
    {
        case -1:
            printf("o ");
            break;
        case 1:
            printf("x ");
            break;
        default:
            printf("inconnu ");
    }
    printf("joue:\n");
    while(1)
    {
        fgets(buffer, 32, stdin);
        if(sscanf(buffer, "%c%i%c%i\n", &c1, &l1, &c2, &l2) == 4)
        {
            if(f_bouge_piece(plateauDeJeu, l1, f_convert_char2int(c1), l2, f_convert_char2int(c2), joueur) == 0)
                break;
        }
        fflush(stdin);




        printf("mauvais choix\n");
    }

#ifdef DEBUG
    printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
}

int main(int argv, char *argc[])
{
    int fin = 0,mode=0 , ret, joueur = 1;
    printf("1 humain vs IA\n2 humain vs humain\n3 IA vs IA\n");
    scanf("%d",&mode);

    plateauDeJeu = f_init_plateau();
    while (!fin)
    {
        f_affiche_plateau(plateauDeJeu);
        if(mode==1)
        {
            if(joueur>0)
                f_humain(joueur);
            else
                f_IA(joueur);
        }
        else if(mode==2)
        {
            f_humain(joueur);
        }
        else
        {
            f_IA(joueur);
        }

        if ((ret = f_gagnant()) != 0)
        {
            switch (ret)
            {
                case 1:
                    f_affiche_plateau(plateauDeJeu);
                    printf("joueur x gagne!\n");
                    fin = 1;
                    break;
                case -1:
                    f_affiche_plateau(plateauDeJeu);
                    printf("joueur o gagne!\n");
                    fin = 1;
                    break;
            }
        }
        joueur = -joueur;
    }

#ifdef DEBUG
    printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif

    return 0;
}

