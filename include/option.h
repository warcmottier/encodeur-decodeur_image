#ifndef _OPTION_
#define _OPTION_

typedef struct option{
    int c; // flag encodeur
    int u; // flag decodeur
    char* nomEntrer; // nom du fichier d'entrer
    char* nomSortie; // nom du fichier cree doit valoir {QTC|PGM}/out.{qtc|pgm} par defaut
    int g; // affichier la grille 1 si oui 0 sinon
    int v; // mode verbeux
}Option;


/**
 * @brief initialise les option
 * 
 * @return Option 
 */
Option init_Option();

/**
 * @brief remplie les option
 * 
 * @param option 
 * @param argc 
 * @param argv 
 */
void choixOption(Option* option, int argc, char*argv[]);

/**
 * @brief lance les algo selon les option
 * 
 * @param option 
 */
void lanceAlgo(Option option);

#endif