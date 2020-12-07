#ifndef INC_WAM_HEADER
#define INC_WAM_HEADER

#include "../../parser/arvore.h"
#include "registers.h"
#include "instruction_set.h"
#include "memory.h"

extern int reg_counter;

enum INSTRUCTION_TYPE {
    QUERY_INSTR = 41,
    PROG_INSTR = 42
};

typedef struct queue {
    TreeNode *data;
    int visited;
    int inited;
    struct queue *next;
} Queue;

void execute_wam(TreeNode *root, int query_prog);
void register_names(TreeNode* root);

void query_token_stream(TreeNode* root);
void query_token_children(TreeNode* root);

void prog_token_stream(TreeNode* root);

void process_token(TreeNode *node, int init, int inst_type);

void map_instruction(TempRegister *reg, int occ, int init, int inst_type);


#endif
