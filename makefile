CC=clang
CFLAGS=-I.
DEPS = computer.h inputbuffering.h instr.h
OBJ = computer.o inputbuffering.o instr.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hellomake: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
