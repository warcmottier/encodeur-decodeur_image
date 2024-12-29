#include <limits.h>
#include "../include/gereBit.h"

void afficherBinaire8Bits(unsigned char n) {
    for (int i = 7; i >= 0; i--) {
        // Extraire chaque bit en partant du plus significatif
        unsigned char bit = (n >> i) & 1;
        // Afficher le bit
        printf("%d", bit);
    }
    printf("\n");
}

/**
 * @brief met les bit le plus a droite a l'emplacement de la capaciter de l'octet actuelle
 * 
 * @param src 
 * @param capa 
 * @return unsigned char 
 */
unsigned char moveRightmostBitToCapa(unsigned char src, unsigned char capa) {

    unsigned char rightmostBit = src & 1;

    unsigned char result = src >> 1; 

    result <<= capa - 1;

    if (rightmostBit) {
        result |= (1 << (capa - 1));
    }

    return result;
}

void ecrireBit(BitStream *bitStream, unsigned char bit) {

    // si l'octet courant est plein on passe à l'octet suivant.
    if (bitStream->capa == 0) {
        bitStream->index++;
        bitStream->capa = 8;
    }

    unsigned char mask = moveRightmostBitToCapa(1, bitStream->capa);
    if (bit) {
        bitStream->ptr[bitStream->index] |= mask;
    }
    
    bitStream->capa--;
}

unsigned char lireBits(BitStream* bitStream, int n) {
    unsigned char resultat = 0;
    
    for (int i = 0; i < n; i++) {

        unsigned char bit = (bitStream->ptr[bitStream->index / 8] >> (7 - bitStream->index % 8)) & 1;
        resultat = (resultat << 1) | bit;

        bitStream->index++;
    }
    
    return resultat;
}