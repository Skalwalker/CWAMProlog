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
    printf("=========Symbol Table=========");
    for(s=s_table; s != NULL; s=s->hh.next) {
        printf("\n= Entry:\n= Lexeme: %s\n", s->lexeme);
        switch (s->type)
        {
        case 1:
            printf("= Type: REF\n=");
            break;

        case 2:
            printf("= Type: STR\n=");
            break;

        case 3:
            printf("= Type: CON\n=");
            break;

        case 4:
            printf("= Type: LIS\n=");
            break;

        default:
            break;
        }
    }
    printf("==============================\n");
}


// void add_str(int user_id, char *name) {
//     struct my_struct *s;

//     s = malloc(sizeof(struct my_struct));
//     s->id = user_id;
//     strcpy(s->name, name);
//     HASH_ADD_INT( users, id, s );  /* id: name of key field */
// }

// void add_list(int user_id, char *name) {
//     struct my_struct *s;

//     s = malloc(sizeof(struct my_struct));
//     s->id = user_id;
//     strcpy(s->name, name);
//     HASH_ADD_INT( users, id, s );  /* id: name of key field */
// }

