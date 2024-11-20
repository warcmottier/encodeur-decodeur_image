CC = gcc
CFLAGS =-std=c17 -pedantic -Wall
LDFLAGS = -lMLV
OBJ = option.o algo.o main.o
EXE = codec
SOURCE = src/
iNCLUDE = include/

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	
main.o: $(SOURCE)main.c $(iNCLUDE)option.h

option.o: $(SOURCE)option.c $(iNCLUDE)option.h $()

algo.o: $(SOURCE)algo.c $(iNCLUDE)algo.h

quadtree.o: $(SOURCE)quadtree.c $(INCLUDE)quadtree.h

%.o: $(SOURCE)%.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o

mrproper: clean
	rm -f $(EXE)