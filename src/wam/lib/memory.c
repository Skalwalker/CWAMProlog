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

Node* create_node(DataType *x, int heap_top){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->index = heap_top;
    newNode->data = x;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (DEBUG == 1) {
        printf("Created Node with Element Type");
    }

    return newNode;
}


Tag* create_tag(char *name, int arity){
    Tag *new_tag = (Tag*)malloc(sizeof(Tag));
    strcpy(new_tag->name, name);
    new_tag->arity = arity;
    return new_tag;
}

DataType* create_data(int type, int heap_ref, Tag* tag){
    DataType *new;
    new = (DataType*)malloc(sizeof(DataType));
    new->data_type = type;
    new->tag = tag;
    new->heap_ref = heap_ref;
    return new;
}

int heap_insert_tail(Node *node){

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

int heap_insert_head(Node *node){

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

Node* find_in_heap(int index){
    Node* current = heap->head;

    while (current != NULL){
        if (current->index == index) {
            return current;
        }
        current = current->next;
    }
    printf("O programa falhou em unificar!\n");
    exit(0); 
}


int print_heap(){
    Node* current = heap->head;
    char temp[4];

    if (current == NULL) {
        printf("Empty Heap\n");
        return HEAP_EMPTY;
    }

    printf(".__________________.\n");
    printf("| key | value      |\n");
    while (current != NULL){
        if (current->data->data_type == TAG_SYMBOL) {
             printf("| %d | %s | \n", current->index, current->data->tag->name);
        } else {
            datatype_token_to_str(temp, current->data->data_type);
            printf("| %d | <%s, %d> | \n", current->index, temp, current->data->heap_ref);
        }

        current = current->next;
    }
    printf(".__________________.\n");

    return HEAP_PRINT_SUCCESS;
}

int print_heap_reversed() {
    Node* current = heap->tail;
    char temp[4];

    if (current == NULL) {
        printf("Empty Heap\n");
        return HEAP_EMPTY;
    }

    printf(".__________________.\n");
    printf("| key | value      |\n");
    while (current != NULL){
        if (current->data->data_type == TAG_SYMBOL) {
             printf("| %3d | %10s | \n", current->index, current->data->tag->name);
        } else {
            datatype_token_to_str(temp, current->data->data_type);
            printf("| %3d | <%s, %3d> | \n", current->index, temp, current->data->heap_ref);
        }
        current = current->prev;
    }
    printf(".__________________.\n");

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

int destroy_heap(){
    if (heap == NULL) {
        return 0;
    }

    while (heap->head != NULL) {
        Node* node = heap_remove_head(heap);
        free(node);
    }

    free(heap);
    printf("Heap Destroyed\n");
    return 1;
}
