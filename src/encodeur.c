#include "../include/encodeur.h"
#include <math.h>
#include <string.h>
#include <time.h>
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
    long positionDebutLigneValide;
    do {
        positionDebutLigneValide = ftell(f);
        fgets(ligne, sizeof(ligne), f);
    } while (ligne[0] == '#');

    fseek(f, positionDebutLigneValide, SEEK_SET);

    fscanf(f, "%d ", taille);

    fscanf(f, "%d ", taille);

    fscanf(f, "%d ", &maxValGris);

    if(maxValGris != 255){
        fprintf(stderr, "erreur valeur max de gris doit être a 255\n");
        fclose(f);
        return NULL;
    }

    image = malloc(sizeof(unsigned char*) * (*taille));

    if(image == NULL){
        fprintf(stderr, "erreur allocation memoire\n");
        fclose(f);
        return NULL;
    }
    
    for(int i = 0; i < (*taille); i++){
        image[i] = malloc(sizeof(unsigned char) * (*taille));
        
        if(image[i] == NULL){
            fprintf(stderr, "erreur allocation memoire\n");
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

void libererImage(unsigned char** image, int taille){
    
    for(int i = 0; i < taille; i++)
        free(image[i]);
    free(image);

    return;
}

/**
 * @brief prend le nom du fichier .pgm et renvoie le nom de se meme fichier mais en .qtc
 * 
 * @param fichierPGM 
 * @return char* 
 */
char* ExtensionQTC(const char* fichierPGM){
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
 * @brief calcule le nombre de donner a ecrire
 * 
 * @param nbm 
 * @param nbe 
 * @param nbu 
 * @param quadtree 
 * @param index 
 * @param profondeurMax 
 * @param noeud4 
 */
void nbDonnee(int* nbm, int* nbe, int* nbu, TabQuadtree quadtree, int index, int profondeurMax, int noeud4){
    static int profondeurActu = 0;
    
    if(index >= quadtree.tailleTable){
        profondeurActu--;
        return;
    }

    if(noeud4 != 1)
        (*nbm)++;

    if(profondeurActu == profondeurMax){
        profondeurActu--;
        return;
    }

    if(quadtree.noeuds[index].epsilon == 0){
        (*nbu)++;
    }

    (*nbe)++;

    if(quadtree.noeuds[index].u == 1 && quadtree.noeuds[index].epsilon == 0){
        //printf("coucou %d %d %d\n", quadtree.noeuds[index].m, quadtree.noeuds[index].epsilon, quadtree.noeuds[index].u);
        profondeurActu--;
        return;
    }

    profondeurActu++;
    nbDonnee(nbm, nbe, nbu, quadtree, 4 * index + 1, profondeurMax, 0); //1er fils
    profondeurActu++;
    nbDonnee(nbm, nbe, nbu, quadtree, 4 * index + 2, profondeurMax, 0); //2eme fils
    profondeurActu++;
    nbDonnee(nbm, nbe, nbu, quadtree, 4 * index + 3, profondeurMax, 0); //3eme fils
    profondeurActu++;
    nbDonnee(nbm, nbe, nbu, quadtree, 4 * index + 4, profondeurMax, 1); //4eme fils

    profondeurActu--;

}

/**
 * @brief rempli le tableau de bit bit par bit pour pouvoir ecrire tout les octet d'un coup
 * 
 * @param bit 
 * @param quadtree 
 * @param index 
 * @param profondeurMax 
 * @param noeud4 
 */
void remplirBit(BitStream* bit, TabQuadtree quadtree, int index, int profondeurMax, int noeud4) {

    for(int i = 0; i < quadtree.tailleTable; i++){

        if(quadtree.noeuds[i].affiche == 0){
            continue;
        }

        if (4 * i + 1 >= quadtree.tailleTable && i % 4 != 0){
            for(int j = 7; j >= 0; j--){
                ecrireBit(bit, (quadtree.noeuds[i].m >> j) & 1);
            }
            continue;
        }

        if(4 * i + 1 >= quadtree.tailleTable && i % 4 == 0){
            
            continue;
        }
        
        if(i == 0 || i % 4 != 0){
            for(int j = 7; j >= 0; j--){
                ecrireBit(bit, (quadtree.noeuds[i].m >> j) & 1);
            }
        }

        for (int j = 1; j >= 0; j--) {
            ecrireBit(bit, (quadtree.noeuds[i].epsilon >> j) & 1);
        }

        if(quadtree.noeuds[i].epsilon == 0){
            ecrireBit(bit, quadtree.noeuds[i].u);
        }
    }

}

/**
 * @brief initialise le bitstream pour l'ecriture du qtc
 * 
 * @param tailleTotal 
 * @return BitStream 
 */
BitStream initBitStreamEcriture(int tailleTotal){
    BitStream bit;
    bit.capa = 8;
    bit.index = 0;
    bit.tailleTotal = tailleTotal;
    bit.ptr = malloc(sizeof(unsigned char) * bit.tailleTotal);
    for(int i = 0; i < tailleTotal; i++){
        bit.ptr[i] = 0;
    }
    
    return bit;
}

int totalOctet(int nbm, int nbe, int nbu){
    return (nbm * 8 + nbe * 2 + nbu + 7) / 8;
}

/**
 * @brief cree et ecrit le qtc
 * parcourir l'arbre pour recup le nombre de e u et m a ecrire 
 * pour calculer le taux de compression (taille * taille * 8) / (e * 3 + u * 1 + m * 8) * 100
 * 
 * @param tab 
 * @param nom 
 */
void ecrireQTC(TabQuadtree tab, const char* nom, unsigned char profondeurs, int taille){
    int nbm = 0, nbe = 0, nbu = 0;
    time_t actuelle;
    struct tm *infoTemp;
    
    FILE* f = fopen(nom, "wb");

    if(f == NULL){
        fprintf(stderr, "erreur fichier ne peut pas s'ouvrir\n");
    }

    
    nbDonnee(&nbm, &nbe, &nbu, tab, 0, profondeurs, 0);
    
    BitStream bit = initBitStreamEcriture(totalOctet(nbm, nbe, nbu));

    if(bit.ptr == NULL){
        fprintf(stderr, "erreur malloc pas assez de memoire\n");
        fclose(f);
        return;
    }

    fprintf(f, "Q1\n");

    
    time(&actuelle);
    infoTemp = localtime(&actuelle);

    fprintf(f, "# crée le %d-%d-%d à %d:%d:%d\n", infoTemp->tm_mday, 
                                                 infoTemp->tm_mon + 1,
                                                 infoTemp->tm_year,
                                                 infoTemp->tm_hour,
                                                 infoTemp->tm_min,
                                                 infoTemp->tm_sec);

    fprintf(f, "# taux de compression : %f%%\n", ((float) (nbm * 8 + nbe * 2 + nbu) / (taille * taille * 8)) * 100 );

    printf("profondeur ecrite : %d\n", profondeurs);
    fprintf(f, "%c", profondeurs);

    remplirBit(&bit, tab, 0, profondeurs, 0);

    fwrite(bit.ptr, sizeof(unsigned char), bit.tailleTotal, f);

    fclose(f);
}


void codage(char* nom){
    int taille, profondeurs;
    unsigned char** image;
    TabQuadtree tree;
    image = creeTabImage(nom, &taille);
    if(image == NULL)
        return;
    profondeurs = profondeur(taille);
    printf("%d\n", profondeurs);

    tree = constructeurQuadtreePGM(taille, image, profondeurs);
    libererImage(image, taille);

    ecrireQTC(tree, ExtensionQTC(nom), profondeurs, taille);
    libereQuadtree(&tree);
}
