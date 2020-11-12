#ifndef INC_SYMBOL_HEADER
#define INC_SYMBOL_HEADER

#include "uthash.h"
#include "shared.h"

typedef struct symbol_table {
    char lexeme[100];
    char key[100];
    int var_type;
    int arity;
    int pos;
    int type;
    UT_hash_handle hh;
} SymbolTable;

void st_delete();
void st_add_symbol(int symbol, char *name, int arity, int pos, int var_type);
void st_print();

#endif
