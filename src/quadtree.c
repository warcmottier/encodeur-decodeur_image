#include "../include/quadtree.h"

/**
 * @brief initialise le Quadtree
 * 
 * @param profondeur 
 * @return Quadtree 
 */
TabQuadtree initQuadtree(int profondeur){
    TabQuadtree quadtree;
    quadtree.tailleTable = (((int) pow(4, profondeur)) - 1) / 3;
    quadtree.noeuds = malloc(sizeof(Noeud) * quadtree.tailleTable);
    if(quadtree.noeuds == NULL)
        return quadtree;
    for (int i = 0; i < quadtree.tailleTable; i++){
        quadtree.noeuds[i].epsilon = 0;
        quadtree.noeuds[i].u = 1;
        quadtree.noeuds[i].m = 0;
        quadtree.noeuds[i].affiche = 1;
        quadtree.profondeurMax = profondeur;
    }
    
    return quadtree;
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
void rempliQuadtreePGM(int tailleImage, int x, int y, unsigned char** image, TabQuadtree *quadtree, int index) {
    //on est sur une feuille
    if (tailleImage == 1) {
        quadtree->noeuds[index].m = image[y][x];
        return;
    }

    int moitierTaille = tailleImage / 2;


    rempliQuadtreePGM(moitierTaille, x, y, image, quadtree, 4 * index + 1); // enfant haut gauche
    rempliQuadtreePGM(moitierTaille, x + moitierTaille, y, image, quadtree, 4 * index + 2); // enfant haut droit
    rempliQuadtreePGM(moitierTaille, x + moitierTaille, y + moitierTaille, image, quadtree, 4 * index + 3); // enfant bas droit
    rempliQuadtreePGM(moitierTaille, x, y + moitierTaille, image, quadtree, 4 * index + 4); // enfant bas gauche

    quadtree->noeuds[index].m = (quadtree->noeuds[4 * index + 1].m + 
                                    quadtree->noeuds[4 * index + 2].m + 
                                    quadtree->noeuds[4 * index + 3].m + 
                                    quadtree->noeuds[4 * index + 4].m) / 4;

    quadtree->noeuds[index].epsilon = (quadtree->noeuds[4 * index + 1].m + quadtree->noeuds[4 * index + 2].m +
                                          quadtree->noeuds[4 * index + 3].m + quadtree->noeuds[4 * index + 4].m) % 4; 

    if(quadtree->noeuds[index].epsilon != 0 
        || quadtree->noeuds[4 * index + 1].u == 0 || quadtree->noeuds[4 * index + 2].u == 0 || quadtree->noeuds[4 * index + 3].u == 0 
        || quadtree->noeuds[4 * index + 4].u == 0 
        || quadtree->noeuds[4 * index + 1].m != quadtree->noeuds[4 * index + 2].m || quadtree->noeuds[ 4 * index + 1].m != quadtree->noeuds[ 4 * index + 3].m 
        || quadtree->noeuds[ 4 * index + 1].m != quadtree->noeuds[ 4 * index + 4].m){
            quadtree->noeuds[index].u = 0;
    }
    else{
        flagAffiche(quadtree, index);
        quadtree->noeuds[index].affiche = 1;
    }
}

TabQuadtree constructeurQuadtreePGM(int tailleImage, unsigned char** image, int profondeur){
    TabQuadtree quadtree = initQuadtree(profondeur + 1);
    rempliQuadtreePGM(tailleImage, 0, 0, image, &quadtree, 0);
    return quadtree;
}

/**
 * @brief trouve le parent d'un fils
 * 
 * @param index 
 * @return int 
 */
int trouverParent(int index) {
    return (index - 1) / 4;
}

/**
 * @brief met affiche a 0 pour tout le sous arbre du noued d'index index afin de remplire leur bonne valeur m dans le quadtree 
 * @param quadtree 
 * @param m 
 * @param index 
 */
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

/**
 * @brief rempli la grille
 * 
 * @param quadtree 
 * @param grille 
 * @param index 
 * @param uniforme 
 * @param taille 
 * @param x 
 * @param y 
 */
void quadtreeToGrille(TabQuadtree quadtree, int** grille, int index, int taille, int x, int y) {
    
    if (taille == 1) {
        grille[y][x] = quadtree.noeuds[trouverParent(index)].u ? 255 : 0;
        return;
    }

    int moitierTaille = taille / 2;

    quadtreeToGrille(quadtree, grille, 4 * index + 1, moitierTaille, x, y);
    quadtreeToGrille(quadtree, grille, 4 * index + 2, moitierTaille, x + moitierTaille, y);
    quadtreeToGrille(quadtree, grille, 4 * index + 3, moitierTaille, x + moitierTaille, y + moitierTaille);
    quadtreeToGrille(quadtree, grille, 4 * index + 4, moitierTaille, x, y + moitierTaille);
}


int** creeGrille(TabQuadtree quadtree, int taille){
    int** grille;

    
    grille = malloc(sizeof(int*) * taille);

    if(grille == NULL){
        fprintf(stderr, "erreur allocation memoire\n");
        return NULL;
    }
    
    for(int i = 0; i < (taille); i++){
        grille[i] = malloc(sizeof(int) * taille);
        
        if(grille[i] == NULL){
            fprintf(stderr, "erreur allocation memoire\n");
            for(int j = 0; j < i; j++){
                free(grille[j]);
            }

            free(grille);
            return NULL;
        }
    }

    quadtreeToGrille(quadtree, grille, 0, taille, 0, 0);
    

    return grille;
}

void afficheQuadtree(TabQuadtree quadtree){
    for(int i = 0; i < quadtree.tailleTable; i++){
        printf("(%d %d %d)", quadtree.noeuds[i].m, quadtree.noeuds[i].epsilon, quadtree.noeuds[i].u);
    }
    printf("\n");
}

void libereQuadtree(TabQuadtree* quadtree){
    free(quadtree->noeuds);
}
