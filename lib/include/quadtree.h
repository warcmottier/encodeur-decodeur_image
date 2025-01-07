#ifndef _QUADTREE_
#define _QUADTREE_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gereBit.h"

typedef struct noeud{
    unsigned char m; //moyenne de ses quatre fils
    unsigned char epsilon; //valeur d'erreur de l'extrapolation
    unsigned char u; // bit uniforme
    int affiche; // flag si je dois ecrire le noeud dans le qtc
    // struct noeud *hg, *hd, *bg, *bd; pointeur vers le carre cree par la subdivision
}Noeud;

typedef struct quadtree{
    Noeud *noeuds; // tableau de noeud representant mon tas d'ordre 4
    int tailleTable; // taille de la table de noeud
    int profondeurMax; // profondeur max du quadtree
}TabQuadtree;

/**
 * @brief cree et rempli le quadtree a partir du tableau de tableau image rempli des donner du fichier pgm
 * 
 * @param tailleImage 
 * @param image 
 * @param profondeur 
 * @param alpha
 * @return TabQuadtree 
 */
TabQuadtree constructeurQuadtreePGM(int tailleImage, unsigned char** image, int profondeur);

/**
 * @brief construit le quadtree en lisant le .qtc
 * 
 * @param profondeur 
 * @param quadtree 
 * @param f 
 * @param bitStream 
 */
void constructeurQuadtreeQTC(int profondeur, TabQuadtree* quadtree, FILE* f, BitStream bitStream);

/**
 * @brief affiche le quadtree
 * 
 * @param tab 
 */
void afficheQuadtree(TabQuadtree tab);

/**
 * @brief vide le quadtree
 * 
 */
void libereQuadtree(TabQuadtree* quadtree);

/**
 * @brief cree et rempli la grille
 * 
 * @param quadtree 
 * @param taille 
 * @return int** 
 */
int** creeGrille(TabQuadtree quadtree, int taille);

#endif
