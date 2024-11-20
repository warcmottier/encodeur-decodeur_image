#ifndef _QUADTREE_
#define _QUADTREE_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct noeud{
    unsigned char m; //moyenne de ses quatre fils
    int epsilon; //valeur d'erreur de l'extrapolation
    int u; // bit 
    struct noeud *hg, *hd, *bg, *bd; //pointeur vers le carre cree par la subdivision
}Noeud;

typedef struct quadtree{
    Noeud* noeuds; // tableau de noeud representant mon tas d'ordre 3
    int tailleTable; // taille de la table de noeud
    int profondeurMax; // profondeur max du quadtree
}Quadtree;

/**
 * @brief initialise le Quadtree
 * 
 * @param profondeur 
 * @return Quadtree 
 */
Quadtree initQuadtree(int profondeur);

#endif