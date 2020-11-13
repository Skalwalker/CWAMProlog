#ifndef INC_TREE_HEADER
#define INC_TREE_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../wam/include/shared.h"

extern int yylineno;

typedef struct tree_node TreeNode;
typedef struct str_data StrData;
typedef struct term_data TermData;

enum NODETYPES {
    NODE_RULE = 31,
    NODE_FACT = 32,
    NODE_TERM = 33,
    NODE_STR = 34,
    NODE_ARGS = 35,
    NODE_STRS = 36,
    NODE_LIS = 37
};


struct str_data {
    char nome[100];
    int arity;
};

struct term_data {
    char nome[100];
    int type;
    int var_type;
};

struct tree_node {
    TreeNode *left;
    TreeNode *right;
    int node_type;
    union {
        int pos;
        TermData* term_data;
        StrData* str_data;
    };
};

StrData *new_str(char *nome, int arity);
TermData *new_term(char *nome, int type, int var_type);
TreeNode* new_tree_node(TreeNode* left, TreeNode* right, int node_type, TermData *term, StrData *str);
void print_tree(TreeNode *root, int tabs);

void print_header(char tipo[]);
void print_footer();

void free_tree(TreeNode *root);

#endif
