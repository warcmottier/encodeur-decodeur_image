#include "../include/encodeur.h"
#include <math.h>
#include <string.h>
#include <limits.h>

/**
 * @brief calcule la profondeur du quadtree a partir de la taille de l'image
 * 
 * @param taille 
 * @return int 
 */
int profondeur(int taille){
    return log2(taille);
}

/**
 * @brief cree un tableau de la taille de l'image remplis
 * 
 * @param nom 
 * @param taille 
 * @return unsigned* 
 */
unsigned char** creeTabImage(char* nom, int* taille){
    FILE* f;

    f = fopen(nom, "rb");

    if(!f){
        fprintf(stderr, "erreur fichier non existant\n");
        return NULL;
    }

    char enTete[3];
    int maxValGris;
    unsigned char **image;

    if(fscanf(f, "%2s", enTete) != 1 || strcmp(enTete, "P5") != 0){
        fprintf(stderr, "erreur format non conforme utilisation du format P5 requis\n");
        fclose(f);
        return NULL;
    }

    fgetc(f);

    char ligne[100];
    
    fgets(ligne, sizeof(ligne), f);

    fscanf(f, "%d ", taille);

    fscanf(f, "%d ", taille);

    fscanf(f, "%d ", &maxValGris);

    if(maxValGris < 255){
        fprintf(stderr, "erreur valeur max de gris doit être a 255\n");
        fclose(f);
        return NULL;
    }

    image = malloc(sizeof(unsigned char*) * (*taille));
    
    for(int i = 0; i < (*taille); i++){
        image[i] = malloc(sizeof(unsigned char) * (*taille));
        
        if(image[i] == NULL){
            for(int j = 0; j < i; j++){
                free(image[j]);
            }

            free(image);
            fclose(f);
            return NULL;
        }
    }

    for(int i = 0; i < (*taille); i++){
        for(int j = 0; j < (*taille); j++){
            image[i][j] = fgetc(f);
        }
    }

    fclose(f);


    return image;
}

/**
 * @brief prend le nom du fichier .pgm et renvoie le nom de se meme fichier mais en .qtc
 * 
 * @param fichierPGM 
 * @return char* 
 */
char* nouvelleExtension(const char* fichierPGM){
    const char* point = strrchr(fichierPGM, '.');

    int longPrefixe = point - fichierPGM;
    int longNouveau = longPrefixe + 4;

    char* fichierQTC = malloc(sizeof(char) * longNouveau);
    if(fichierQTC == NULL)
        return NULL;
    
    strncpy(fichierQTC, fichierPGM, longPrefixe);

    strcpy(fichierQTC + longPrefixe, ".qtc");

    return fichierQTC;
}

/**
 * @brief cree et ecrit le qtc
 * parcourir l'arbre pour recup le nombre de e u et m a ecrire 
 * pour calculer le taux de compression (taille * taille * 8) / (e * 3 + u * 1 + m * 8) * 100
 * 
 * @param tab 
 * @param nom 
 */
void ecrireQTC(TabQuadtree tab, const char* nom){
    FILE* f2 = fopen(nom, "wb");
    BitStream bit;

    bit.ptr = malloc(sizeof(unsigned char) * );

    if(f2 == NULL){
        return;
    }

    for(int i = 0; i < tab.tailleTable; i++){

    }

}


void codage(char* nom){
    int taille;
    unsigned char** image;
    TabQuadtree tree;
    image = creeTabImage(nom, &taille);

    tree = constructeurQuadtreePGM(taille, image, profondeur(taille));

    ecrireQTC(tree, nouvelleExtension(nom));

    afficheQuadtree(tree);
}
