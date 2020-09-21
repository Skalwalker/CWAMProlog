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

Node* create_node(int x){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = x;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (DEBUG == 1) {
        printf("Created Node with Element %d\n", x);
    }

    return newNode;
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
        if (heap->head != NULL){
            printf("\tHead = %d\n", heap->head->data);
        } else {
            printf("\tHEAD = NULL\n");
        }

        if (heap->tail != NULL){
            printf("\tTail = %d\n", heap->tail->data);
        } else {
            printf("\tTail = NULL\n");
        }
        // printf("Heap Head %d", heap->head);
        // printf("Heap Tail %d", heap->tail);
        return HEAP_EMPTY;
    }

    while (current != NULL){
        printf("Element = %d\n", current->data);
        if (current->next != NULL){
            printf("\tNext = %d\n", current->next->data);
        } else {
            printf("\tNext = NULL\n");
        }

        if (current->prev != NULL)
        {
            printf("\tPrev = %d\n", current->prev->data);
        } else {
            printf("\tPrev = NULL\n");
        }



        current = current->next;
    }

    // printf("NULL\n");

    return HEAP_PRINT_SUCCESS;
}

int print_heap_reversed(Heap* heap) {
    Node* current = heap->tail;

    if (current == NULL) {
        printf("Empty Heap\n");
        return HEAP_EMPTY;
    }

    while (current != NULL){
        printf("Element = %d\n", current->data);
        printf("\tNext = %d\n", current->next->data);
        printf("\tPrev = %d\n", current->prev->data);
        current = current->prev;
    }

    // printf("NULL\n");

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
