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

void pushbits(BitStream* curr, unsigned char src){
    if(curr->capa == CHAR_BIT){
      curr->capa = 0;
      curr->index++;
    }
  curr->ptr[curr->index] |= (src << (CHAR_BIT - 1 - curr->capa));
  curr->capa++;
}
