#ifndef INC_SHARED_HEADER
#define INC_SHARED_HEADER

#include <stdio.h>
#include <stdlib.h>

#define DEBUG 1

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



#endif
