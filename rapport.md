# Rapport projet
## Pottier Marc et Depaoli Ugo TD 2

### compilation et lancement

pour compiler l'executable et la lib qui va avec vous avez juste a faire la commande
```make```

si vous voulez compiler juste la lib vous devez faire la commande 
```make libcodec```

si la lib est deja compiler et que vous voulez compiler juste l'executable vous faite la commande

```make codec```

le fichier executable se trouve dans le dossier bin

voici les differente option que vous avez pour lancer le code :

- le sens de traitement ( -c : encodeur, -u décodeur)

- le nom du fichier d’entrée -i input.{pgm|qtc} (en cohérence avec l’option précédente)

- le nom du fichier de sortie :

- par défaut le fichier de sortie sera nommé {QTC|PGM}/out. {qtc|pgm}

- pour renommer le fichier de sortie on utilisera l’option -o output.{qtc|pgm}

- édition de la grille de segmentation ( -g ) pour le codeur et le décodeur.

- les classiques : -h (aide), -v (mode verbeux)

exemple de commande :

```./bin/codec -c -g -i PGM/toto.pgm -o QTC/toto.qtc```

### repartition du travaille

Marc Pottier :
- ecriture et lecture de pgm 
- quadtree
- lecture ecriture de qtc

Depaoli Ugo :
- lecture ecriture de qtc
- option

