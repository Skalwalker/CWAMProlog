#include "arvore.h"


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
