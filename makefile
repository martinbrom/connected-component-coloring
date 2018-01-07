CC = gcc
BIN = ccl.exe
OBJ = image.o disjoint_set.o labelling.o main.o

%.o: %.c
	$(CC) -c $< -o $@

$(BIN): $(OBJ)
	$(CC) $^ -o $@
