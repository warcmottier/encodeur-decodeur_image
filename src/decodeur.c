#include "../include/encodeur.h"
#include <math.h>
#include <string.h>
#include <time.h>

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
void ecrirePGM(char* nom, unsigned char** image, int taille){
    FILE* f = fopen(nom, "wb");

    if(f == NULL){
        fprintf(stderr, "erreur lecture fichier\n");
        return;
    }

    time_t actuelle;
    struct tm *infoTemp;

    time(&actuelle);
    infoTemp = localtime(&actuelle);

    fprintf(f, "P5\n");
    fprintf(f, "# crée le %d-%d-%d à %d:%d:%d\n", infoTemp->tm_mday, 
                                                 infoTemp->tm_mon + 1,
                                                 infoTemp->tm_year,
                                                 infoTemp->tm_hour,
                                                 infoTemp->tm_min,
                                                 infoTemp->tm_sec);
    fprintf(f, "%d %d\n", taille, taille);
    fprintf(f, "255\n");
    
    for(int i = 0; i < taille; i++){
        for(int j = 0; j < taille; j++){
            fprintf(f, "%c", image[i][j]);
        }
    }
}

void quadtreeToPixmap(TabQuadtree quadtree, unsigned char** image, int taille, int x, int y, int index, int profondeur){
    if(index >= quadtree.tailleTable || profondeur > quadtree.profondeurMax - 1)
        return;

    if(taille == 1 || profondeur == quadtree.profondeurMax - 1){
        image[y][x] = quadtree.noeuds[index].m;
        //printf("feuille %d %d\n", quadtree.noeuds[index-1].m, index - 1);
        return;
    }

    int moitierTaille = taille / 2;

    //printf("index %d\n", index);
    quadtreeToPixmap(quadtree, image, moitierTaille, x, y, 4 * index + 1, profondeur + 1); // enfant haut gauche
    quadtreeToPixmap(quadtree, image, moitierTaille, x + moitierTaille, y, 4 * index + 2, profondeur + 1); // enfant haut droit
    quadtreeToPixmap(quadtree, image, moitierTaille, x + moitierTaille, y + moitierTaille, 4 * index + 3, profondeur + 1); // enfant bas droit
    quadtreeToPixmap(quadtree, image, moitierTaille, x, y + moitierTaille, 4 * index + 4, profondeur + 1); // enfant bas gauche
}

/**
 * @brief initialise le bitstream pour l'ecriture du qtc
 * 
 * @param tailleTotal 
 * @return BitStream 
 */
BitStream initBitStreamLecture(int tailleTotal, FILE* f){
    BitStream bit;
    bit.capa = 8;
    bit.index = 0;
    bit.tailleTotal = tailleTotal;
    bit.ptr = malloc(sizeof(unsigned char) * bit.tailleTotal);
    fread(bit.ptr, sizeof(unsigned char), tailleTotal, f);
    
    return bit;
}

/**
 * @brief ecrit le pgm de l'image
 * 
 * @param nom 
 */
void lireQTC(char* nom, TabQuadtree* quadtree) {
    FILE* f = fopen(nom, "rb");

    if (!f) {
        fprintf(stderr, "erreur fichier non existant\n");
        return;
    }

    BitStream bitstream;
    char enTete[3];
    int nbOctet;
    unsigned char profondeur;
    long positionActu;

    if (fscanf(f, "%2s", enTete) != 1 || strcmp(enTete, "Q1") != 0) {
        fprintf(stderr, "erreur format non conforme utilisation du format Q1 requis\n");
        fclose(f);
        return;
    }

    fgetc(f);  // Consomme le caractère suivant (probablement un retour à la ligne)

    char ligne[100];
    long positionDebutLigneValide;

    // Sauter les commentaires jusqu'à trouver une ligne valide
    do {
        positionDebutLigneValide = ftell(f);  // Sauvegarder la position avant de lire la ligne
        fgets(ligne, sizeof(ligne), f);
    } while (ligne[0] == '#');

    // Revenir au début de la ligne valide
    fseek(f, positionDebutLigneValide, SEEK_SET);

    // Lire la profondeur
    fscanf(f, "%c", &profondeur);  // Ou utilisez "%d" si la profondeur est un entier
    printf("Profondeur lue : %d\n", profondeur);

    // Calculer la taille du fichier après la lecture de la profondeur
    positionActu = ftell(f);
    fseek(f, 0, SEEK_END);
    nbOctet = ftell(f) - positionActu;
    fseek(f, positionActu, SEEK_SET);

    // Initialiser le bitstream et construire le quadtree
    bitstream = initBitStreamLecture(nbOctet, f);
    constructeurQuadtreeQTC(profondeur, quadtree, f, bitstream);

    fclose(f);
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

unsigned char** initImage(int taille){
    unsigned char** image;
    image = malloc(sizeof(unsigned char*) * taille);
    if(image == NULL){
        fprintf(stderr, "erreur allocation memoire\n");
        return NULL;
    }

    for(int i = 0; i < taille; i++){
        image[i] = malloc(sizeof(unsigned char) * taille);

        if(image[i] == NULL){
            fprintf(stderr, "erreur allocation memoire\n");
            for(int j = 0; j < i; j++){
                free(image[j]);
            }
            free(image);
            return NULL;
        }
    }

    for(int i = 0; i < taille; i++){
        for(int j = 0; j < taille; j++){
            image[i][j] = 0;
        }
    }

    return image;
}

void libereImage(unsigned char** image, int taille){
    
    for(int i = 0; i < taille; i++)
        free(image[i]);
    free(image);

    return;
}

void decode(char* nom){
    TabQuadtree quadtree;
    lireQTC(nom, &quadtree);
    int taille = tailleImage(quadtree.profondeurMax - 1);
    unsigned char** image = initImage(taille);
    quadtreeToPixmap(quadtree, image, taille, 0, 0, 0, 0);
    libereQuadtree(&quadtree);
    ecrirePGM(ExtensionPGM(nom), image, taille);
    libereImage(image, taille);

}  
