#ifndef INC_REG_HEADER
#define INC_REG_HEADER

#include "memory.h"
typedef struct temp_register {
    struct node *value;
} TempRegister;

void create_register();
void load_register();
void store_register();

#endif
