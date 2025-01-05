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
 * @brief ecrit le pgm a partir de la pixmap
 * 
 * @param nom 
 * @param image 
 * @param taille 
 * @param verbeux 
 */
void ecrirePGM(char* nom, unsigned char** image, int taille, int verbeux){
    FILE* f = fopen(nom, "wb");

    if(f == NULL){
        fprintf(stderr, "erreur lecture fichier\n");
        exit(EXIT_FAILURE);
    }

    if(verbeux){
        fprintf(stderr, "fichier ouvert\n");
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

    if(verbeux){
        fprintf(stderr, "fichier ecrit\n");
    }
}

void quadtreeToPixmap(TabQuadtree quadtree, unsigned char** image, int taille, int x, int y, int index, int profondeur){
    if(index >= quadtree.tailleTable || profondeur > quadtree.profondeurMax - 1)
        return;

    if(taille == 1 || profondeur == quadtree.profondeurMax - 1){
        image[y][x] = quadtree.noeuds[index].m;
        return;
    }

    int moitierTaille = taille / 2;

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
 * @brief lit le qtc
 * 
 * @param nom 
 * @param quadtree 
 * @param verbeux 
 */
void lireQTC(char* nom, TabQuadtree* quadtree, int verbeux) {
    FILE* f = fopen(nom, "rb");

    if (!f) {
        fprintf(stderr, "erreur fichier non existant\n");
        exit(EXIT_FAILURE);
    }

    if(verbeux){
        fprintf(stderr, "fichier ouvert\n");
    }

    BitStream bitstream;
    char enTete[3];
    int nbOctet;
    unsigned char profondeur;
    long positionActu;

    if (fscanf(f, "%2s", enTete) != 1 || strcmp(enTete, "Q1") != 0) {
        fprintf(stderr, "erreur format non conforme utilisation du format Q1 requis\n");
        fclose(f);
        exit(EXIT_FAILURE);
    }

    fgetc(f);

    char ligne[100];
    long positionDebutLigneValide;

    // Sauter les commentaires jusqu'à trouver une ligne valide
    do {
        positionDebutLigneValide = ftell(f);
        fgets(ligne, sizeof(ligne), f);
    } while (ligne[0] == '#');

    fseek(f, positionDebutLigneValide, SEEK_SET);

    fscanf(f, "%c", &profondeur);

    positionActu = ftell(f);
    fseek(f, 0, SEEK_END);
    nbOctet = ftell(f) - positionActu;
    fseek(f, positionActu, SEEK_SET);

    bitstream = initBitStreamLecture(nbOctet, f);
    constructeurQuadtreeQTC(profondeur, quadtree, f, bitstream);

    if(verbeux){
        fprintf(stderr, "fichier lue\n");
    }

    fclose(f);
}

/**
 * @brief initialise l'image
 * 
 * @param taille 
 * @return unsigned** 
 */
unsigned char** initImage(int taille){
    unsigned char** image;
    image = malloc(sizeof(unsigned char*) * taille);
    if(image == NULL){
        fprintf(stderr, "erreur allocation memoire\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < taille; i++){
        image[i] = malloc(sizeof(unsigned char) * taille);

        if(image[i] == NULL){
            fprintf(stderr, "erreur allocation memoire\n");
            for(int j = 0; j < i; j++){
                free(image[j]);
            }
            free(image);
            exit(EXIT_FAILURE);
        }
    }

    for(int i = 0; i < taille; i++){
        for(int j = 0; j < taille; j++){
            image[i][j] = 0;
        }
    }

    return image;
}

/**
 * @brief libere de la memoire l'image
 * 
 * @param image 
 * @param taille 
 */
void libereImage(unsigned char** image, int taille){
    for(int i = 0; i < taille; i++)
        free(image[i]);
    free(image);

    return;
}

/**
 * @brief renvoie le nom du fichier qui va contenir la grille
 * 
 * @param nomOriginal 
 * @return char* 
 */
char* nomGrilleQTC(const char* nomOriginal) {
    if (nomOriginal == NULL) {
        return NULL;
    }

    const char* extension = strrchr(nomOriginal, '.');
    if (extension == NULL || strcmp(extension, ".qtc") != 0) {
        return NULL;
    }

    int longueurBase = extension - nomOriginal;

    char* new_name = (char*)malloc(longueurBase + 3 + strlen(extension) + 1);
    if (new_name == NULL) {
        return NULL;
    }

    strncpy(new_name, nomOriginal, longueurBase);
    new_name[longueurBase] = '\0';
    strcat(new_name, "_g");
    strcat(new_name, ".pgm");

    return new_name;
}

/**
 * @brief cree le fichier qui va contenir la grille
 * 
 * @param grille 
 * @param nom 
 * @param taille 
 */
void afficheGrilleQTC(int** grille, char* nom, int taille){
    
    FILE* f = fopen(nom, "w");

    if(f == NULL){
        fprintf(stderr, "erreur fichier inexistant\n");
        return;
    }

    fprintf(f, "P2\n");

    fprintf(f, "# grille quadtree\n");

    fprintf(f, "%d %d\n", taille, taille);

    fprintf(f, "255\n");

    for(int i = 0; i < taille; i++){
        for(int j = 0; j < taille; j++){
            fprintf(f, "%d ", grille[i][j]);
        }
        fprintf(f, "\n");
    }
}

/**
 * @brief libere le tableau qui contenais la grille
 * 
 * @param grille 
 * @param taille 
 */
void libereGrilleQTC(int** grille, int taille){
    for(int i = 0; i < taille; i++){
        free(grille[i]);
    }
    free(grille);
}

void decode(char* nomEntrer, char* nomSortie, int grille, int verbeux){
    TabQuadtree quadtree;
    lireQTC(nomEntrer, &quadtree, verbeux);
    int taille = tailleImage(quadtree.profondeurMax - 1);

    if(grille){
        int ** grille = creeGrille(quadtree, taille);
        afficheGrilleQTC(grille, nomGrilleQTC(nomEntrer), taille);
        libereGrilleQTC(grille, taille);
    }

    unsigned char** image = initImage(taille);
    quadtreeToPixmap(quadtree, image, taille, 0, 0, 0, 0);
    
    if(verbeux){
        fprintf(stderr, "pixmap charger\n");
    }
    
    libereQuadtree(&quadtree);
    ecrirePGM(nomSortie, image, taille, verbeux);
    libereImage(image, taille);

}  
