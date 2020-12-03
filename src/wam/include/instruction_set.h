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
void put_structure();
void put_variable();
void put_value();

/* Set Instructions */
void set_variable();
void set_value();

/* Get Instructions */
void get_structure();
void get_variable();
void get_value();

/* Unify Instructions */
void unify_variable();
void unify_value();

void unify(int addr1, int addr2);
int deref(int address);
void bind(int addr1, int addr2);

#endif
