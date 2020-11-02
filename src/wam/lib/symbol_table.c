#include "../include/symbol_table.h"

extern SymbolTable *s_table;

void st_add_symbol(int symbol, char *name, int arity, int pos, int var_type) {

    SymbolTable *s;

    HASH_FIND_STR(s_table, name, s);

    if(s==NULL){
        s = (SymbolTable*)malloc(sizeof(SymbolTable));
        char key[100];
        key[0] = '\0';
        char lineno[100];
        strcat(key, name);
        strcat(key, "::");
        sprintf(lineno, "%d", arity);
        strcat(key, lineno);
        strcpy(s->key, key);
        s->type = symbol;
        s->occurences = 1;
        strcpy(s->lexeme, name);
        s->var_type = var_type;
        s->arity = arity;
        s->pos = pos;
        HASH_ADD_STR(s_table, key, s);
    }
}

void st_print() {
    SymbolTable *s;
    printf("\n===================Tabela de Simbolos===================\n");
    for(s=s_table; s != NULL; s=s->hh.next) {
        char token[4];
        datatype_token_to_str(token, s->type);
        printf("(Chave: %s | Valor: %s | Tipo: %s | Posição: %d", s->key, s->lexeme, token, s->pos);
        if (s->type == REF_SYMBOL){
          char var_type[20];
          vartype_token_to_str(var_type, s->var_type);
          printf(" | Var_Type: %s", var_type);
        }
        if (s->arity > 0) {
          printf(" | Aridade: %d", s->arity);
        }
        printf(")\n");
    }
    printf("========================================================\n");
}

void st_delete() {
  SymbolTable *current_table, *tmp;

  HASH_ITER(hh, s_table, current_table, tmp) {
    HASH_DEL(s_table,current_table);
    free(current_table);
  }
}
