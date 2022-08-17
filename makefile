CC=clang
CFLAGS=-I .
DEPS = computer.h inputbuffering.h
OBJ = computer.c inputbuffering.c

$.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

computer: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean: 
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~