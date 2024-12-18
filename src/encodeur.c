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
        *nbm++;

    if(profondeurActu == profondeurMax){
        profondeurActu--;
        return;
    }

    if(quadtree.noeuds[index].epsilon == 0){
        *nbu++;
    }

    *nbe++;

    profondeurActu++;
    nbDonnee(nbm, nbe, nbu, quadtree, 4 * index + 1, profondeurMax, 0); //1er fils
    nbDonnee(nbm, nbe, nbu, quadtree, 4 * index + 2, profondeurMax, 0); //2eme fils
    nbDonnee(nbm, nbe, nbu, quadtree, 4 * index + 3, profondeurMax, 0); //3eme fils
    nbDonnee(nbm, nbe, nbu, quadtree, 4 * index + 4, profondeurMax, 1); //4eme fils

    profondeurActu--;

}

int totalOctet(int nbm, int nbe, int nbu){
    int res = nbm * 8 + nbe * 2 + nbu;
    res += res % 8;
    return res / 8;
}

void remplirBit(BitStream* bit, TabQuadtree quadtree, int index, int profondeurMax, int noeud4){
    if(index >= quadtree.tailleTable){
        return;
    }

    if(noeud4 != 1){
        pushbits(bit, quadtree.noeuds[index].m, 8);
    }

    pushbits(bit, quadtree.noeuds[index].epsilon, 3);
    
    if(quadtree.noeuds[index].epsilon == 0)
        pushbits(bit, quadtree.noeuds[index].u, 1);
    
    

}

/**
 * @brief cree et ecrit le qtc
 * parcourir l'arbre pour recup le nombre de e u et m a ecrire 
 * pour calculer le taux de compression (taille * taille * 8) / (e * 3 + u * 1 + m * 8) * 100
 * 
 * @param tab 
 * @param nom 
 */
void ecrireQTC(TabQuadtree tab, const char* nom, int profondeurs){
    int nbm, nbe, nbu;
    FILE* f = fopen(nom, "wb");
    BitStream bit;
    
    nbDonnee(&nbm, &nbe, &nbu, tab, 0, profondeurs, 0);

    
    bit.ptr = malloc(sizeof(unsigned char) * totalOctet(nbm, nbe, nbu));

}


void codage(char* nom){
    int taille, profondeurs;
    unsigned char** image;
    TabQuadtree tree;
    image = creeTabImage(nom, &taille);
    profondeurs = profondeur(taille);

    tree = constructeurQuadtreePGM(taille, image, profondeurs);

    //ecrireQTC(tree, nouvelleExtension(nom));

    afficheQuadtree(tree);
}
