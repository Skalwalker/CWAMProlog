#ifndef INC_REG_HEADER
#define INC_REG_HEADER

#include "memory.h"
#include "uthash.h"
#include "utlist.h"

typedef struct temp_register {
    int num;
    DataType data;
} TempRegister;

typedef struct reg_stream {
    TempRegister reg;
    struct reg_stream *prev;
    struct reg_stream *next;
} RegStream;

typedef struct reg_table {
    char name[100];
    int reg_num;
    UT_hash_handle hh;
} RegTable;

int table_add_reg(char *name);
void table_reg_delete();
void print_registers();


// void create_register();


// void load_register();
// void store_register();

#endif
