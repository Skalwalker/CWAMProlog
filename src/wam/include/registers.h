#ifndef INC_REG_HEADER
#define INC_REG_HEADER

#include "memory.h"
#include "uthash.h"
#include "utlist.h"
#include "../../parser/arvore.h"

typedef struct temp_register {
    int num;
    DataType *data;
} TempRegister;

typedef struct reg_table {
    char name[100];
    int reg_num;
    int on_stream;
    TempRegister *reg;
    UT_hash_handle hh;
} RegTable;

typedef struct reg_stream {
    TempRegister *reg;
    struct reg_stream *prev;
    struct reg_stream *next;
} RegStream;

RegTable *table_reg_lookup(char *nome);
int table_add_reg(char *name, DataType *data);
TempRegister* create_register(int num, DataType *data);
void table_reg_delete();

void print_registers();
void print_stream();
void add_stream(TempRegister *reg);

#endif
