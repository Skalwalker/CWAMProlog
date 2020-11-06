%debug
%defines
%define parse.error verbose
%define api.pure
// %define api.value.type union

%{
	#include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../wam/include/main.h"

    extern char file_name[];
    extern int yylineno;
    extern int error_col;

    int arity = 0;
    int error_last = 0;
    char last[100];
    void check_var();
    void check_cont(char new_str[], int arity);

    typedef struct node_rule NodeRule;
    typedef struct node_str NodeStr;
    typedef struct node_strs NodeStrs;
    typedef struct node_args NodeArgs;
    typedef struct node_term NodeTerm;
    typedef struct node_fact NodeFact;
    typedef struct node_list NodeList;


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

    struct node_fact {
        char op;
        NodeStr *one;
        int pos;
    };

    struct node_rule {
        char op;
        NodeStr *one;
        NodeStrs *two;
        int pos;
    };

    struct node_str {
        char op;
        NodeArgs *one;
        char nome[100];
        int arity;
    };

    struct node_strs {
        char op;
        NodeStr *one;
        NodeStrs *two;
    };

    struct node_args {
        char op;
        NodeTerm *one;
        NodeArgs *two;

    };

    struct node_term {
        NodeStr *one;
        NodeList *two;
        char nome[100];
        int type;
        int var_type;
    };

    struct node_list {
        NodeTerm *one;
        NodeTerm *two;
    };


    NodeFact* new_node_fact(NodeStr* one, char op, int pos);
    NodeRule* new_node_rule(NodeStr* one, NodeStrs* two, char op, int pos);
    NodeStr* new_node_str(NodeArgs* one, char nome[], char op, int arity);
    NodeStrs* new_node_strs(NodeStr* one, NodeStrs* two, char op);
    NodeArgs* new_node_args(NodeTerm* one, NodeArgs* two, char op);
    NodeTerm* new_node_term(NodeStr* one, NodeList* two, char nome[], int type, int var_type);
    NodeList* new_node_list(NodeTerm* one, NodeTerm* two);

    void print_str(NodeStr *root, int tabs);
    void print_strs(NodeStrs *root, int tabs);
    void print_args(NodeArgs *root, int tabs);
    void print_term(NodeTerm *root, int tabs);
    void print_fact(NodeFact *root, int tabs);
    void print_rule(NodeRule *root, int tabs);
    void print_list(NodeList *root, int tabs);

    void free_fact(NodeFact *root);
    void free_rule(NodeRule *root);
    void free_str(NodeStr *root);
    void free_strs(NodeStrs *root);
    void free_args(NodeArgs *root);
    void free_term(NodeTerm *root);
    void free_list(NodeList *root);

    void print_header(char tipo[]);
    void print_footer();

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
            free_fact($1);
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
    estrutura RULE_SYM estruturas '.' {$$ = new_node_rule($1, $3, '.', yylineno);}
;

estruturas:
    estrutura {$$ = new_node_strs($1, NULL, '\0');}
    | estrutura ',' estruturas {$$ = new_node_strs($1, $3, ',');}
;

estrutura:
    CON {$$ = new_node_str(NULL, $1, '\0', arity);  st_add_symbol(CON_SYMBOL, $1, 0, yylineno, 0);}
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
    | VAR {hash_add_variable($1, BASIC_VAR, 1); $$ = new_node_term(NULL, NULL, $1, REF_SYMBOL, BASIC_VAR);}
    | SINGLE_VAR {hash_add_variable($1, SINGLETON_VAR, 1); $$ = new_node_term(NULL, NULL, $1, REF_SYMBOL, SINGLETON_VAR);}
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


NodeFact* new_node_fact(NodeStr* one, char op, int pos) {
	NodeFact *e = (NodeFact*) malloc(sizeof(NodeFact));
	e->one = one;
    e->op = op;
    e->pos = pos;
	return e;
}

NodeRule* new_node_rule(NodeStr* one, NodeStrs* two, char op, int pos) {
	NodeRule *e = (NodeRule*) malloc(sizeof(NodeRule));
	e->one = one;
	e->two = two;
    e->op = op;
    e->pos = pos;
	return e;
}

NodeStr* new_node_str(NodeArgs* one, char nome[], char op, int arity) {
	NodeStr *e = (NodeStr*) malloc(sizeof(NodeStr));
    e->one = one;
    e->op = op;
    strcpy(e->nome, nome);
    e->arity = arity;
	return e;
}

NodeStrs* new_node_strs(NodeStr* one, NodeStrs* two, char op) {
	NodeStrs *e = (NodeStrs*) malloc(sizeof(NodeStrs));
	e->one = one;
	e->two = two;
    e->op = op;
	return e;
}

NodeArgs* new_node_args(NodeTerm* one, NodeArgs* two, char op) {
	NodeArgs *e = (NodeArgs*) malloc(sizeof(NodeArgs));
	e->one = one;
	e->two = two;
    e->op = op;
	return e;
}

NodeTerm* new_node_term(NodeStr* one, NodeList* two, char nome[], int type, int var_type) {
	NodeTerm *e = (NodeTerm*) malloc(sizeof(NodeTerm));
	e->one = one;
    e->two = two;
    strcpy(e->nome, nome);
    e->type = type;
    e->var_type = var_type;
	return e;
}

NodeList* new_node_list(NodeTerm* one, NodeTerm* two) {
	NodeList *e = (NodeList*) malloc(sizeof(NodeList));
	e->one = one;
    e->two = two;
	return e;
}

void print_tab(int tabs){
    int i;
    tabs = tabs*2;
    for (i= 0 ; i <tabs; i++){
        printf(" ");
    }
}

void print_str(NodeStr *root, int tabs) {
    if (root == NULL) {
        return;
    }

    print_tab(tabs);
    if (root->arity == 0) {
        printf("<estrutura> %s (type: CON)\n", root->nome);
    } else {
        printf("<estrutura> %s (aridade: %d)\n", root->nome, root->arity);
    }

    if (root->op != '\0') {
        print_tab(tabs+1);
        printf("<(>\n");
    }

    print_args(root->one, tabs+1);

    if (root->op != '\0'){
        print_tab(tabs+1);
        printf("<)>\n");
    }
}

void print_strs(NodeStrs *root, int tabs) {
    if (root == NULL) {
        return;
    }

    print_tab(tabs);
    printf("<estruturas>\n");

    print_str(root->one, tabs+1);
    if (root->op != '\0'){
        print_tab(tabs);
        printf("<%c>\n", root->op);
    }
    print_strs(root->two, tabs+1);
}

void print_args(NodeArgs *root, int tabs) {
    if (root == NULL) {
        return;
    }

    print_tab(tabs);
    printf("<argumentos>\n");

    print_term(root->one, tabs+1);
    if (root->op != '\0'){
        print_tab(tabs);
        printf("<%c>\n", root->op);
    }
    print_args(root->two, tabs+1);
}

void print_term(NodeTerm *root, int tabs) {
    char type[4];
    char var_type[20];
    if (root == NULL) {
        return;
    }

    print_tab(tabs);

    if (root->nome[0] != '\0') {
        datatype_token_to_str(type, root->type);
        if (root->type == REF_SYMBOL){
            vartype_token_to_str(var_type, root->var_type);
            printf("<termo> %s (tipo: %s; tipo_var: %s)\n", root->nome, type, var_type);
        } else {
            printf("<termo> %s (tipo: %s)\n", root->nome, type);
        }
    } else if(root->one != NULL) {
        printf("<termo>\n");
        print_str(root->one, tabs+1);
    } else {
        printf("<termo>\n");
        print_list(root->two, tabs+1);
    }
}

void print_list(NodeList *root, int tabs) {
    if (root == NULL) {
        return;
    }

    print_tab(tabs);
    printf("<lista>");

    printf("<[>\n");
    print_term(root->one, tabs+1);
    if (root->two != NULL) printf("<|>\n");
    print_term(root->two, tabs+1);
    printf("<]>\n");
}

void print_fact(NodeFact *root, int tabs) {

    if (root == NULL) {
        return;
    }

    print_tab(tabs);
    printf("<fato> (pos: %d)\n", root->pos);

    print_str(root->one, tabs+1);

    printf("<%c>", root->op);
}

void print_rule(NodeRule *root, int tabs) {
    if (root == NULL) {
        return;
    }

    print_tab(tabs);
    printf("<regra> (pos: %d) \n", root->pos);
    print_str(root->one, tabs+1);
    print_tab(tabs);
    printf("<:->\n");
    print_strs(root->two, tabs+1);
    print_tab(tabs);
    printf("<.>\n");
}

void print_header(char tipo[]){
    printf("\n\n=====================Arvore %s, Linha %d=====================\n", tipo, yylineno);
}

void print_footer(){
    printf("\n=========================================================\n");
}


void free_fact(NodeFact *root) {
	if (root->one) free_str(root->one);
	free(root);
}

void free_rule(NodeRule *root) {
	if (root->one) free_str(root->one);
	if (root->two) free_strs(root->two);
	free(root);
}

void free_str(NodeStr *root) {
	if (root->one) free_args(root->one);

	free(root);
}

void free_strs(NodeStrs *root) {
	if (root->one) free_str(root->one);
	if (root->two) free_strs(root->two);
	free(root);
}

void free_args(NodeArgs *root) {
	if (root->one) free_term(root->one);
	if (root->two) free_args(root->two);

	free(root);
}

void free_term(NodeTerm *root) {
	if (root->one) free_str(root->one);
	if (root->two) free_list(root->two);

	free(root);
}

void free_list(NodeList *root) {
	if (root->one) free_term(root->one);
	if (root->two) free_term(root->two);
	free(root);
}
