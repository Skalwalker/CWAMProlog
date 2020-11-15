#include "../include/memory.h"

Heap* create_heap(){
    Heap* newHeap = (Heap*)malloc(sizeof(Heap));
    newHeap->head = NULL;
    newHeap->tail = NULL;

    if (DEBUG == 1) {
        printf("Created Heap\n");
    }

    return newHeap;
}

Node* create_node(DataType x){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = x;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (DEBUG == 1) {
        printf("Created Node with Element Type: %d\n", x.data_type);
    }

    return newNode;
}

DataType* create_data(int type, int heap_ref, Tag* tag){
    DataType *new;
    new = (DataType*)malloc(sizeof(DataType));

    new->data_type = type;

    if (type == TAG_SYMBOL){
        new->tag = tag;
    } else {
        new->heap_ref = heap_ref;
    }

    return new;
}

int heap_insert_tail(Heap *heap, Node *node){

    if (heap == NULL) {
        return HEAP_NOT_FOUND;
    }

    if (heap->head == NULL) {
        heap->head = node;
        heap->tail = node;
        return HEAP_CREATE_SUCCESS;
    }

    Node* temp = heap->head;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    temp->next = node;
    node->prev = temp;
    heap->tail = node;

    if (DEBUG == 1) {
        printf("Element Added\n");
    }

    return HEAP_CREATE_SUCCESS;
}

int heap_insert_head(Heap *heap, Node *node){

    if (heap == NULL) {
        return HEAP_NOT_FOUND;
    }


    if (heap->head == NULL) {
        heap->head = node;
        heap->tail = node;
        return HEAP_CREATE_SUCCESS;
    }

    Node* temp = heap->head;
    temp->prev = node;
    node->next = temp;
    heap->head = node;

    if (DEBUG == 1) {
        printf("Element Added\n");
    }

    return HEAP_CREATE_SUCCESS;
}

int print_heap(Heap* heap){
    Node* current = heap->head;

    if (current == NULL) {
        printf("Empty Heap\n");
        return HEAP_EMPTY;
    }

    while (current != NULL){
        printf("Element = %d\n", current->index);
        printf("\tNext = %d\n", current->next->index);
        printf("\tPrev = %d\n", current->prev->index);
        current = current->next;
    }

    return HEAP_PRINT_SUCCESS;
}

int print_heap_reversed(Heap* heap) {
    Node* current = heap->tail;

    if (current == NULL) {
        printf("Empty Heap\n");
        return HEAP_EMPTY;
    }

    while (current != NULL){
        printf("Element = %d\n", current->index);
        printf("\tNext = %d\n", current->next->index);
        printf("\tPrev = %d\n", current->prev->index);
        current = current->prev;
    }

    return HEAP_PRINT_SUCCESS;
}

Node* heap_remove_head(Heap* heap){

    if (heap == NULL) {
        return NULL;
    }

    if (heap->head == NULL) {
        return NULL;
    }

    Node* removing = heap->head;
    heap->head = removing->next;
    if (removing->next != NULL){
        removing->next->prev = NULL;
    } else {
        heap->tail = NULL;
    }

    removing->next = NULL;
    removing->prev = NULL;

    return removing;
}

Node* heap_remove_tail(Heap* heap){

    if (heap == NULL) {
        return NULL;
    }

    if (heap->head == NULL) {
        return NULL;
    }

    Node* removing = heap->tail;
    heap->tail = removing->prev;
    if (removing->prev != NULL){
        removing->prev->next = NULL;
    } else {
        heap->head = NULL;
    }

    removing->next = NULL;
    removing->prev = NULL;

    return removing;
}

void destroy_node(Node* node) {
    free(node);
}

int destroy_heap(Heap* heap){

    while (heap->head != NULL) {
        Node* node = heap_remove_head(heap);
        free(node);
    }

    free(heap);
    printf("Heap Destroyed\n");
    return 1;
}
