#ifndef INC_WAM_HEADER
#define INC_WAM_HEADER

#include "../../parser/arvore.h"
#include "registers.h"
#include "instruction_set.h"
#include "memory.h"

// extern int reg_counter;

void create_wam();
void flatten_fact(NodeFact* fact);
void flatten_args(NodeStr *root);

#endif
