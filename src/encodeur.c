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
    
    char ligne[100];
    long positionDebutLigneValide;
    fgets(ligne, sizeof(ligne), f);
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

void estFeuille(int index, TabQuadtree quadtree){
    if(4 * index + 1 >= quadtree.tailleTable)
        printf("je suis une feuille\n");
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
    int racine = 1;
    int noued4 = 0;

    // Vérification si quadtree.tailleTable est valide
    if (quadtree.tailleTable <= 0 || quadtree.noeuds == NULL) {
        return;  // Gestion d'erreur, retour si les données sont invalides
    }

    for (int i = 0; i < quadtree.tailleTable; i++) {

        // Si le noeud actuel ne doit pas être affiché, on passe au suivant
        if (quadtree.noeuds[i].affiche == 0) {
            continue;
        }

        // Si ce n'est pas la première racine, on augmente le compteur de noeuds de type 4
        if (!racine) {
            noued4++;
        } else {
            racine = 0;  // On définit "racine" à 0 après le premier tour
        }

        // Si on a dépassé la taille du tableau ou si le noeud4 n'est pas encore à 4, on écrit l'octet
        if (4 * i + 1 >= quadtree.tailleTable && noeud4 != 4) {
            for (int j = 7; j >= 0; j--) {
                ecrireBit(bit, (quadtree.noeuds[i].m >> j) & 1);
            }
            noued4 = 0;  // Réinitialisation du compteur de noeuds 4
            continue;
        }

        // Si le noeud4 est égal à 4, on le saute sans rien faire
        if (4 * i + 1 >= quadtree.tailleTable && noued4 == 4) {
            continue;
        }

        // Si le compteur de noeuds 4 est différent de 4, on écrit l'octet
        if (noued4 != 4) {
            for (int j = 7; j >= 0; j--) {
                ecrireBit(bit, (quadtree.noeuds[i].m >> j) & 1);
            }
            noued4 = 0;  // Réinitialisation du compteur de noeuds 4
        }

        
        // Écriture des bits de l'epsilon
        for (int j = 1; j >= 0; j--) {
            ecrireBit(bit, (quadtree.noeuds[i].epsilon >> j) & 1);
        }

        // Si epsilon est égal à 0, on écrit la valeur de u
        if (quadtree.noeuds[i].epsilon == 0) {
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

    printf("%d %d %d\n", nbm, nbe, nbu);
    
    BitStream bit = initBitStreamEcriture(totalOctet(nbm, nbe, nbu));
    printf("%d\n", bit.tailleTotal);

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

    fprintf(f, "# taux de compression : %.2f%%\n", ((float) (nbm * 8 + nbe * 2 + nbu) / (taille * taille * 8)) * 100 );

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

    tree = constructeurQuadtreePGM(taille, image, profondeurs, 1);
    libererImage(image, taille);

    ecrireQTC(tree, ExtensionQTC(nom), profondeurs, taille);
    libereQuadtree(&tree);
}
