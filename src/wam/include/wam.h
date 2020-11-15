#ifndef INC_WAM_HEADER
#define INC_WAM_HEADER

#include "../../parser/arvore.h"
#include "registers.h"
#include "instruction_set.h"
#include "memory.h"

typedef struct queue {
    TreeNode *data;
    struct queue *next;
} Queue;

void execute_wam(TreeNode *root);
void register_names(TreeNode* root);
void token_stream(TreeNode* root);
void token_children(TreeNode* root);
void map_instruction(TempRegister *reg, int occ);
// void flatten_fact(NodeFact* fact);
// void flatten_args(NodeStr *root);
// void print_tokens(NodeStr *root);

#endif
