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
        tree.noeuds[i].v = 0;
        tree.profondeurMax = profondeur;
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
    if (tailleImage == 1) {
        tabQuadtree->noeuds[index].m = image[y][x];
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
    
    double variance = 0.0;
    for(int i = 0; i < 4; i++){
        variance += pow(tabQuadtree->noeuds[4 * index + i + 1].v, 2) + pow(tabQuadtree->noeuds[index].m - tabQuadtree->noeuds[4 * index + i + 1].m, 2);
    }
    tabQuadtree->noeuds[index].v = sqrt(variance / 7);

    tabQuadtree->noeuds[index].epsilon = (tabQuadtree->noeuds[4 * index + 1].m + tabQuadtree->noeuds[4 * index + 2].m +
                                          tabQuadtree->noeuds[4 * index + 3].m + tabQuadtree->noeuds[4 * index + 4].m) % 4; 

    if(tabQuadtree->noeuds[index].epsilon != 0 
        || tabQuadtree->noeuds[4 * index + 1].u == 0 || tabQuadtree->noeuds[4 * index + 2].u == 0 || tabQuadtree->noeuds[4 * index + 3].u == 0 
        || tabQuadtree->noeuds[4 * index + 4].u == 0 
        || tabQuadtree->noeuds[4 * index + 1].m != tabQuadtree->noeuds[4 * index + 2].m || tabQuadtree->noeuds[ 4 * index + 1].m != tabQuadtree->noeuds[ 4 * index + 3].m 
        || tabQuadtree->noeuds[ 4 * index + 1].m != tabQuadtree->noeuds[ 4 * index + 4].m){
            tabQuadtree->noeuds[index].u = 0;
    }
    else{
        flagAffiche(tabQuadtree, index);
        tabQuadtree->noeuds[index].affiche = 1;
    }
        
}

/**
 * @brief Fonction récursive de filtrage du quadtree.
 * 
 * @param quadtree : La structure du quadtree.
 * @param index : L'indice du noeud courant à filtrer.
 * @param sigma : Le seuil calculé à partir de la variance moyenne/maximale.
 * @param alpha : Facteur pour ajuster le seuil au fil de la descente dans l'arbre.
 * @return 1 si le noeud a été uniformisé, 0 sinon.
 */
int filtrage(TabQuadtree* quadtree, double sigma, double alpha, int index) {
    if (quadtree->noeuds[index].u == 1) {
        return 1;
    }

    if (4 * index + 1 >= quadtree->tailleTable) {
        return 1;
    }

    int s = 0;
    s += filtrage(quadtree, sigma * alpha, alpha, 4 * index + 1); // 1er fils
    s += filtrage(quadtree, sigma * alpha, alpha, 4 * index + 2); // 2ème fils
    s += filtrage(quadtree, sigma * alpha, alpha, 4 * index + 3); // 3ème fils
    s += filtrage(quadtree, sigma * alpha, alpha, 4 * index + 4); // 4ème fils

    if (s < 4 || quadtree->noeuds[index].v > sigma) {
        return 0;
    }

    quadtree->noeuds[index].epsilon = 0;
    quadtree->noeuds[index].u = 1; 

    return 1;
}

/**
 * @brief Calcule les variances moyenne et maximale pour tous les nœuds du quadtree.
 * 
 * @param quadtree : Le quadtree sur lequel calculer les variances.
 * @param medvar : La variance moyenne.
 * @param maxvar : La variance maximale.
 */
void calculerVariances(TabQuadtree* quadtree, double* medvar, double* maxvar) {
    double sum_var = 0;
    *maxvar = 0;
    int count = 0;
    for (int i = 0; i < quadtree->tailleTable; i++) {

        if (quadtree->noeuds[i].v  > *maxvar) {
            *maxvar = quadtree->noeuds[i].v;
        }
        sum_var += quadtree->noeuds[i].v ;
        count++;
    }

    *medvar = sum_var / count;
}

/**
 * @brief Fonction de filtrage pour tout le quadtree, partant de la racine.
 * 
 * @param quadtree : Le quadtree à filtrer.
 * @param alpha : Facteur d'ajustement du seuil.
 */
void filtrerQuadtree(TabQuadtree* quadtree, double alpha) {
    double medvar, maxvar;
    calculerVariances(quadtree, &medvar, &maxvar);
    printf("max %2f med %2f\n", maxvar, medvar);

    double sigma = (maxvar == 0) ? 1.0 : medvar / maxvar;

    filtrage(quadtree, sigma, alpha, 0);

    /*for(int i = 0; i < quadtree->tailleTable; i++){
        printf("%d %d\n", quadtree->noeuds[i].epsilon, quadtree->noeuds[i].u);   
    }*/
}

