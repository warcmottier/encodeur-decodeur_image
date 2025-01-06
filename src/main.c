
#include "../include/option.h"
int main(int argc, char* argv[]){
    Option opt = init_Option();
    choixOption(&opt, argc, argv);
    lanceAlgo(opt);
    return 0;
}