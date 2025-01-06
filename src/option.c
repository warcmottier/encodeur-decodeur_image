#define _POSIX_C_SOURCE 200809L
#include "../include/option.h"
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

/**
 * @brief Initialise les options a leurs valeurs par défaut
 * 
 * @return Option 
 */
Option init_Option(){
    Option opt;
    opt.c = 0;
    opt.u = 0;
    opt.nomEntrer = NULL;
    opt.nomSortie = NULL;
    opt.g = 0;
    opt.v = 0;
    opt.flag_okay = 1;
    opt.o = 0;
    return opt;
}

void help(){
    printf("-----------------Argument de départ (Un seul parmis les 3)-----------------\n");
    printf("-h : affiche ce message d'aide\n"); 
    printf("-c : mets le sens de traitement en \"Encodage\" (pgm -> qtc)\n");
    printf("-u : mets le sens de traitement en \"Décodage\" (qtc -> pgm)\n");
    printf("-----------------Argument Obligatoire-----------------\n");
    printf("-i nom_de_fichier.{pgm|qtc} (pgm si vous avez choisis d'encoder | qtc si vous avez choisis de décoder) : encode/decode le fichier nom_de_fichier\n");
    printf("-----------------Autres arguments-----------------\n");
    printf("-o nom_de_fichier.{qtc|pgm} (qtc si vous avez choisis d'encoder | pgm si vous avez choisis de décoder) : place le fichier encodé/décocdé dans nom_de_fichier\n");
    printf("-g : édite la grille de ségmentation pour le décodeur/encodeur\n");
    printf("-v : mode verbose\n");
}

int check_file_extension(const char* filename, const char* extension) {
    size_t len_filename = strlen(filename);
    size_t len_extension = strlen(extension);

    if (len_filename < len_extension) {
        return 0; // Trop court pour contenir le suffixe
    }

    // Vérifie si la fin de filename correspond à extension
    return strcmp(filename + len_filename - len_extension, extension) == 0;
}

void choixOption(Option* opt, int argc, char* argv[]) {
    int opt_char; 
    while ((opt_char = getopt(argc, argv, "cui:o:gvh")) != -1) {
        switch (opt_char) {
            case 'c':
                opt->c = 1;
                break;
            case 'u':
                opt->u = 1;
                break;
            case 'i':
                opt->nomEntrer = strdup(optarg);
                break;
            case 'o':
                opt->o = 1;
                opt->nomSortie = strdup(optarg);
                break;
            case 'g':
                opt->g = 1;
                break;
            case 'v':
                opt->v = 1;
                break;
            case 'h':
                help();
                opt->flag_okay = 0;
                return;
            default:
                fprintf(stderr, "Erreur : Option inconnue.\nUtilisez -h pour afficher l'aide.\n");
                exit(EXIT_FAILURE);
        }
    }

    // Validation des options obligatoires
    if (!opt->c && !opt->u) {
        fprintf(stderr, "Erreur : L'une des options -c ou -u est obligatoire.\n");
        exit(EXIT_FAILURE);
    }

    if (opt->c && opt->u) {
        fprintf(stderr, "Erreur : Vous ne pouvez pas utiliser -c et -u simultanément.\n");
        exit(EXIT_FAILURE);
    }

    if (opt->nomEntrer == NULL) {
        fprintf(stderr, "Erreur : L'option -i est obligatoire.\n");
        exit(EXIT_FAILURE);
    }

    // Vérification de l'extension du fichier d'entrée
    if (opt->c && !check_file_extension(opt->nomEntrer, ".pgm")) {
        fprintf(stderr, "Erreur : Avec l'option -c, le fichier d'entrée doit avoir l'extension .pgm.\n");
        exit(EXIT_FAILURE);
    }

    if (opt->u && !check_file_extension(opt->nomEntrer, ".qtc")) {
        fprintf(stderr, "Erreur : Avec l'option -u, le fichier d'entrée doit avoir l'extension .qtc.\n");
        exit(EXIT_FAILURE);
    }

     // Vérification de l'extension du fichier d'entrée
    if(opt->nomSortie != NULL){
        if (opt->c && !check_file_extension(opt->nomSortie, ".qtc")) {
            fprintf(stderr, "Erreur : Avec l'option -o, le fichier de sortie doit avoir l'extension .pgm si vous avez mis l'option -c.\n");
            exit(EXIT_FAILURE);
        }

        if (opt->u && !check_file_extension(opt->nomSortie, ".pgm")) {
            fprintf(stderr, "Erreur : Avec l'option -o, le fichier de doit avoir l'extension .qtc si vous avez mis l'option -u.\n");
            exit(EXIT_FAILURE);
        }
    }
    else{ // Définir le fichier de sortie par défaut si non spécifié
        if (opt->c) {
            opt->nomSortie = strdup("QTC/out.qtc");
        } else if (opt->u) {
            opt->nomSortie = strdup("PGM/out.pgm");
        }
    }
    // Affichage du mode verbeux
    if (opt->v) {
        printf("Options configurées :\n");
        printf("  Mode : %s\n", opt->c ? "Encodeur" : "Décodeur");
        printf("  Fichier d'entrée : %s\n", opt->nomEntrer);
        printf("  Fichier de sortie : %s\n", opt->nomSortie);
        printf("  Grille : %s\n", opt->g ? "Activée" : "Désactivée");
    }
}

void lanceAlgo(Option opt){
    if(opt.flag_okay == 0)
        return;
    
    if(opt.c){
        codage(opt.nomEntrer, opt.nomSortie, opt.g, opt.v);
    }
    else if(opt.u){
        decode(opt.nomEntrer, opt.nomSortie, opt.g, opt.v);
    }
}
