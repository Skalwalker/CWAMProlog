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

    int reg_input = 1;
    int arity = 0;
    int error_last = 0;
    char last[100];

    void check_var();
    void check_cont(char new_str[]);

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
    TreeNode *node;
}

%token <con> CON
%token <var> VAR
%token <svar> SINGLE_VAR
%token <avar> ANON_VAR
%token RULE_SYM

%type <node> list
%type <node> fato
%type <node> regra
%type <node> estrutura
%type <node> estruturas
%type <node> termo
%type <node> argumentos

%destructor { free_tree($$); } <node>

%start programa

%%

programa:
    predicado {
        hash_structure_delete();
        hash_variable_delete();
    }
;

predicado:
    clausula
    | predicado clausula
;

clausula:
    fato {
        print_header("Fato");
        print_tree($1, 0);
        print_footer();
        check_cont($1->left->str_data->nome);
        check_var();
        hash_variable_delete();
        register_names($1);
        print_registers();
        // RegStream *stream = NULL;
        token_stream($1);
        print_stream();
        free_tree($1);
    }
    | regra {
        print_header("Regra");
        print_tree($1, 0);
        print_footer();
        check_cont($1->left->str_data->nome);
        check_var();
        hash_variable_delete();
        free_tree($1);
    }
    | error '.' { yyerrok; }
;

fato:
    estrutura '.' {
        $$ = new_tree_node($1, NULL, NODE_FACT, NULL, NULL);
    }
;

regra:
    estrutura RULE_SYM estruturas '.' {
        $$ = new_tree_node($1, $3, NODE_RULE, NULL, NULL);
    }
;

estruturas:
    estrutura {
        $$ = new_tree_node($1, NULL, NODE_STRS, NULL, NULL);
    }
    | estrutura ',' estruturas {
        $$ = new_tree_node($1, $3, NODE_STRS, NULL, NULL);
    }
;

estrutura:
    CON {
        StrData *str = new_str($1, 0);
        $$ = new_tree_node(NULL, NULL, NODE_STR, NULL, str);
        st_add_symbol(CON_SYMBOL, $1, 0, yylineno, 0);
    }
    | CON '(' argumentos ')' {
        strcat($1, "/");
        char temp[100];
        sprintf(temp,"%d",arity);
        strcat($1, temp);
        StrData *str = new_str($1, arity);
        $$ = new_tree_node($3, NULL, NODE_STR, NULL, str);
        st_add_symbol(STR_SYMBOL, $1, arity, yylineno, 0);
        arity = 0;
    }
;

argumentos:
    termo {
        arity += 1;
        $$ = new_tree_node($1, NULL, NODE_ARGS, NULL, NULL);
    }
    | termo ',' argumentos {
        arity += 1;
        $$ = new_tree_node($1, $3, NODE_ARGS, NULL, NULL);
    }
;

termo:
    estrutura {
        $$ = new_tree_node($1, NULL, NODE_TERM, NULL, NULL);
    }
    | VAR {
        TermData *term = new_term($1, REF_SYMBOL, BASIC_VAR);
        hash_add_variable($1, BASIC_VAR, 1);
        $$ = new_tree_node(NULL, NULL, NODE_TERM, term, NULL);
        st_add_symbol(REF_SYMBOL, $1, 0, yylineno, BASIC_VAR);
    }
    | SINGLE_VAR {
        TermData *term = new_term($1, REF_SYMBOL, SINGLETON_VAR);
        hash_add_variable($1, SINGLETON_VAR, 1);
        $$ = new_tree_node(NULL, NULL, NODE_TERM, term, NULL);
        st_add_symbol(REF_SYMBOL, $1, 0, yylineno, SINGLETON_VAR);
        reg_input += 1;
    }
    | ANON_VAR  {
        TermData *term = new_term($1, REF_SYMBOL, ANONYM_VAR);
        $$ = new_tree_node(NULL, NULL, NODE_TERM, term, NULL);
        reg_input += 1;
    }
    | list {
        $$ = new_tree_node($1, NULL, NODE_TERM, NULL, NULL);
    }
;

list:
    '[' ']' {
        $$ = new_tree_node(NULL, NULL, NODE_LIS, NULL, NULL);
    }
    | '[' termo ']' {
        $$ = new_tree_node($2, NULL, NODE_LIS, NULL, NULL);
    }
    | '[' termo '|' termo ']' {
        $$ = new_tree_node($2, $4, NODE_LIS, NULL, NULL);
    }
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
            printf("WARNING: %s:%d:%d: Semantic error, singleton variables: [%s]\n",  file_name, yylineno, error_col, v->name);
        }
        if ((v->var_type == SINGLETON_VAR)&&(v->occurrances > 1)){
            printf("WARNING: %s:%d:%d: Semantic error, singleton−marked variable appears more than once: %s\n",  file_name, yylineno, error_col, v->name);
        }
    }
}

void check_cont(char new_str[]){
    StrAdded *s;
    HASH_FIND_STR(str_table, new_str, s);
    if (s == NULL) {
        hash_add_structure(new_str);
        strcpy(last, new_str);
    } else {
        if (strcmp(new_str, last) != 0||error_last == 1){
            printf("WARNING: %s:%d:%d: Semantic error, clauses of %s are not together in the source-file\n",  file_name, yylineno, error_col, new_str);
            strcpy(last, new_str);
            error_last = 1;
        } else {
            strcpy(last, new_str);
            error_last = 0;
        }
    }
}

