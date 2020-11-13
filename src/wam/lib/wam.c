#include "../include/wam.h"

// Heap *heap;

// void create_wam(){
//     TempRegister x[100];
//     heap = create_heap();
// }

// void flatten_fact(NodeFact* fact){
//     NodeStr *root = fact->one;
//     flatten_args(root);
//     printf("\n");
//     print_registers();
//     dfs(root);
// }

// void dfs(NodeStr *root) {
//     NodeArgs *arg_i;
//     NodeTerm *term_i;

//     for (arg_i = root->one; arg_i != NULL; arg_i = arg_i->two) {
//         term_i = arg_i->one;

//         if (term_i->one != NULL) {
//             printf("X%d ", table_add_reg(term_i->one->nome));
//             if (term_i->type == STR_SYMBOL){
//                 dfs(term_i->one);
//             }

//         } else {
//             printf("X%d ", table_add_reg(term_i->nome));
//         }
//     }
// }


// void flatten_args(NodeStr *root) {
//     NodeArgs *arg_i;
//     NodeTerm *term_i;
//     Queue *head = NULL;
//     Queue *elt, *tmp, *str;

//     printf("X%d ", table_add_reg(root->nome));

//     for (arg_i = root->one; arg_i != NULL; arg_i = arg_i->two) {
//         term_i = arg_i->one;

//         if (term_i->one == NULL) {
//             printf("X%d ", table_add_reg(term_i->nome));
//         } else {
//             printf("X%d ", table_add_reg(term_i->one->nome));
//             str = (Queue*)malloc(sizeof(Queue));
//             str->data = term_i->one;
//             LL_APPEND(head, str);
//         }
//     }

//     LL_FOREACH_SAFE(head,elt,tmp) {
//       LL_DELETE(head, elt);
//       flatten_args(elt->data);
//       free(elt);
//     }
// }


// void tokenize() {

// }

// void build_heap(){

// }


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
