#include "../include/instruction_set.h"

int mode;

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
void unify_variable(TempRegister *reg){
    switch (mode) {
    case WRITE:
        /* code */
        break;
    case READ:
        /* code */
        break;

    default:
        break;
    }
    subterm_register += 1;
}

void unify_value(TempRegister *reg) {
    switch (mode) {
    case WRITE:
        /* code */
        break;
    case READ:
        /* code */
        break;

    default:
        break;
    }

    subterm_register;
}

void deref() {

}

void unify() {

}
