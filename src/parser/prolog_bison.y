%debug
%defines
%define parse.error verbose
%define api.pure

%{
	#include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../wam/include/main.h"

    extern char file_name[];
    extern int yylineno;

    typedef struct node_rule NodeRule;
    typedef struct node_str NodeStr;
    typedef struct node_strs NodeStrs;
    typedef struct node_args NodeArgs;
    typedef struct node_term NodeTerm;
    typedef struct node_fact NodeFact;
    typedef struct node_list NodeList;

    struct node_fact {
        char op;
        NodeStr *one;
    };

    struct node_rule {
        char op;
        NodeStr *one;
        NodeStrs *two;
    };

    struct node_str {
        char op;
        NodeArgs *one;
        char nome[100];
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
    };

    struct node_list {
        NodeTerm *one;
        NodeTerm *two;
    };


    NodeFact* new_node_fact(NodeStr* one, char op);
    NodeRule* new_node_rule(NodeStr* one, NodeStrs* two, char op);
    NodeStr* new_node_str(NodeArgs* one, char nome[], char op);
    NodeStrs* new_node_strs(NodeStr* one, NodeStrs* two, char op);
    NodeArgs* new_node_args(NodeTerm* one, NodeArgs* two, char op);
    NodeTerm* new_node_term(NodeStr* one, NodeList* two, char nome[]);
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
	void yyerror (char const *);
    int yylex();
%}

%locations

%union {
    char con[100];
    char var[100];
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
%token RULE_SYM

%type <list> list
%type <fato> fato
%type <regra> regra
%type <str> estrutura
%type <strs> estruturas
%type <term> termo
%type <args> argumentos

%start programa

%destructor { free_list($$); } <list>
%destructor { free_fact($$); } <fato>
%destructor { free_rule($$); } <regra>
%destructor { free_str($$); } <str>
%destructor { free_strs($$); } <strs>
%destructor { free_term($$); } <term>
%destructor { free_args($$); } <args>


%%

programa:
    predicado
;

predicado:
    clausula
    | predicado clausula
;

clausula:
    fato {print_header("Fato"); print_fact($1, 0); print_footer();free_fact($1);}
    | regra {print_header("Regra"); print_rule($1, 0); print_footer();free_rule($1);}
    | error '.' {yyerrok; }
;

fato:
    estrutura '.' {$$ = new_node_fact($1, '.');}
;

regra:
    estrutura RULE_SYM estruturas '.' {$$ = new_node_rule($1, $3, '.');}
;

estruturas:
    estrutura {$$ = new_node_strs($1, NULL, '\0');}
    | estrutura ',' estruturas {$$ = new_node_strs($1, $3, ',');}
;

estrutura:
    CON {$$ = new_node_str(NULL, $1, '\0');  st_add_symbol(CON_SYMBOL, $1);}
    | CON '(' argumentos ')' {$$ = new_node_str($3, $1, ')'); st_add_symbol(STR_SYMBOL, $1);}
;

argumentos:
    termo {$$ = new_node_args($1, NULL, '\0');}
    | termo ',' argumentos {$$ = new_node_args($1, $3, ',');}
;

termo:
    estrutura {$$ = new_node_term($1, NULL, "\0");}
    | VAR {$$ = new_node_term(NULL, NULL, $1);}
    | list {$$ = new_node_term(NULL, $1, "\0");}
;

list:
    '[' ']' {$$ = new_node_list(NULL, NULL);}
    | '[' termo ']' {$$ = new_node_list($2, NULL);}
    | '[' termo '|' termo ']' {$$ = new_node_list($2, $4);}
;

%%

NodeFact* new_node_fact(NodeStr* one, char op) {
	NodeFact *e = (NodeFact*) malloc(sizeof(NodeFact));
	e->one = one;
    e->op = op;
	return e;
}

NodeRule* new_node_rule(NodeStr* one, NodeStrs* two, char op) {
	NodeRule *e = (NodeRule*) malloc(sizeof(NodeRule));
	e->one = one;
	e->two = two;
    e->op = op;
	return e;
}

NodeStr* new_node_str(NodeArgs* one, char nome[], char op) {
	NodeStr *e = (NodeStr*) malloc(sizeof(NodeStr));
    e->one = one;
    e->op = op;
    strcpy(e->nome, nome);

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

NodeTerm* new_node_term(NodeStr* one, NodeList* two, char nome[]) {
	NodeTerm *e = (NodeTerm*) malloc(sizeof(NodeTerm));
	e->one = one;
    e->two = two;
    strcpy(e->nome, nome);
	return e;
}

NodeList* new_node_list(NodeTerm* one, NodeTerm* two) {
	NodeList *e = (NodeList*) malloc(sizeof(NodeList));
	e->one = one;
    e->two = two;
	return e;
}

void print_tab(int tabs){
    tabs = tabs*2;
    char *t = malloc(sizeof(char) * tabs+1);
    memset(t, ' ', tabs);
    t[tabs] = '\0';
    printf("%s", t);
    free(t);
}

void print_str(NodeStr *root, int tabs) {
    if (root == NULL) {
        return;
    }

    print_tab(tabs);

    printf("<estrutura> %s\n", root->nome);
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
    if (root == NULL) {
        return;
    }

    print_tab(tabs);

    if (root->nome[0] != '\0') {
        printf("<termo> %s\n", root->nome);
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
    printf("<fato>\n");

    print_str(root->one, tabs+1);

    printf("<%c>", root->op);
}

void print_rule(NodeRule *root, int tabs) {
    if (root == NULL) {
        return;
    }

    print_tab(tabs);
    printf("<regra>\n");
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
