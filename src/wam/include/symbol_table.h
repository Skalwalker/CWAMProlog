#include "uthash.h"
#include "shared.h"

typedef struct symbol_table {
    char lexeme[100];
    int type;
    UT_hash_handle hh;
} SymbolTable;

void st_add_symbol(int symbol, char *name);
void st_print();
