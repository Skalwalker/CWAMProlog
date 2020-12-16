#include "../include/registers.h"

XRegister *registers = NULL;
RegMapping *reg_map = NULL;

int reg_counter = 1;
int heap_register = 0;
int subterm_register = 0;


XRegister *create_register(int num, DataType *data){
    XRegister *reg;

    HASH_FIND_INT(registers, &num, reg);

    if (reg == NULL) {
      reg = (XRegister*)malloc(sizeof(XRegister));
      reg->reg_num = num;
      reg->data = data;
      HASH_ADD_INT(registers, reg_num, reg);
    } else {
      reg->data = data;
    }

    return reg;
}

XRegister *find_register(int id) {
    XRegister *reg;
    HASH_FIND_INT(registers, &id, reg);
    return reg;
}

void free_xregisters() {
  XRegister *current_table, *tmp;

  HASH_ITER(hh, registers, current_table, tmp) {
    HASH_DEL(registers,current_table);
    free(current_table);
  }
}


int map_reg(char *name) {

    RegMapping *new;

    HASH_FIND_STR(reg_map, name, new);

    if(new==NULL){
        new = (RegMapping*)malloc(sizeof(RegMapping));
        strcpy(new->name, name);
        new->reg_num = reg_counter;
        new->occ = 0;
        reg_counter += 1;
        HASH_ADD_STR(reg_map, name, new);
        return new->reg_num;
    }

    return -1;
}

RegMapping *find_reg_map(char *name) {

    RegMapping *reg;

    HASH_FIND_STR(reg_map, name, reg);

    if(reg==NULL){
        return NULL;
    }

    return reg;
}

void free_reg_map() {
  RegMapping *current_table, *tmp;

  HASH_ITER(hh, reg_map, current_table, tmp) {
    HASH_DEL(reg_map,current_table);
    free(current_table);
  }
}


void print_xregisters() {
    XRegister *r;
    printf("\n==========REG TABLE==========\n");
    for(r=registers; r != NULL; r=r->hh.next) {
        // printf("KEY %s: Reg_Number %d\n", r->name,  r->reg_num);
        // printf("  Register %d Data:\n", r->reg->num);
        // printf("      Data Type %d\n", r->reg->data->data_type);
        // printf("      Heap Ref %d\n", r->reg->data->heap_ref);
    }
    printf("=============================\n");
}


