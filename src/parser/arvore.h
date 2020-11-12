#ifndef INC_TREE_HEADER
#define INC_TREE_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../wam/include/shared.h"

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


void print_header(char tipo[]);
void print_footer();

void free_fact(NodeFact *root);
void free_rule(NodeRule *root);
void free_str(NodeStr *root);
void free_strs(NodeStrs *root);
void free_args(NodeArgs *root);
void free_term(NodeTerm *root);
void free_list(NodeList *root);

#endif
