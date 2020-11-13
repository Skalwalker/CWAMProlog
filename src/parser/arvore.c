#include "arvore.h"

TreeNode* new_tree_node(TreeNode* left, TreeNode* right, int node_type, TermData *term, StrData *str) {
    TreeNode* new = (TreeNode*)malloc(sizeof(TreeNode));
    new->left = left;
    new->right = right;
    new->node_type = node_type;

    switch (node_type) {
        case NODE_RULE:
            new->pos = yylineno;
            break;
        case NODE_FACT:
            new->pos = yylineno;
            break;
        case NODE_TERM:
            new->term_data = term;
            break;
        case NODE_STR:
            new->str_data = str;
            break;
        default:
            break;
    }

    return new;
}

StrData *new_str(char *nome, int arity) {
    StrData *new_str;
    new_str = (StrData*)malloc(sizeof(StrData));

    strcpy(new_str->nome, nome);
    new_str->arity = arity;

    return new_str;
}

TermData *new_term(char *nome, int type, int var_type) {
    TermData *new_term;
    new_term = (TermData*)malloc(sizeof(TermData));

    strcpy(new_term->nome, nome);
    new_term->type = type;
    new_term->var_type = var_type;

    return new_term;
}

void print_header(char tipo[]){
    printf("\n\n=====================Arvore %s, Linha %d=====================\n", tipo, yylineno);
}

void print_footer(){
    printf("\n=========================================================\n");
}

void print_tab(int tabs){
    int i;
    tabs = tabs*2;
    for (i= 0 ; i <tabs; i++){
        printf(" ");
    }
}

void print_tree(TreeNode *root, int tabs) {
    StrData *str_nd = NULL;
    TermData *term_nd = NULL;
    char type[10];
    char var_type[20];

    if (root == NULL) {
        return;
    }

    print_tab(tabs);

    switch (root->node_type) {
        case NODE_RULE:
            printf("<regra> (pos: %d)\n", root->pos);
            break;
        case NODE_FACT:
            printf("<fato> (pos: %d)\n", root->pos);
            break;
        case NODE_TERM:
            term_nd = root->term_data;
            if (term_nd != NULL) {
                datatype_token_to_str(type, term_nd->type);
                if (term_nd->type == REF_SYMBOL) {
                    vartype_token_to_str(var_type, term_nd->var_type);
                    printf("<termo> %s (tipo: %s; tipo_var: %s)\n", term_nd->nome, type, var_type);
                } else {
                    printf("<termo> %s (tipo: %s)\n", term_nd->nome, type);
                }
            } else {
                printf("<termo>\n");
            }
            break;
        case NODE_STR:
            str_nd = root->str_data;
            if (str_nd->arity == 0){
                printf("<estrutura> %s (type: CON)\n", str_nd->nome);
            } else {
                printf("<estrutura> %s (aridade: %d)\n", str_nd->nome, str_nd->arity);
            }
            break;
        case NODE_STRS:
            printf("<estruturas>\n");
            break;
        case NODE_ARGS:
            printf("<argumentos>\n");
            break;
        case NODE_LIS:
            printf("<lista>\n");
            break;
        default:
            break;
    }

    print_tree(root->left, tabs+1);
    print_tree(root->right, tabs+1);
}

void free_tree(TreeNode *root) {
    if (root->left) free_tree(root->left);
    if (root->right) free_tree(root->right);

    if (root->node_type == NODE_STR){
        free(root->str_data);
    }

    if (root->node_type == NODE_TERM){
        free(root->term_data);
    }

    free(root);
}
