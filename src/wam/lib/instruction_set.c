#include "../include/instruction_set.h"

// typedef struct tag {
//     char name[100]; // f
//     int arity; //2
// } Tag;

// typedef struct temp_register {
//     int num;
//     DataType *data;
// } TempRegister;

// typedef struct data_type {
//     int data_type;
//     int heap_ref;
//     Tag* tag;
// } DataType;

// typedef struct heap_node {
//     int index;
//     DataType *data;
//     struct heap_node *next;
//     struct heap_node *prev;
// } Node;


/* Put Instructions */
void put_structure(TempRegister *reg) {
    Node *token, *functor;
    DataType *data;
    token = (Node*)malloc(sizeof(Node));
    functor = (Node*)malloc(sizeof(Node));

    // heap[h] <- <Str H+1>
    data = reg->data;
    // x_i <- heap[h]
    data->heap_ref = heap_register + 1;
    token = create_node(data, heap_register);
    heap_insert_head(token);

    // heap[h] <- nome/aridade
    data = create_data(TAG_SYMBOL, 0, reg->data->tag);
    functor = create_node(data, heap_register+1);
    heap_insert_head(functor);

    heap_register += 2;
}

void put_variable() {

}

void put_value() {

}

/* Set Instructions */
void set_variable(TempRegister *reg) {
    Node *token;
    token = (Node*)malloc(sizeof(Node));
    // x_i <- heap[h]
    reg->data->heap_ref = heap_register;
    token = create_node(reg->data, heap_register);
    // heap[h] <- <REF H>
    heap_insert_head(token);
    heap_register += 1;
}

void set_value(TempRegister *reg) {
    Node *token;
    token = (Node*)malloc(sizeof(Node));
    token = create_node(reg->data, heap_register);
    heap_insert_head(token); // heap[h] <- Xi
    heap_register += 1;
}

/* Get Instructions */
void get_structure() {

}

void get_variable() {

}

void get_value(){

}

/* Unify Instructions */
void unify_variable(){

}

void unify_value() {

}
