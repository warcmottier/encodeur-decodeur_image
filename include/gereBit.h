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
size_t pushbits(BitStream* curr, unsigned char src, size_t nbit);

/**
 * @brief affiche un octet
 * 
 * @param n 
 */
void afficherBinaire8Bits(unsigned char n);

/**
 * @brief ecrit un seul bit dans l'octet courant du bitstream
 * 
 * @param bs 
 * @param bit 
 */
void ecrireBit(BitStream *bitStream, unsigned char bit);

/**
 * @brief lis le nombre de bit n voulue dans bs et renvoie ces dernier
 * 
 * @param bs 
 * @param n 
 * @return unsigned char 
 */
unsigned char lireBits(BitStream* bitStream, int n);

#endif
