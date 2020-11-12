%debug
%defines
%define parse.error verbose
%define api.pure

%{

    #include "arvore.h"
    #include "../wam/include/main.h"

    extern char file_name[];
    extern int yylineno;
    extern int error_col;

    int register_input = 1;
    int arity = 0;
    int error_last = 0;
    char last[100];

    void check_var();
    void check_cont(char new_str[], int arity);

    typedef struct already_added {
        char name[100];
        UT_hash_handle hh;
    } StrAdded;

    StrAdded *str_table = NULL;
    void hash_add_structure(char *name);
    void hash_structure_delete();

    typedef struct var_table {
        char name[100];
        int var_type;
        int occurrances;
        UT_hash_handle hh;
    } VarTable;

    VarTable *var_table = NULL;
    void hash_add_variable(char *name, int var_type, int occurrances);
    void hash_variable_delete();

    int yyparse(void);
	void yyerror (char const *s);
    int yylex();
%}

%locations

%union {
    char con[100];
    char var[100];
    char svar[100];
    char avar[100];
    struct node_fact *fato;
    struct node_rule *regra;
    struct node_str *str;
    struct node_strs *strs;
    struct node_term *term;
    struct node_args *args;
    struct node_list *list;
}

%token <con> CON
%token <var> VAR
%token <svar> SINGLE_VAR
%token <avar> ANON_VAR
%token RULE_SYM

%type <list> list
%type <fato> fato
%type <regra> regra
%type <str> estrutura
%type <strs> estruturas
%type <term> termo
%type <args> argumentos

%start programa

%destructor { free_list($$);} <list>
%destructor { free_fact($$); hash_variable_delete();} <fato>
%destructor { free_rule($$); hash_variable_delete();} <regra>
%destructor { free_str($$); } <str>
%destructor { free_strs($$); } <strs>
%destructor { free_term($$); } <term>
%destructor { free_args($$); } <args>

%%

programa:
    predicado {hash_structure_delete();  hash_variable_delete();}
;

predicado:
    clausula
    | predicado clausula
;

clausula:
    fato {
            print_header("Fato");
            print_fact($1, 0);
            print_footer();
            check_cont($1->one->nome, $1->one->arity);
            check_var();
            hash_variable_delete();
            flatten_fact($1);
            // free_fact($1);
        }
    | regra {
              print_header("Regra");
              print_rule($1, 0);
              print_footer();
              check_cont($1->one->nome, $1->one->arity);
              check_var();
              hash_variable_delete();
              free_rule($1);
            }
    | error '.' { yyerrok; }
;

fato:
    estrutura '.' {$$ = new_node_fact($1, '.', yylineno);}
;

regra:
    estrutura RULE_SYM estruturas '.' {
        $$ = new_node_rule($1, $3, '.', yylineno);
    }
;

estruturas:
    estrutura {$$ = new_node_strs($1, NULL, '\0');}
    | estrutura ',' estruturas {$$ = new_node_strs($1, $3, ',');}
;

estrutura:
    CON {$$ = new_node_str(NULL, $1, '\0', arity);st_add_symbol(CON_SYMBOL, $1, 0, yylineno, 0);}
    | CON '(' argumentos ')' {
                                    $$ = new_node_str($3, $1, ')', arity);
                                    strcat($1, "/");
                                    char temp[100];
                                    sprintf(temp,"%d",arity);
                                    strcat($1, temp);
                                    st_add_symbol(STR_SYMBOL, $1, arity, yylineno, 0);
                                    arity = 0;
                            }
;

argumentos:
    termo {arity += 1; $$ = new_node_args($1, NULL, '\0');}
    | termo ',' argumentos {arity += 1; $$ = new_node_args($1, $3, ',');}
;

termo:
    estrutura {$$ = new_node_term($1, NULL, "\0", STR_SYMBOL, 0);}
    | VAR {
            hash_add_variable($1, BASIC_VAR, 1);
            $$ = new_node_term(NULL, NULL, $1, REF_SYMBOL, BASIC_VAR);
            st_add_symbol(REF_SYMBOL, $1, 0, yylineno, BASIC_VAR);
        }
    | SINGLE_VAR {
                  hash_add_variable($1, SINGLETON_VAR, 1);
                  $$ = new_node_term(NULL, NULL, $1, REF_SYMBOL, SINGLETON_VAR);
                  st_add_symbol(REF_SYMBOL, $1, 0, yylineno, SINGLETON_VAR);
                }
    | ANON_VAR  {$$ = new_node_term(NULL, NULL, $1, REF_SYMBOL, ANONYM_VAR);}
    | list {$$ = new_node_term(NULL, $1, "\0", LIS_SYMBOL, 0);}
;

list:
    '[' ']' {$$ = new_node_list(NULL, NULL);}
    | '[' termo ']' {$$ = new_node_list($2, NULL);}
    | '[' termo '|' termo ']' {$$ = new_node_list($2, $4);}
;

%%

void hash_add_variable(char *name, int var_type, int occurrances) {

    VarTable *s;

    HASH_FIND_STR(var_table, name, s);

    if(s==NULL){
        s = (VarTable*)malloc(sizeof(VarTable));
        strcpy(s->name, name);
        s->var_type = var_type;
        s->occurrances = occurrances;
        HASH_ADD_STR(var_table, name, s);
    } else {
        s->occurrances += 1;
    }
}

void hash_variable_delete() {
  VarTable *current_table, *tmp;

  HASH_ITER(hh, var_table, current_table, tmp) {
    HASH_DEL(var_table,current_table);
    free(current_table);
  }
}

void hash_add_structure(char *name) {

    StrAdded *s;

    HASH_FIND_STR(str_table, name, s);

    if(s==NULL){
        s = (StrAdded*)malloc(sizeof(StrAdded));
        strcpy(s->name, name);
        HASH_ADD_STR(str_table, name, s);
    }
}

void hash_structure_delete() {
  StrAdded *current_table, *tmp;

  HASH_ITER(hh, str_table, current_table, tmp) {
    HASH_DEL(str_table,current_table);
    free(current_table);
  }
}

void check_var(){
    VarTable *v;
    for(v=var_table; v!=NULL; v=v->hh.next) {
        if ((v->var_type == BASIC_VAR)&&(v->occurrances == 1)) {
            printf("WARNING: %s:%d:%d: Semantic error, singleton variables: [%s]",  file_name, yylineno, error_col, v->name);
        }
        if ((v->var_type == SINGLETON_VAR)&&(v->occurrances > 1)){
            printf("WARNING: %s:%d:%d: Semantic error, singleton−marked variable appears more than once: %s",  file_name, yylineno, error_col, v->name);
        }
    }
}

void check_cont(char new_str[], int arity){
    StrAdded *s;
    strcat(new_str, "/");
    char temp[100];
    sprintf(temp,"%d",arity);
    strcat(new_str, temp);
    HASH_FIND_STR(str_table, new_str, s);
    if (s == NULL) {
        hash_add_structure(new_str);
        strcpy(last, new_str);
    } else {
        if (strcmp(new_str, last) != 0||error_last == 1){
            printf("WARNING: %s:%d:%d: Semantic error, clauses of %s are not together in the source-file",  file_name, yylineno, error_col, new_str);
            strcpy(last, new_str);
            error_last = 1;
        } else {
            strcpy(last, new_str);
            error_last = 0;
        }
    }
}

