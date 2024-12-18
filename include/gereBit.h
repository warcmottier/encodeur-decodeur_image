#ifndef __GEREBIT__
#define __GEREBIT__

#include <stdio.h>
#include <stdlib.h>

typedef struct { 
    unsigned char* ptr; // tableau de la taille de l'image 
    int index; // l'indice actuelle du tableau
    int tailleTotal;
    size_t capa; // capaciter restant sur le byte
} BitStream;

/**
 * @brief ecrit les bit de src dans curr
 * 
 * @param curr 
 * @param src 
 * @return  
 */
void pushbits(BitStream* curr, unsigned char src);

#endif
