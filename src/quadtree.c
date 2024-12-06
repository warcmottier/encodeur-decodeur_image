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
void rempliQuadtreePGM(int tailleImage , int x , int y, unsigned char* image, TabQuadtree *tabQuadtree, int index){
    if(tailleImage == 1){
        tabQuadtree->noeuds[index]->m = image[y * tailleImage + x];
        return;
    }

    int moitierTaille = tailleImage / 2;

    tabQuadtree->tailleTable += 4;

    rempliQuadtreePGM(moitierTaille, x, y, image, tabQuadtree, 4 * index + 1); // enfant haut gauche
    rempliQuadtreePGM(moitierTaille, x, y + moitierTaille, image, tabQuadtree, 4 * index + 2); // enfant haut droite
    rempliQuadtreePGM(moitierTaille, x + moitierTaille, y, image, tabQuadtree, 4 * index + 2); // enfant bas droite
    rempliQuadtreePGM(moitierTaille, x + moitierTaille, y + moitierTaille, image, tabQuadtree, 4 * index + 2); // enfant bas gauche

    tabQuadtree->noeuds[index]->m = (tabQuadtree->noeuds[4 * index + 1]->m + tabQuadtree->noeuds[4 * index + 2]->m + tabQuadtree->noeuds[4 * index + 3]->m + tabQuadtree->noeuds[4 * index + 4]->m) / 4;
    
    tabQuadtree->noeuds[index]->epsilon = (tabQuadtree->noeuds[4 * index + 1]->m + tabQuadtree->noeuds[4 * index + 2]->m + tabQuadtree->noeuds[4 * index + 3]->m + tabQuadtree->noeuds[4 * index + 4]->m) % 4;

    if(tabQuadtree->noeuds[index]->epsilon != 0 || tabQuadtree->noeuds[4 * index + 1]->m || tabQuadtree->noeuds[4 * index + 2]->m || tabQuadtree->noeuds[4 * index + 3]->m || tabQuadtree->noeuds[4 * index + 4]->m)
        tabQuadtree->noeuds[index]->u = 0;
}

TabQuadtree constructeurQuadtreePGM(int tailleImage, unsigned char* image, int profondeur){
    TabQuadtree tree = initQuadtree(profondeur + 1);
    tree.tailleTable++;
    rempliQuadtreePGM(tailleImage, 0, 0, image, &tree, 0);
    return tree;
}
