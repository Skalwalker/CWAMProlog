#ifndef INC_MEMORY_HEADER
#define INC_MEMORY_HEADER

#include "shared.h"


typedef struct tag {
    char name[100];
    int arity;
} Tag;

typedef struct data_type {
    int data_type;
    int heap_ref;
    Tag* tag;
} DataType;


Tag* create_tag(char *name, int arity);
DataType *create_data(int type, int heap_ref, Tag* tag);

typedef struct heap_node {
    int index;
    DataType *data;
    struct heap_node *next;
    struct heap_node *prev;
} Node;

typedef struct heap {
    struct heap_node *head;
    struct heap_node *tail;
} Heap;

extern Heap *heap;

Heap* create_heap();
Node* create_node(DataType *x, int heap_top);
Node *fetch_node(int address);
int heap_insert_tail(Node *node);
int heap_insert_head(Node *node);
int print_heap();
int print_heap_reversed();
Node* heap_remove_head();
Node* heap_remove_tail();
void destroy_node(Node* node);
int destroy_heap();

#endif
