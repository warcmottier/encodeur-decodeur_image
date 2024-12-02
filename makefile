CC = gcc
CFLAGS =-std=c17 -pedantic -Wall
LDFLAGS = -lm
OBJ = option.o algo.o main.o quadtree.o
EXE = codec
SOURCE = src/
INCLUDE = include/

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	
main.o: $(SOURCE)main.c $(INCLUDE)option.h

option.o: $(SOURCE)option.c $(INCLUDE)option.h $(INCLUDE)algo.h

algo.o: $(SOURCE)algo.c $(INCLUDE)algo.h $(INCLUDE)quadtree.h

quadtree.o: $(SOURCE)quadtree.c $(INCLUDE)quadtree.h

%.o: $(SOURCE)%.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o

mrproper: clean
	rm -f $(EXE)