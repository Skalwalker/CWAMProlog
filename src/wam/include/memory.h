#ifndef INC_MEMORY_HEADER
#define INC_MEMORY_HEADER

#include "shared.h"

int heap_counter;

typedef struct tag {
    char name[100];
    char composed_name[120];
    int arity;
} Tag;

typedef struct data_type {
    int data_type;
    union {
        Tag tag;
        int heap_ref;
    };
} DataType;

DataType create_data(int type, int heap_ref, Tag tag);

typedef struct node {
    int index;
    struct data_type data;
    struct node *next;
    struct node *prev;
} Node;

typedef struct heap {
    struct node *head;
    struct node *tail;
} Heap;

Heap* create_heap();
Node* create_node(DataType x);
int heap_insert_tail(Heap *heap, Node *node);
int heap_insert_head(Heap *heap, Node *node);
int print_heap(Heap* heap);
int print_heap_reversed(Heap* heap);
Node* heap_remove_head(Heap* heap);
Node* heap_remove_tail(Heap* heap);
void destroy_node(Node* node);
int destroy_heap(Heap* heap);

#endif
