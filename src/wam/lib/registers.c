#include "../include/registers.h"

RegTable *r_table = NULL;
RegStream *stream = NULL;

int reg_counter = 1;

void add_stream(TempRegister *reg){
    RegStream *new;
    new = (RegStream*)malloc(sizeof(RegStream));
    new->reg = reg;
    LL_APPEND(stream, new);
}

RegTable *table_reg_lookup(char *nome) {
    RegTable *reg;
    HASH_FIND_STR(r_table, nome, reg);
    return reg;
}

int table_add_reg(char *name, DataType *data) {

    RegTable *new;
    TempRegister *reg;

    HASH_FIND_STR(r_table, name, new);

    if(new==NULL){
        new = (RegTable*)malloc(sizeof(RegTable));
        strcpy(new->name, name);
        new->reg_num = reg_counter;
        reg = create_register(reg_counter, data);
        new->reg = reg;
        new->on_stream = 0;
        HASH_ADD_STR(r_table, name, new);
        reg_counter += 1;
        return new->reg_num;
    } else {
        return -1;
    }
}

void table_reg_delete() {
  RegTable *current_table, *tmp;

  HASH_ITER(hh, r_table, current_table, tmp) {
    HASH_DEL(r_table,current_table);
    free(current_table);
  }
}

void print_registers() {
    RegTable *r;
    printf("\n==========REG TABLE==========\n");
    for(r=r_table; r != NULL; r=r->hh.next) {
        printf("KEY %s: Reg_Number %d\n", r->name,  r->reg_num);
        printf("  Register %d Data:\n", r->reg->num);
        printf("      Data Type %d\n", r->reg->data->data_type);
        printf("      Heap Ref %d\n", r->reg->data->heap_ref);
    }
    printf("=============================\n");
}

void print_stream() {
    RegStream *reg;
    printf("\n");
    LL_FOREACH(stream,reg) {
        printf("X%d ", reg->reg->num);
    }
    printf("\n");
}

TempRegister* create_register(int num, DataType *data){
    TempRegister *reg = (TempRegister*)malloc(sizeof(TempRegister));
    reg->num = num;
    reg->data = data;
    return reg;
}


