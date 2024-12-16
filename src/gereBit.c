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

size_t pushbits(BitStream* curr, unsigned char src, size_t nbit){
    int diff = 0;
    size_t totalBit = 0;

    if(curr->capa == 0){
        curr->capa = CHAR_BIT;
        curr->ptr++;
    }

    if(curr->ptr)
        return totalBit;
    
    if(curr->capa <= nbit){
        for(int i = 0; i < nbit; i++){
            curr->capa--;
            setbit(curr->ptr, CHAR_BIT - curr->capa, getbit(src, i));
            totalBit++;
        }
    }
    else{
        diff = nbit - curr->capa;
        for(int i = 0; i < nbit; i++){
            curr->capa--;
            setbit(curr->ptr, CHAR_BIT - curr->capa, getbit(src, i));
            if(nbit == diff){
                curr->capa = CHAR_BIT;
                curr->ptr++;
                if(!curr->ptr)
                    return totalBit;
            }
            totalBit++;
        }
    }
}
