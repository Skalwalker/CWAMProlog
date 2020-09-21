ROOT=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))

IDIR=src/include
CC=gcc
CFLAGS= -Wall -Wextra -pedantic -Werror -g -I$(IDIR)

ODIR=obj
LDIR=src/lib

LIBS=-lm

_DEPS = instruction_set.h memory.h main.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o instruction_set.o memory.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(LDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

bin/main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
