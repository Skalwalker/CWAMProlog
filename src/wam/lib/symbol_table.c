#include "../include/symbol_table.h"

extern SymbolTable *s_table;


void create_key(char *key, char *name, int pos){
  key[0] = '\0';
  char lineno[100];
  strcat(key, name);
  strcat(key, "::");
  sprintf(lineno, "%d", pos);
  strcat(key, lineno);
}

SymbolTable* create_symbol(char *key, int symbol, char *name, int var_type, int arity, int pos, int occ) {
  SymbolTable *s;
  s = (SymbolTable*)malloc(sizeof(SymbolTable));
  strcpy(s->key, key);
  s->type = symbol;
  strcpy(s->lexeme, name);
  s->var_type = var_type;
  s->arity = arity;
  s->pos = pos;
  s->occ = occ;
  return s;
}

int st_add_symbol(int symbol, char *name, int arity, int pos, int var_type) {

    SymbolTable *s;
    char key[100];
    create_key(key, name, pos);
    printf("%s", key);

    HASH_FIND_STR(s_table, key, s);

    if(s==NULL){
        s = create_symbol(key, symbol, name, var_type, arity, pos, 1);
        HASH_ADD_STR(s_table, key, s);
    } else {
        s->occ += 1;
        if (symbol == STR_SYMBOL){
          char newkey[100];
          char newname[100];
          char occstr[20];
          newname[0] = '\0';
          strcat(newname, name);
          strcat(newname, ":");
          sprintf(occstr, "%d", s->occ);
          strcat(newname, occstr);
          printf("%s", newname);
          create_key(newkey, newname, pos);
          s = create_symbol(newkey, symbol, newname, var_type, arity, pos, s->occ);
          HASH_ADD_STR(s_table, key, s);
          return s->occ;
        }
    }

    return 0;
}

void st_print() {
    SymbolTable *s;
    printf("\n===================Tabela de Simbolos===================\n");
    for(s=s_table; s != NULL; s=s->hh.next) {
        char token[4];
        datatype_token_to_str(token, s->type);
        printf("(Chave: %s | Valor: %s | Tipo: %s | Posição: %d | InClauseOcc: %d", s->key, s->lexeme, token, s->pos, s->occ);
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
