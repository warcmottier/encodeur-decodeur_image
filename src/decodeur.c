#include "../include/encodeur.h"
#include <math.h>
#include <string.h>

/**
 * @brief Calcule la taille de l'image à partir de la profondeur du quadtree
 * 
 * @param profondeur La profondeur du quadtree
 * @return int La taille de l'image (côté d'une image carrée)
 */
int tailleImage(int profondeur) {
    return pow(2, profondeur);
}

/**
 * @brief lit le qtc et rempli le quadtree
 * 
 * @param nom 
 */
TabQuadtree lireQTC(char* nom){

}

/**
 * @brief ecrit le pgm de l'image
 * 
 * @param nom 
 */
void ecrirePGM(char* nom){
    FILE* f = fopen(nom, "rb");

    if(!f){
        fprintf(stderr, "erreur fichier non existant\n");
        return;
    }

    TabQuadtree quadtree;

    char enTete[3];
    int profondeur;

    if(fscanf(f, "%2s", enTete) != 1 || strcmp(enTete, "Q1") != 0){
        fprintf(stderr, "erreur format non conforme utilisation du format Q1 requis\n");
        fclose(f);
        return;
    }

    fgetc(f);

    char ligne[100];
    do {
        fgets(ligne, sizeof(ligne), f);
    } while (ligne[0] == '#');

    fseek(f, -strlen(ligne), SEEK_CUR);

    profondeur = fgetc(f);
}

/**
 * @brief prend le nom du fichier .qtc et renvoie le nom de se meme fichier mais en .pgm
 * 
 * @param fichierQTC
 * @return char* 
 */
char* ExtensionPGM(const char* fichierQTC){
    const char* point = strrchr(fichierQTC, '.');

    int longPrefixe = point - fichierQTC;
    int longNouveau = longPrefixe + 4;

    char* fichierPGM = malloc(sizeof(char) * longNouveau);
    if(fichierPGM == NULL)
        return NULL;
    
    strncpy(fichierPGM, fichierQTC, longPrefixe);

    strcpy(fichierPGM + longPrefixe, ".pgm");

    return fichierPGM;
}


void decode(char* nom){
    
}
