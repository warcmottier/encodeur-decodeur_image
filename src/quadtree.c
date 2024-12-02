#include "../include/quadtree.h"


/**
 * @brief initialise le Quadtree
 * 
 * @param profondeur 
 * @return Quadtree 
 */
TabQuadtree initQuadtree(int profondeur){
    TabQuadtree tree;
    tree.tailleTable = (pow(4, profondeur + 1) - 1) / 3;
    tree.noeuds = malloc(sizeof(Noeud) * tree.tailleTable);
    for (int i = 0; i < tree.tailleTable; i++){
        tree.noeuds[i]->hg = NULL;
        tree.noeuds[i]->hd = NULL;
        tree.noeuds[i]->bd = NULL;
        tree.noeuds[i]->bg = NULL;
        tree.noeuds[i]->epsilon = 0;
        tree.noeuds[i]->u = 1;
        tree.noeuds[i]->m = 0;
    }
    
    return tree;
}

/**
 * @brief cree un noeud deja allouer a l'initialisation
 * 
 * @param tabQuadtree 
 * @return Quadtree 
 */
Quadtree creeNoeud(TabQuadtree tabQuadtree){
    return tabQuadtree.noeuds[tabQuadtree.tailleTable];
}


/**
 * @brief rempli le quadtree a partir des pointeur et en recursive
 * 
 * @param tailleImage 
 * @param x 
 * @param y 
 * @param image 
 * @param tree 
 * @param tabQuadtree 
 */
void rempliQuadtreePGM(int tailleImage , int x , int y, unsigned char** image, Quadtree *tree, TabQuadtree *tabQuadtree){
    // si on est dans une feuille on arrete
    if (tailleImage == 1){
        (*tree)->m = image[x][y];
        return;
    }

    int moitierTaille = tailleImage / 2;

    //alloue les fils au noeud courant
    (*tree)->hg = creeNoeud(*tabQuadtree);
    tabQuadtree->tailleTable++;
    (*tree)->hd = creeNoeud(*tabQuadtree);
    tabQuadtree->tailleTable++;
    (*tree)->hg = creeNoeud(*tabQuadtree);
    tabQuadtree->tailleTable++;
    (*tree)->bd = creeNoeud(*tabQuadtree);
    tabQuadtree->tailleTable++;
    (*tree)->bg = creeNoeud(*tabQuadtree);
    tabQuadtree->tailleTable++;

    // on parcour le quadtree
    rempliQuadtreePGM(moitierTaille, x, y, image, &(*tree)->hg, tabQuadtree); // enfant haut gauche
    rempliQuadtreePGM(moitierTaille, x, y + moitierTaille, image, &(*tree)->hd, tabQuadtree); // enfant haut droite
    rempliQuadtreePGM(moitierTaille, x + moitierTaille, y, image, &(*tree)->hg, tabQuadtree); // enfant bas droite
    rempliQuadtreePGM(moitierTaille, x + moitierTaille, y + moitierTaille, image, &(*tree)->hg, tabQuadtree); // enfant bas gauche

    //on rempli le champ m qui est la moyenne des m ces fils
    (*tree)->m = ((*tree)->hg->m + (*tree)->hd->m + (*tree)->bd->m + (*tree)->bg->m) / 4;

    // on rempli le champ epsilon qui est le modulo de l'addition des m de ces fils
    (*tree)->epsilon = ((*tree)->hg->m + (*tree)->hd->m + (*tree)->bd->m + (*tree)->bg->m) % 4;

    // on met le champ u a 0 si l'erreur est different de 0 et si un de ces fils n'est pas uniforme
    if((*tree)->epsilon != 0 || (*tree)->hg->u != 1 || (*tree)->hd->u != 1 || (*tree)->bd->u != 1 || (*tree)->bg->u != 1)
        (*tree)->u = 0;
}

TabQuadtree constructeurQuadtreePGM(int tailleImage, unsigned char** image, int profondeur){
    TabQuadtree tree = initQuadtree(profondeur + 1);
    tree.tailleTable++;
    rempliQuadtreePGM(tailleImage, 0, 0, image, tree.noeuds, &tree);
    return tree;
}
