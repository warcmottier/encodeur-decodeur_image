CC := gcc
BIN := bin/
SOURCE := src/
INCLUDE := include/
LIB := lib/
OBJ := $(BIN)option.o $(BIN)main.o
EXE := $(BIN)codec
CFLAGS := -std=c17 -pedantic -Wall
PFLAGS := -I$(LIB)$(INCLUDE)
LFLAGS := -Wl,-rpath,$(LIB) -L$(LIB) -lcodec -lm

all : $(BIN) libcodec codec clean

$(BIN):
	mkdir -p $(BIN)

libcodec:
	make -f makelib

codec: $(OBJ)
	$(CC) -o $(EXE) $^ $(LFLAGS)

$(BIN)main.o: $(SOURCE)main.c $(INCLUDE)option.h
	$(CC) -c $< $(CFLAGS) $(PFLAGS) -o $@

$(BIN)option.o: $(SOURCE)option.c $(INCLUDE)option.h $(LIB)$(INCLUDE)codec.h
	$(CC) -c $< $(CFLAGS) $(PFLAGS) -o $@

clean:
	rm -f $(BIN)*.o

mrproper: clean
	rm -f $(EXE)

cleanlib :
	make -f makelib mrproper
