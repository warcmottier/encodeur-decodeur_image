#ifndef _ALGO_
#define _ALGO_

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief algorythme d'encodage d'un fichier source PGM pour en faire un fichier destination qtc compresser
 * 
 * @param source 
 * @param destination 
 */
void encode(FILE* source, FILE* destination);

/**
 * @brief deocde un fichier source qtc compresser pour en faire un fichier destination PGM non compresser
 * 
 * @param source 
 * @param destination 
 */
void decode(char* source, char* destination);

#endif