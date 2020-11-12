#include "../include/wam.h"

Heap *heap;

void create_wam(){
    TempRegister x[100];
    heap = create_heap();
}



// def CONSTROI_LEXICO(root, contador_local, contador_pai):
//     x_value = ADD_REGISTRADOR(contador_local, root.nome);
//     hash_functions["X" + str(x_value)] = []
//     arg = root.args
//     ari_parent = root.ari
//     for i in range(root.ari):
//         contador_local += 1
//         if arg.terminal is not None:
//             reg_numb = ADD_REGISTRADOR(contador_pai+1, arg.terminal)
//             hash_functions["X" + str(x_value)].append("X" + str(reg_numb))
//         else:
//             CONSTROI_LEXICO(arg.est, contador_local, ari_parent+1)
//             hash_functions["X" + str(x_value)].append("X" + str(hash_nomes[arg.est.nome]))

//         arg = arg.args

void flatten_fact(NodeFact* fact){
    NodeStr *root = fact->one;
    table_add_reg(root->nome);
    printf("%s \n", root->nome);
    flatten_args(root);
    print_registers();
}

void flatten_args(NodeStr *root) {
    NodeArgs *arg_i;
    NodeTerm *term_i;

    table_add_reg(root->nome);

    for (arg_i = root->one; arg_i != NULL; arg_i = arg_i->two) {
        term_i = arg_i->one;

        if (term_i->one == NULL) {
            table_add_reg(term_i->nome);
            printf("%s \n", term_i->nome);
        } else {
            // flatten_args(term_i->one);
            // printf("%s \n", term_i->one->nome);
        }
    }
}


void tokenize() {

}

void build_heap(){

}
// p(Z, h(Z, W), f(W)).
//
// Flattened
//
// X3 = h(X2, X5)
// X4 = f(X5)
// X1 = p(X2, X3, X4)
//
// Tokenized
//
// [X3 (h/3), X2, X5, X4(f/1), X5, X1(p/3), X2, X3, X4]
