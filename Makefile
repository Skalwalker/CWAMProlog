ROOT=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))


UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LINKER = -lfl
endif
ifeq ($(UNAME_S),Darwin)
	LINKER = -ll
endif

IDIR=src/wam/include
CC=gcc
CFLAGS= -std=gnu11 -Wall -Wextra -pedantic -g

ODIR=obj
LDIR=src/wam/lib

LIBS=-lm -I $(IDIR) $(LINKER)


_DEPS = instruction_set.h memory.h uthash.h shared.h symbol_table.h main.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))


_OBJ = instruction_set.o memory.o shared.o symbol_table.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(LDIR)/%.c src/parser/cwam_parser.h $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: bison_flex bin/main

bison_flex:
	bison src/parser/prolog_bison.y --output=src/parser/cwam_parser.c
	flex -o src/lexical/cwam_lex.yy.c src/lexical/prolog_lex.l


bin/main: $(OBJ)
	$(CC) -c -o obj/cwam_lex.yy.o src/lexical/cwam_lex.yy.c $(CFLAGS)
	$(CC) -c -o obj/cwam_parser.o src/parser/cwam_parser.c $(CFLAGS)
	# $(CC) -o bin/parser obj/cwam_lex.yy.o obj/cwam_parser.o $(CFLAGS)
	$(CC) -o $@ $^ obj/cwam_parser.o obj/cwam_lex.yy.o $(CFLAGS) $(LIBS)


.PHONY: clean bison_flex bin/main

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
