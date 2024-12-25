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

    // Extraire le bit de poids faible
    unsigned char rightmostBit = src & 1;

    unsigned char result = src >> 1; 

    result <<= capa - 1;

    if (rightmostBit) {
        result |= (1 << (capa - 1));
    }

    return result;
}

void ecrireBit(BitStream *bs, unsigned char bit) {

    // si l'octet courant est plein on passe à l'octet suivant.
    if (bs->capa == 0) {
        bs->index++;
        bs->capa = 8;
    }

    unsigned char mask = moveRightmostBitToCapa(1, bs->capa);
    if (bit) {
        bs->ptr[bs->index] |= mask;
    }
    
    bs->capa--;
}

