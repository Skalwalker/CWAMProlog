#include "../include/symbol_table.h"

extern SymbolTable *s_table;

void st_add_symbol(int symbol, char *name) {

    SymbolTable *s;

    HASH_FIND_STR(s_table, name, s);

    if(s==NULL){
        s = (SymbolTable*)malloc(sizeof(SymbolTable));
        s->type = symbol;
        strcpy(s->lexeme, name);
        HASH_ADD_STR(s_table, lexeme, s);
    }
}

void st_print() {
    SymbolTable *s;
    printf("\n======================Symbol Table======================\n");
    for(s=s_table; s != NULL; s=s->hh.next) {
        // printf("\n= Entry:\n= Lexeme: %s\n", s->lexeme);
        char token[4];
        switch (s->type) {
            case 1:
                strcpy(token, "REF\0");
                break;

            case 2:
                strcpy(token, "STR\0");
                break;

            case 3:
                strcpy(token, "CON\0");
                break;

            case 4:
                strcpy(token, "LIS\0");
                break;

            default:
                break;
        }

        printf("Key: %20s | Lexeme: %20s\n", s->lexeme, token);
    }
    printf("========================================================\n");
}

void st_delete() {
  SymbolTable *current_table, *tmp;

  HASH_ITER(hh, s_table, current_table, tmp) {
    HASH_DEL(s_table,current_table);  /* delete; users advances to next */
    free(current_table);            /* optional- if you want to free  */
  }
}

