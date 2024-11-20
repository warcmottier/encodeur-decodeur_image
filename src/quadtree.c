#include "./include/quadtree.h"

Quadtree initQuadtree(int profondeur){
    Quadtree tree;
    tree.tailleTable = (pow(4, profondeur + 1) - 1) / 3;
    tree.noeuds = malloc(sizeof(Noeud) * tree.tailleTable);
    return tree;
}

Quadtree constructeurQuadtree(int tailleImage, unsigned char** image, int n){
    Quadtree tree = initQuadtree(n + 1);
    for(int i = 0; i < tailleImage; i++){
        for (int i = 0; i < tailleImage; i++){
            
        }
        
    }
}
