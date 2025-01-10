#ifndef _OPTION_
#define _OPTION_

#include "../lib/include/codec.h"


typedef struct {
    int c; // flag encodeur
    int u; // flag decodeur
    char* nomEntrer; // nom du fichier d'entrer
    char* nomSortie; // nom du fichier cree doit valoir {QTC|PGM}/out.{qtc|pgm} par defaut
    int g; // affichier la grille 1 si oui 0 sinon
    int v; // mode verbeux
    int o; // si il y a eu un renomage
    double alpha; // alpha du filtrage
}Option;

/**
 * @brief initialise option
 * 
 * @return Option 
 */
Option init_Option();

/**
 * @brief choisi les option 
 * 
 * @param opt 
 * @param argc 
 * @param argv 
 */
void choixOption(Option* opt, int argc, char* argv[]);

/**
 * @brief Lance les algos selon les options rentrées
 * 
 * @param option 
 * @param argc 
 * @param argv 
 */
void lanceAlgo(Option opt);

#endif
