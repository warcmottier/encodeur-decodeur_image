#include "../include/quadtree.h"

/**
 * @brief initialise le Quadtree
 * 
 * @param profondeur 
 * @return Quadtree 
 */
TabQuadtree initQuadtree(int profondeur){
    TabQuadtree tree;
    tree.tailleTable = (((int) pow(4, profondeur)) - 1) / 3;
    tree.noeuds = malloc(sizeof(Noeud) * tree.tailleTable);
    if(tree.noeuds == NULL)
        return tree;
    
    
    for (int i = 0; i < tree.tailleTable; i++){
        tree.noeuds[i].epsilon = 0;
        tree.noeuds[i].u = 1;
        tree.noeuds[i].m = 0;
        tree.noeuds[i].affiche = 1;
    }
    
    return tree;
}

/**
 * @brief cree un noeud deja allouer a l'initialisation
 * 
 * @param tabQuadtree 
 * @return Quadtree 
 */
Noeud creeNoeud(TabQuadtree tabQuadtree){
    return tabQuadtree.noeuds[tabQuadtree.tailleTable];
}

void flagAffiche(TabQuadtree* quadtree, int index){
    if(4 * index + 1 >= quadtree->tailleTable){
        quadtree->noeuds[index].affiche = 0;
        return;
    }

    quadtree->noeuds[index].affiche = 0;
    flagAffiche(quadtree, 4 * index + 1); //1er fils
    flagAffiche(quadtree, 4 * index + 2); //2eme fils
    flagAffiche(quadtree, 4 * index + 3); //3eme fils
    flagAffiche(quadtree, 4 * index + 4); //4eme fils

}

/**
 * @brief rempli le quadtree a partir des pointeur et en recursive
 * 
 * @param tailleImage 
 * @param x 
 * @param y 
 * @param image 
 * @param tree 
 * 
 */
void rempliQuadtreePGM(int tailleImage, int x, int y, unsigned char** image, TabQuadtree *tabQuadtree, int index) {
    // Cas de base : taille de l'image = 1
    if (tailleImage == 1) {
        tabQuadtree->noeuds[index].m = image[y][x];  // On affecte la valeur à m
        return;
    }

    int moitierTaille = tailleImage / 2;

    rempliQuadtreePGM(moitierTaille, x, y, image, tabQuadtree, 4 * index + 1); // enfant haut gauche
    rempliQuadtreePGM(moitierTaille, x + moitierTaille, y, image, tabQuadtree, 4 * index + 2); // enfant haut droit
    rempliQuadtreePGM(moitierTaille, x + moitierTaille, y + moitierTaille, image, tabQuadtree, 4 * index + 3); // enfant bas droit
    rempliQuadtreePGM(moitierTaille, x, y + moitierTaille, image, tabQuadtree, 4 * index + 4); // enfant bas gauche

    tabQuadtree->noeuds[index].m = (tabQuadtree->noeuds[4 * index + 1].m + 
                                    tabQuadtree->noeuds[4 * index + 2].m + 
                                    tabQuadtree->noeuds[4 * index + 3].m + 
                                    tabQuadtree->noeuds[4 * index + 4].m) / 4;

    tabQuadtree->noeuds[index].epsilon = (tabQuadtree->noeuds[4 * index + 1].m + tabQuadtree->noeuds[4 * index + 2].m + tabQuadtree->noeuds[4 * index + 3].m + tabQuadtree->noeuds[4 * index + 4].m) % 4; 

    if(tabQuadtree->noeuds[index].epsilon != 0 
        || tabQuadtree->noeuds[4 * index + 1].u == 0 || tabQuadtree->noeuds[4 * index + 2].u == 0 || tabQuadtree->noeuds[4 * index + 3].u == 0 || tabQuadtree->noeuds[4 * index + 4].u == 0 
        || tabQuadtree->noeuds[4 * index + 1].m != tabQuadtree->noeuds[4 * index + 2].m || tabQuadtree->noeuds[ 4 * index + 1].m != tabQuadtree->noeuds[ 4 * index + 3].m || tabQuadtree->noeuds[ 4 * index + 1].m != tabQuadtree->noeuds[ 4 * index + 4].m){
            tabQuadtree->noeuds[index].u = 0;
    }
    else{
        flagAffiche(tabQuadtree, index);
        tabQuadtree->noeuds[index].affiche = 1;
    }
        
}


TabQuadtree constructeurQuadtreePGM(int tailleImage, unsigned char** image, int profondeur){
    TabQuadtree tree = initQuadtree(profondeur + 1);
    rempliQuadtreePGM(tailleImage, 0, 0, image, &tree, 0);
    return tree;
}

void afficheQuadtree(TabQuadtree tab){
    for(int i = 0; i < tab.tailleTable; i++){
        printf("(%d %d %d)", tab.noeuds[i].m, tab.noeuds[i].epsilon, tab.noeuds[i].u);
    }
    printf("\n");
}
