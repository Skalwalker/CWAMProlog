#ifndef INC_INSTSET_HEADER
#define INC_INSTSET_HEADER

#include "memory.h"
#include "registers.h"
#include "wam.h"
#include "utstack.h"

enum MODES {
    WRITE = 0,
    READ = 1
};

extern int heap_register;
extern int subterm_register;
extern Heap *heap;
extern PDLNode *pdl;

/* Put Instructions */
void put_structure(StrData *str, int reg_num);
void put_variable();
void put_value();

/* Set Instructions */
void set_variable(int reg_num);
void set_value(int reg_num);

/* Get Instructions */
void get_structure(StrData *tag, int reg_num);
void get_variable();
void get_value();

/* Unify Instructions */
void unify_variable(int reg_num);
void unify_value(int reg_num);

int deref(int address);
void unify();

void unify(int addr1, int addr2);
int deref(int address);
void bind(int addr1, int addr2);

#endif
