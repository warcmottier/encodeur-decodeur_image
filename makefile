CC := gcc
CFLAGS := -std=c17 -pedantic -Wall
LDFLAGS := -lm
BIN := bin/
SOURCE := src/
INCLUDE := include/
OBJ := $(BIN)option.o $(BIN)encodeur.o $(BIN)decodeur.o $(BIN)main.o $(BIN)quadtree.o $(BIN)gereBit.o
EXE := $(BIN)codec

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(BIN)main.o: $(SOURCE)main.c $(INCLUDE)option.h
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN)option.o: $(SOURCE)option.c $(INCLUDE)option.h $(INCLUDE)lib.h
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN)encodeur.o: $(SOURCE)encodeur.c $(INCLUDE)encodeur.h $(INCLUDE)quadtree.h $(INCLUDE)gereBit.h
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN)decodeur.o: $(SOURCE)decodeur.c $(INCLUDE)decodeur.h $(INCLUDE)quadtree.h $(INCLUDE)gereBit.h
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN)quadtree.o: $(SOURCE)quadtree.c $(INCLUDE)quadtree.h $(INCLUDE)gereBit.h
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN)gereBit.o: $(SOURCE)gereBit.c $(INCLUDE)gereBit.h
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(BIN)*.o

mrproper: clean
	rm -f $(EXE)
