#include "shared.h"

typedef struct heap {
    struct node *head;
    struct node *tail;
} Heap;

typedef struct data_type {
    int data;
} DataType;

typedef struct node {
    struct data_type data;
    struct node *next;
    struct node *prev;
} Node;


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
