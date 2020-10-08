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

    void print_str(NodeStr *root);
    void print_strs(NodeStrs *root);
    void print_args(NodeArgs *root);
    void print_term(NodeTerm *root);
    void print_fact(NodeFact *root);
    void print_rule(NodeRule *root);
    void print_list(NodeList *root);
    void eat_to_newline(void);

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

%%

programa:
    predicado
;

predicado:
    clausula
    | predicado clausula
;

clausula:
    fato {print_header("Fato"); print_fact($1); print_footer();}
    | regra {print_header("Regra"); print_rule($1); print_footer();}
    | error '.' {yyerrok;}
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
    '[' ']' {$$ = new_node_list(NULL, NULL); st_add_symbol(LIS_SYMBOL, "[]");}
    | '[' termo ']' {$$ = new_node_list($2, NULL);  st_add_symbol(LIS_SYMBOL, "[TERMS]");}
    | '[' termo '|' termo ']' {$$ = new_node_list($2, $4);st_add_symbol(LIS_SYMBOL, "[TERMS|TERMS]");}
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


void print_str(NodeStr *root) {
    if (root == NULL) {
        return;
    }

    printf("%s\n", root->nome);
    if (root->op != '\0') {
        printf("(\n");
    }

    print_args(root->one);

    if (root->op != '\0'){
        printf(")\n");
    }
}

void print_strs(NodeStrs *root) {
    if (root == NULL) {
        return;
    }

    print_str(root->one);
    if (root->op != '\0') printf("%c\n", root->op);
    print_strs(root->two);
}

void print_args(NodeArgs *root) {
    if (root == NULL) {
        return;
    }

    print_term(root->one);
    if (root->op != '\0') printf("%c\n", root->op);
    print_args(root->two);
}

void print_term(NodeTerm *root) {
    if (root == NULL) {
        return;
    }

    if (root->nome[0] != '\0') {
        printf("%s\n", root->nome);
    } else if(root->one != NULL) {
        print_str(root->one);
    } else {
        print_list(root->two);
    }
}

void print_list(NodeList *root) {
    if (root == NULL) {
        return;
    }

    printf("[\n");
    print_term(root->one);
    if (root->two != NULL) printf("|\n");
    print_term(root->two);
    printf("]\n");
}

void print_fact(NodeFact *root) {

    if (root == NULL) {
        return;
    }

    print_str(root->one);

    printf("%c", root->op);
}

void print_rule(NodeRule *root) {
    if (root == NULL) {
        return;
    }

    print_str(root->one);
    printf(":-\n");
    print_strs(root->two);
    printf(".\n");
}

void print_header(char tipo[]){
    printf("\n\n=====================Arvore %s, Linha %d=====================\n", tipo, yylineno);
}

void print_footer(){
    printf("\n=========================================================\n");
}


// void free_tree(Node *root) {
// 	if (root->one) free_tree(root->one);
// 	if (root->two) free_tree(root->two);
//     if (root->three) free_tree(root->three);
// 	if (root->four) free_tree(root->four);
// 	free(root);
// }
