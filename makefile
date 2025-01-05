CC := gcc
STD := -std=c17
SRC := src/
LIB := lib/
TEST := test_unitaire/
BIN := bin/

PFLAGS := -I$(LIB)include
CFLAGS := -Wall -O2
LFLAGS := -lm -Wl,-rpath,$(LIB) -L$(LIB) -lmtrack
INCLUDE := -include $(LIB)include/mtrack.h

all: $(BIN) libmtrack.so test1 test2 test3 test4 test5 test6 test7

$(BIN):
	mkdir -p $(BIN)

libmtrack.so:
	make -f makelib

test1:
	$(CC) $(STD) $(TEST)test1.c $(LFLAGS) $(PFLAGS) $(CFLAGS) $(INCLUDE) -o $(BIN)test1

test2:
	$(CC) $(STD) $(TEST)test2.c $(LFLAGS) $(PFLAGS) $(CFLAGS) $(INCLUDE) -o $(BIN)test2

test3:
	$(CC) $(STD) $(TEST)test3.c $(LFLAGS) $(PFLAGS) $(CFLAGS) $(INCLUDE) -o $(BIN)test3

test4:
	$(CC) $(STD) $(TEST)test4.c $(LFLAGS) $(PFLAGS) $(CFLAGS) $(INCLUDE) -o $(BIN)test4

test5:
	$(CC) $(STD) $(TEST)test5.c $(LFLAGS) $(PFLAGS) $(CFLAGS) $(INCLUDE) -o $(BIN)test5

test6:
	$(CC) $(STD) $(TEST)test6.c $(LFLAGS) $(PFLAGS) $(CFLAGS) $(INCLUDE) -o $(BIN)test6

test7:
	$(CC) $(STD) $(TEST)test7.c $(LFLAGS) $(PFLAGS) $(CFLAGS) $(INCLUDE) -o $(BIN)test7

make clean:
	rm -f $(BIN)test1 $(BIN)test2 $(BIN)test3 $(BIN)test4 $(BIN)test5 $(BIN)test6 $(BIN)test7
	rm -r $(BIN)

make mrproper: clean
	make -f makelib clean

