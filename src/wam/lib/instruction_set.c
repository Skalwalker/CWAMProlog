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
// void get_structure(TempRegister *reg) {
//     int addr;
//     addr = deref(reg->num);
//     switch (1) { //STORE[addr]
//         case 1: // <REF, _>
//             // HEAp[h] <- <STR, H+1>;
//             // HEAp[h+1] <- f/n;
//             // bind(addr, H);
//             heap_register += 2;
//             mode = WRITE;
//             break;
//         case 2: // <STR, a>
//             if (1) { // heap[a] = f/n
//                 // subterm_register = a + 1
//                 mode = READ;
//             } else {
//                 fail = 1;
//             }
//             break;

//         default:
//             fail = 1;
//             break;
//     }
// }

void get_variable() {

}

void get_value(){

}

/* Unify Instructions */
void unify_variable(TempRegister *reg){
    Node *token;

    switch (mode) {
        case READ:
            reg->data->heap_ref = subterm_register;
            break;
        case WRITE:
            token = (Node*)malloc(sizeof(Node));
            // x_i <- heap[h]
            reg->data->heap_ref = heap_register;
            token = create_node(reg->data, heap_register);
            // heap[h] <- <REF H>
            heap_insert_head(token);
            heap_register += 1; // h <- h + 1
            break;
        default:
            break;
    }
    subterm_register += 1;
}

void unify_value(TempRegister *reg) {
    Node *token;

    switch (mode) {
        case READ:
            unify();
            break;
        case WRITE:
            token = (Node*)malloc(sizeof(Node));
            token = create_node(reg->data, heap_register);
            heap_insert_head(token); // heap[h] <- Xi
            heap_register += 1; // h <- h + 1
            break;
        default:
            break;
    }

    subterm_register += 1;
}

int deref(int address) {
    DataType *term;
    // STORE[a]
    term = fetch_node(address)->data;
    if ((term->data_type == REF_SYMBOL)&&(term->heap_ref != address)) {
        return deref(term->heap_ref);
    } else {
        return address;
    }
}

void unify() {

}
