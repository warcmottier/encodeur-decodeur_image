#include <limits.h>
#include "../include/gereBit.h"

/**
 * @brief renvoie le bit numero b de l’octet byte
 * 
 * @param byte 
 * @param b 
 * @return unsigned char 
 */
unsigned char getbit(unsigned char byte, size_t b){
  return (byte >> b) & 1  ;
}

/**
 * @brief change la valeur du numero b de l’octet ∗byte en valb
 * 
 * @param byte 
 * @param b 
 * @param valb 
 */
void setbit(unsigned char* byte, size_t b, unsigned char valb){
  if(valb)
    *byte = *byte | (valb << b);
  else
    *byte = (*byte | (1 << b)) ^ (1 << b);
}

void afficherBinaire8Bits(unsigned char n) {
    for (int i = 7; i >= 0; i--) {
        // Extraire chaque bit en partant du plus significatif
        unsigned char bit = (n >> i) & 1;
        // Afficher le bit
        printf("%d", bit);
    }
    printf("\n");
}

unsigned char moveRightmostBitToCapa(unsigned char src, unsigned char capa) {
    // Vérifier que capa est valide (doit être entre 1 et 8)
    if (capa < 1 || capa > 8) {
        printf("Erreur : capa doit être entre 1 et 8.\n");
        return src;  // Retourner l'original en cas d'erreur
    }

    // Extraire le bit de poids faible
    unsigned char rightmostBit = src & 1;

    // Décaler tous les bits vers la gauche de 'src' (capa - 1 positions)
    unsigned char result = src >> 1;  // Décalage d'abord à droite

    // Décaler tous les bits de 'src' vers la gauche de capa-1 pour simuler un shift de "capa" positions.
    result <<= capa - 1;  // Décalage vers la gauche de capa-1

    // Mettre le bit extrait (bit de poids faible) à la position capa
    if (rightmostBit) {
        result |= (1 << (capa - 1));  // Placer le bit à la bonne position
    }

    return result;
}

void ecrireBit(BitStream *bs, unsigned char bit) {
    // Si l'octet courant est plein, nous devons passer à l'octet suivant.
    if (bs->capa == 0) {
        bs->index++;
        bs->capa = 8;
    }

    // Nous devons insérer le bit dans l'octet en cours
    unsigned char mask = moveRightmostBitToCapa(1, bs->capa);
    if (bit) {
        bs->ptr[bs->index] |= mask;
    }

    // Réduire la capacité restante dans l'octet
    bs->capa--;
}

