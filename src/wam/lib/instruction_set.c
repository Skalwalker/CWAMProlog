#include "../include/instruction_set.h"

int mode;
int fail = 0;

/* Put Instructions */
void put_structure(TempRegister *reg) {
    Node *token, *functor;
    DataType *data;
    // heap[h] <- <Str H+1>
    data = reg->data;
    // x_i <- heap[h]
    data->heap_ref = heap_register + 1;
    token = create_node(data, heap_register);
    heap_insert_head(token);
    // heap[h] <- nome/aridade
    data = create_data(TAG_SYMBOL, -2, reg->data->tag);
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
void get_structure(TempRegister *reg) {
    DataType *new_data;
    Node *str, *functor;
    int addr = deref(reg->data->heap_ref);
    printf("Chegou aqui!");
    Node* found_node = find_in_heap(addr);
    DataType *data = found_node->data;
    printf("%d", addr);

    if (data->heap_ref == -1) {
        printf("Chegou aqui 2!");
        // heap[h] <- <Str H+1>
        new_data = create_data(STR_SYMBOL, heap_register+1, NULL);
        str = create_node(new_data, heap_register);
        heap_insert_head(str);
        // heap[h] <- f/n
        new_data = create_data(TAG_SYMBOL, -2, reg->data->tag);
        functor = create_node(new_data, heap_register+1);
        heap_insert_head(functor);
        bind(addr, heap_register);
        heap_register += 2;
        mode = WRITE;
    } else if (data->data_type == TAG_SYMBOL) {
        printf("Chegou aqui 3!");
        if (data->tag != NULL) {
            subterm_register = found_node->index + 1;
            mode = READ;
        } else {
            fail = 1;
            printf("O programa falhou em unificar!\n");
            exit(0);
        }
    } else {
        printf("Chegou aqui 4!");
        fail = 1;
        printf("O programa falhou em unificar!\n");
        exit(0);
    }
}

void get_variable() {

}

void get_value(){

}

/* Unify Instructions */
void unify_variable(TempRegister *reg){
    Node *token;
    switch (mode) {
        case WRITE:
            token = (Node*)malloc(sizeof(Node));
            // x_i <- heap[h]
            reg->data->heap_ref = heap_register;
            token = create_node(reg->data, heap_register);
            // heap[h] <- <REF H>
            heap_insert_head(token);
            heap_register += 1;
            break;
        case READ:
            reg->data->heap_ref = subterm_register;
            break;
        default:
            break;
    }
    subterm_register += 1;
}

void unify_value(TempRegister *reg) {
    Node *token;

    switch (mode) {
        case WRITE:
            token = (Node*)malloc(sizeof(Node));
            token = create_node(reg->data, heap_register);
            heap_insert_head(token); // heap[h] <- Xi
            heap_register += 1;
            break;
        case READ:
            unify(reg->data->heap_ref, subterm_register);
            break;

        default:
            break;
    }
    subterm_register  += 1;
}

int deref(int address) {
    DataType *data = find_in_heap(address)->data;

    if ((data->data_type == REF_SYMBOL)&&(data->heap_ref != address)){
        return deref(data->heap_ref);
    } else {
        return address;
    }
}

PDLNode *create_pdl_node(int addr){
    PDLNode *new = (PDLNode*)malloc(sizeof(PDLNode));
    new->address = addr;
    return new;
}

void unify(int addr1, int addr2) {
    STACK_PUSH(pdl, create_pdl_node(addr1));
    STACK_PUSH(pdl, create_pdl_node(addr2));
    fail = 0;
    while (!(pdl != NULL || fail == 1)){
        int d1, d2;
        PDLNode *elt;
        STACK_POP(pdl,elt);
        d1 = deref(elt->address); 
        STACK_POP(pdl,elt);
        d2 = deref(elt->address);
        if (d1 != d2) {
            DataType *data1, *data2;
            data1 = find_in_heap(d1)->data;
            data2 = find_in_heap(d2)->data;
            if ((data1->data_type == REF_SYMBOL)||(data2->data_type == REF_SYMBOL)){
                bind(d1, d2);
            } else {
                Tag *tag1, *tag2;
                tag1 = find_in_heap(data1->heap_ref)->data->tag;
                tag2 = find_in_heap(data2->heap_ref)->data->tag;
                if(strcmp(tag1->name, tag2->name) == 0) {
                    for (int i = 1; i <= tag1->arity; i++){
                        STACK_PUSH(pdl, create_pdl_node(data1->heap_ref+i));
                        STACK_PUSH(pdl, create_pdl_node(data2->heap_ref+i));
                    }
                } else {
                    fail = 1;
                    printf("O programa falhou em unificar!");
                    exit(0);
                }
            }
        } 
    }
}

void bind(int addr1, int addr2){
    Node *n1, *n2;
    n1 = find_in_heap(addr1);
    n2 = find_in_heap(addr2);
    if ((n1->data->heap_ref == -1)&&(n2->data->heap_ref >= 0)){
        n1->data->heap_ref = n2->data->heap_ref;
    } else if  ((n1->data->heap_ref >= 0)&&(n2->data->heap_ref == -1)) {
        n2->data->heap_ref = n1->data->heap_ref;
    } else {
        n1->data->heap_ref = n2->data->heap_ref;
    }
}