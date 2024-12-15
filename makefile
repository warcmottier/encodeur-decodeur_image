CC = gcc
CFLAGS =-std=c17 -pedantic -Wall
LDFLAGS = -lm
OBJ = option.o encodeur.o decodeur.o main.o quadtree.o
EXE = codec
SOURCE = src/
INCLUDE = include/

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	
main.o: $(SOURCE)main.c $(INCLUDE)option.h $(INCLUDE)encodeur.h

option.o: $(SOURCE)option.c $(INCLUDE)option.h $(INCLUDE)encodeur.h $(INCLUDE)decodeur.h

encodeur.o: $(SOURCE)encodeur.c $(INCLUDE)encodeur.h $(INCLUDE)quadtree.h

decodeur.o : $(SOURCE)decodeur.c $(INCLUDE)decodeur.h $(INCLUDE)quadtree.h

quadtree.o: $(SOURCE)quadtree.c $(INCLUDE)quadtree.h

%.o: $(SOURCE)%.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o

mrproper: clean
	rm -f $(EXE)