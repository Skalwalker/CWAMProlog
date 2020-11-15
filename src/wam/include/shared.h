#ifndef INC_SHARED_HEADER
#define INC_SHARED_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0

enum DATATYPES {
    REF_SYMBOL = 1,
    STR_SYMBOL = 2,
    CON_SYMBOL = 3,
    LIS_SYMBOL = 4,
    TAG_SYMBOL = 5,
};

enum VARTYPES {
    BASIC_VAR = 6,
    SINGLETON_VAR = 7,
    ANONYM_VAR = 8,
};


enum CODES {
    HEAP_CREATE_SUCCESS = 110,
    HEAP_ADD_SUCCESS = 111,
    HEAP_PRINT_SUCCESS = 112,
    // HEAP_CREATE_SUCCESS = 113,
    HEAP_NOT_FOUND = 100,
    HEAP_CREATE_ERROR = 101,
    HEAP_ADD_ERROR = 102,
    HEAP_REMOVER_ERROR = 103,
    HEAP_DESTRUCTION_ERROR = 104,
    HEAP_EMPTY = 105
};

void datatype_token_to_str(char res[], int type);
void vartype_token_to_str(char res[], int type);



#endif