void afficher(TabQuadtree* quadtree){
    
    for(int i = 0; i < quadtree->tailleTable; i++){
        
        //printf("%d\n", quadtree->noeuds[i].affiche);
        if(!quadtree->noeuds[i].affiche){
            continue;
        }

        if(!quadtree->noeuds[i].epsilon && quadtree->noeuds[i].u){
            
            flagAffiche(quadtree, i);
            quadtree->noeuds[i].affiche = 1;
        }
    }
}

/**
 * @brief Fonction qui construit un quadtree pour une image donnée avec les paramètres de compression avec pertes.
 * 
 * @param tailleImage : La taille de l'image (en pixels).
 * @param image : L'image sous forme de tableau 2D.
 * @param profondeur : La profondeur du quadtree.
 * @param alpha : Facteur d'ajustement du seuil de filtrage.
 * @return TabQuadtree : Le quadtree résultant après filtrage.
 */
TabQuadtree constructeurQuadtreePGM(int tailleImage, unsigned char** image, int profondeur, double alpha) {
    TabQuadtree tree = initQuadtree(profondeur + 1);

    rempliQuadtreePGM(tailleImage, 0, 0, image, &tree, 0);
    
    //filtrerQuadtree(&tree, alpha);
    
    //afficher(&tree);

    return tree;
}

/**
 * @brief trouve le parent d'un fils
 * 
 * @param index 
 * @return int 
 */
int trouverParent(int index) {
    if (index <= 0) {
        return -1;
    }
    return (index - 1) / 4;
}

void remplirePixelUniforme(TabQuadtree* quadtree, unsigned char m, int index){
    if(index >= quadtree->tailleTable)
        return;
    
    quadtree->noeuds[index].m = m;
    quadtree->noeuds[index].affiche = 0;
    remplirePixelUniforme(quadtree, m, 4 * index + 1); //1er fils
    remplirePixelUniforme(quadtree, m, 4 * index + 2); //2eme fils
    remplirePixelUniforme(quadtree, m, 4 * index + 3); //3eme fils
    remplirePixelUniforme(quadtree, m, 4 * index + 4); //4eme fils

}

void constructeurQuadtreeQTC(int profondeur, TabQuadtree* quadtree, FILE* f, BitStream bitStream){
    *quadtree = initQuadtree(profondeur + 1);
    int racine = 1, noeud4 = 0;
    for(int i = 0; i < quadtree->tailleTable; i++){
        
        if(!quadtree->noeuds[i].affiche)
            continue;

        if(!racine)
            noeud4++;
        else
            racine = 0;
        
        // somme nous sur une feuille
        if(4 * i + 1 >= quadtree->tailleTable){
            //interpolation
            if(noeud4 == 4){
                quadtree->noeuds[i].m = (4 * quadtree->noeuds[trouverParent(i)].m 
                        + quadtree->noeuds[trouverParent(i)].epsilon) 
                        - (quadtree->noeuds[i-1].m + quadtree->noeuds[i - 2].m
                        + quadtree->noeuds[i - 3].m);
                noeud4 = 0;
                
            }
            else{
                quadtree->noeuds[i].m = lireBits(&bitStream, 8);
            }
            continue;
        }

        //interpolation
        if(noeud4 == 4){
            quadtree->noeuds[i].m = (4 * quadtree->noeuds[trouverParent(i)].m 
                    + quadtree->noeuds[trouverParent(i)].epsilon) 
                    - (quadtree->noeuds[i-1].m + quadtree->noeuds[i - 2].m
                    + quadtree->noeuds[i - 3].m);
            noeud4 = 0;
        }
        else{
            quadtree->noeuds[i].m = lireBits(&bitStream, 8);
        }

        quadtree->noeuds[i].epsilon = lireBits(&bitStream, 2);

        if(!quadtree->noeuds[i].epsilon){
            quadtree->noeuds[i].u = lireBits(&bitStream, 1);
            if(quadtree->noeuds[i].u){
                quadtree->noeuds[i].affiche = 0;
                //cree tout le sous arbre avec le meme m et epsilon a 0 et u a 1
                remplirePixelUniforme(quadtree, quadtree->noeuds[i].m, i);
            }
        }
        else
            quadtree->noeuds[i].u = 0;
    }
}

void afficheQuadtree(TabQuadtree tab){
    for(int i = 0; i < tab.tailleTable; i++){
        printf("(%d %d %d)", tab.noeuds[i].m, tab.noeuds[i].epsilon, tab.noeuds[i].u);
    }
    printf("\n");
}

void libereQuadtree(TabQuadtree* quadtree){
    free(quadtree->noeuds);
}
