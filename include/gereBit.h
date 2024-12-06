#ifndef __GEREBIT__
#define __GEREBIT__

#include <stdio.h>
#include <stdlib.h>

typedef struct { 
    unsigned char* ptr; // tableau de la taille de l'image 
    size_t capa; // capaciter restant sur le byte
} BitStream;

/**
 * @brief ecrit les bit de src dans curr
 * 
 * @param curr 
 * @param src 
 * @param nbit 
 * @return size_t 
 */
size_t pushbits(BitStream* curr, unsigned char src, size_t nbit);

#endif
