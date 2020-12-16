#include "../include/instruction_set.h"

int mode;
int fail = 0;

/* Put Instructions */
void put_structure(StrData *str, int reg_num) {
    // HEAP[H] = <STR, H+1>
    DataType *data = create_data(STR_SYMBOL, heap_register+1, NULL);
    Node *cell = create_node(data, heap_register);
    heap_insert_head(cell);
    // HEAP[H+1] = f/n
    DataType *data_tag = create_data(TAG_SYMBOL, -2, create_tag(str->nome, str->arity));
    cell = create_node(data_tag, heap_register+1);
    heap_insert_head(cell);
    // Xi = HEAP[H]
    create_register(reg_num, data);
    // H = H + 2
    heap_register += 2;
}

void put_variable() {

}

void put_value() {

}

/* Set Instructions */
void set_variable(int reg_num) {
    // HEAP[H] = <REF, H>
    DataType *data = create_data(REF_SYMBOL, heap_register, NULL);
    Node *cell = create_node(data, heap_register);
    heap_insert_head(cell);
    // Xi = HEAP[H]
    create_register(reg_num, data);
    // H = H + 1
    heap_register += 1;
}

void set_value(int reg_num) {
    // HEAP[H] = Xi
    XRegister *reg = find_register(reg_num);
    DataType *new_data = create_data(reg->data->data_type, reg->data->heap_ref, NULL);
    Node *cell = create_node(new_data, heap_register);
    heap_insert_head(cell);
    // H = H + 1
    heap_register += 1;
}

/* Get Instructions */
void get_structure(StrData *tag, int reg_num) {
    DataType *new_data;
    Node *str, *functor;
    XRegister *reg = find_register(reg_num);
    int addr = deref(reg->data->heap_ref);
    Node* found_node = find_in_heap(addr);
    DataType *data = found_node->data;

    if (data->heap_ref == found_node->index) {
        // heap[h] <- <Str H+1>
        new_data = create_data(STR_SYMBOL, heap_register+1, NULL);
        str = create_node(new_data, heap_register);
        heap_insert_head(str);
        // heap[h] <- f/n
        new_data = create_data(TAG_SYMBOL, -2, create_tag(tag->nome, tag->arity));
        functor = create_node(new_data, heap_register+1);
        heap_insert_head(functor);
        bind(addr, heap_register);
        heap_register += 2;
        mode = WRITE;
    } else if (data->data_type == TAG_SYMBOL) {
        if (data->tag != NULL) {
            subterm_register = found_node->index + 1;
            mode = READ;
        } else {
            fail = 1;
            printf("O programa falhou em unificar!\n");
            exit(0);
        }
    } else {
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
void unify_variable(int reg_num){
    DataType *data;
    Node *cell;
    XRegister *reg;

    switch (mode) {
        case WRITE:
            // HEAP[H] = <REF, H>
            data = create_data(REF_SYMBOL, heap_register, NULL);
            cell = create_node(data, heap_register);
            heap_insert_head(cell);
            // Xi = HEAP[H]
            reg = create_register(reg_num, data);
            // H = H + 1
            heap_register += 1;
            break;
        case READ:
            // Xi = HEAP[S]
            cell = find_in_heap(subterm_register);
            create_register(reg_num, cell->data);
            break;
        default:
            break;
    }
    subterm_register += 1;
}

void unify_value(int reg_num) {
    Node *cell;
    DataType *new_data;
    XRegister *reg;

    switch (mode) {
        case WRITE:
            // HEAP[H] = Xi
            reg = find_register(reg_num);
            new_data = create_data(reg->data->data_type, reg->data->heap_ref, NULL);
            cell = create_node(new_data, heap_register);
            heap_insert_head(cell);
            // H = H + 1
            heap_register += 1;
            break;
        case READ:
            reg = find_register(reg_num);
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
    PDLNode *element;
    element = create_pdl_node(addr1);
    STACK_PUSH(pdl, element);
    element = create_pdl_node(addr2);
    STACK_PUSH(pdl, element);
    fail = 0;

    while (!(pdl == NULL || fail == 1)){
        int d1, d2;
        PDLNode *elt;
        STACK_POP(pdl, elt);
        d1 = deref(elt->address);
        STACK_POP(pdl, elt);
        d2 = deref(elt->address);
        if (d1 != d2) {
            DataType *data1, *data2;
            data1 = find_in_heap(d1)->data;
            data2 = find_in_heap(d2)->data;
            if ((data1->data_type == REF_SYMBOL)||(data2->data_type == REF_SYMBOL)){
                bind(d1, d2);
            } else {
                Tag *tag1, *tag2;
                int local_ref_1, local_ref_2;
                if (data1->data_type == TAG_SYMBOL) {
                    tag1 = data1->tag;
                    local_ref_1 = d1;
                } else {
                    tag1 = find_in_heap(data1->heap_ref)->data->tag;
                    local_ref_1 = data1->heap_ref;
                }
                if (data1->data_type == TAG_SYMBOL) {
                    tag2 = data2->tag;
                    local_ref_2 = d2;
                } else {
                    tag2 = find_in_heap(data2->heap_ref)->data->tag;
                    local_ref_2 = data2->heap_ref;
                }

                if(strcmp(tag1->name, tag2->name) == 0) {
                    for (int i = 1; i <= tag1->arity; i++){
                        element = create_pdl_node(local_ref_1+i);
                        STACK_PUSH(pdl, element);
                        element = create_pdl_node(local_ref_2+i);
                        STACK_PUSH(pdl, element);
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
    if ((n1->data->heap_ref == n1->index)&&(n2->data->heap_ref != n2->index)){
        n1->data->heap_ref = n2->data->heap_ref;
    } else if  ((n1->data->heap_ref != n1->index)&&(n2->data->heap_ref == n2->index)) {
        n2->data->heap_ref = n1->data->heap_ref;
    } else {
        n2->data->heap_ref = n1->data->heap_ref;
    }
}
