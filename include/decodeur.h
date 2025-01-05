#ifndef _DECODEUR_
#define _DECODEUR_

#include <stdio.h>
#include <stdlib.h>
#include "quadtree.h"
#include "gereBit.h"

/**
 * @brief algorythme d'encodage d'un fichier source PGM pour en faire un fichier destination qtc compresser
 * 
 * @param nomEntrer 
 * @param nomSortie 
 * @param grille 
 * @param verbeux 
 */
void decode(char* nomEntrer, char* nomSortie, int grille, int verbeux);

#endif