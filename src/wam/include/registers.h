#ifndef INC_REG_HEADER
#define INC_REG_HEADER

#include "memory.h"
#include "uthash.h"
#include "utlist.h"
#include "../../parser/arvore.h"

typedef struct reg_map{
    char name[100];
    int reg_num;
    int occ;
    UT_hash_handle hh;
} RegMapping;

typedef struct x_register {
    int reg_num;
    DataType *data;
    UT_hash_handle hh;
} XRegister;

int use_register();

XRegister *find_register(int id);
XRegister *create_register(int num, DataType *data);
void free_xregisters();

int map_reg(char *name);
RegMapping *find_reg_map(char *name);
void free_reg_map();

void print_registers();

#endif
