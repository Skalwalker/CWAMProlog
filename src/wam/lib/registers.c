#include "../include/registers.h"


RegTable *r_table = NULL;
int reg_counter = 1;

int table_add_reg(char *name) {

    RegTable *new;

    HASH_FIND_STR(r_table, name, new);

    if(new==NULL){
        new = (RegTable*)malloc(sizeof(RegTable));
        strcpy(new->name, name);
        new->reg_num = reg_counter;
        HASH_ADD_STR(r_table, name, new);
        reg_counter += 1;
        return -1;
    } else {
        return new->reg_num;
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

    for(r=r_table; r != NULL; r=r->hh.next) {
        printf("user id %d: name %s\n", r->reg_num, r->name);
    }
}
