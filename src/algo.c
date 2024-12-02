#include "../include/algo.h"
#include <math.h>
#include <string.h>

/**
 * @brief calcule la profondeur du quadtree a partir de la taille de l'image
 * 
 * @param taille 
 * @return int 
 */
int profondeur(int taille){
    return log2(taille) + 1;
}

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
        return NULL;
    }

    char ligne[100];
    int carac;
    
    while((carac = fgetc(f)) == '#'){
        fgets(ligne, sizeof(ligne), f);
    }

    fseek(f, -strlen(ligne), SEEK_CUR);

    fscanf(f, "%d ", taille);

    fscanf(f, "%d ", taille);

    fscanf(f, "%d ", &maxValGris);

    if(maxValGris < 0 || maxValGris > 255){
        fprintf(stderr, "erreur valeur max de gris non compris entre 0 et 255\n");
        return NULL;
    }

    fgetc(f);

    image = malloc(sizeof(unsigned char*) * *taille);
    for(int i = 0; i < *taille; i++)
        image[i] = malloc(sizeof(unsigned char) * *taille);
    
    for(int i = 0; i < *taille; i++)
        for(int j = 0; j < *taille; j++)
            image[i][j] = fgetc(f);
    
    fclose(f);

    return image;
}

void codage(char* nom){
    int taille;
    unsigned char** image;
    TabQuadtree tree;

    image = creeTabImage(nom, &taille);

    tree = constructeurQuadtreePGM(taille, image, profondeur(taille));
}
