#include "../include/wam.h"

Heap *heap = NULL;
PDLNode *pdl = NULL;

void execute_wam(TreeNode *root, int query_prog){
    if (query_prog == 0) {
        destroy_heap();
        heap = create_heap();
        register_names(root);
        print_registers();
        query_token_stream(root);
    } else if (query_prog == 1) {
        reg_counter = 1;
        table_reg_delete();
        register_names(root);
        print_registers();
        prog_token_stream(root);
    }
}

void map_instruction(TempRegister *reg, int occ, int init, int inst_type) {
    if (inst_type == QUERY_INSTR) {
        if (occ == 1){
            printf("set_value X%d\n", reg->num);
            set_value(reg);
        } else {
            if (reg->data->data_type == STR_SYMBOL) {
                printf("put_structure %s X%d\n", reg->data->tag->name, reg->num);
                put_structure(reg);
            } else if (reg->data->data_type == REF_SYMBOL){
                printf("set_variable X%d\n", reg->num);
                set_variable(reg);
            }
        }
    } else if (inst_type == PROG_INSTR) {
        if ((occ == 1)&&(reg->data->data_type == REF_SYMBOL)){
            printf("unify_value X%d\n", reg->num);
            unify_value(reg);
        } else {
            if (reg->data->data_type == STR_SYMBOL) {
                if (init == 1){
                    printf("get_structure %s X%d\n", reg->data->tag->name, reg->num);
                    get_structure(reg);
                } else {
                    printf("unify_variable X%d\n", reg->num);
                    unify_variable(reg);
                }
            } else if (reg->data->data_type == REF_SYMBOL){
                printf("unify_variable X%d\n", reg->num);
                unify_variable(reg);
            }
        }
    }

    // print_heap();
}

void process_token(TreeNode *node, int init, int inst_type) {

    if (node->node_type == NODE_TERM) {
        RegTable *reg = table_reg_lookup(node->term_data->nome);
        // add_stream(reg->reg);
        // printf("X%d = %s \n", reg->reg_num, node->term_data->nome);
        map_instruction(reg->reg, reg->on_stream, init, inst_type);
        reg->on_stream = 1;
    } else if (node->node_type == NODE_STR) {
        RegTable *reg = table_reg_lookup(node->str_data->nome);
        // add_stream(reg->reg);
        // printf("X%d = %s \n", reg->reg_num, node->term_data->nome);
        map_instruction(reg->reg, reg->on_stream, init, inst_type);
        reg->on_stream = 1;
    }

    return;
}

Queue *root_queue = NULL;

void prog_token_child(TreeNode *root) {
    Queue *str;

    if (root == NULL) {
        return;
    }

    if (root->node_type == NODE_TERM) {
        if (root->term_data != NULL) {
            str = (Queue*)malloc(sizeof *str);
            str->data = root;
            str->visited = 0;
            str->inited = 0;
            LL_APPEND(root_queue, str);
            return;
        }
    } else if (root->node_type == NODE_STR) {
        str = (Queue*)malloc(sizeof *str);
        str->data = root;
        str->visited = 0;
        str->inited = 0;
        LL_APPEND(root_queue, str);
        return;
    }

    prog_token_child(root->left);
    prog_token_child(root->right);
}


void prog_token_stream(TreeNode* root) {
    TreeNode *node;
    Queue *current = NULL;
    Queue *str;
    int first = 0;

    str = (Queue*)malloc(sizeof *str);
    str->data = root;
    str->visited = 0;
    str->inited = 0;
    LL_APPEND(root_queue, str);

    while (root_queue != NULL) {
        current = root_queue;
        LL_DELETE(root_queue, current);
        node = current->data;

        if (first==0) {
            current->inited = 1;
        }

        if ((node->node_type == NODE_STR)&&(first == 1)) {
            if (current->visited == 0) {
                str = (Queue*)malloc(sizeof *str);
                str->data = node;
                str->visited = 1;
                str->inited = 0;
                LL_APPEND(root_queue, str);
            }

            if((current->visited == 1)&&(current->inited == 0)){
                current->inited = 1;
            }
        }

        if (node->node_type == NODE_TERM) {
            if (node->term_data != NULL) {
                process_token(node, current->inited, PROG_INSTR);
            }
        } else if (node->node_type == NODE_STR) {
            process_token(node, current->inited, PROG_INSTR);
            first = 1;
        }

        if (current->visited != 1) {
            prog_token_child(node->left);
            prog_token_child(node->right);
        }

        current->visited = 1;
    }
}


void query_token_children(TreeNode* root) {
    if (root == NULL) {
        return;
    }

    if (root->node_type == NODE_TERM) {
        if (root->term_data != NULL) {
            process_token(root, 0, QUERY_INSTR);
            return;
        }
    } else if (root->node_type == NODE_STR) {
        process_token(root, 0, QUERY_INSTR);
        return;
    }

    query_token_children(root->left);
    query_token_children(root->right);
}

void query_token_stream(TreeNode* root) {
    TreeNode *left, *right;

    if (root == NULL) {
        return;
    }

    if ((root->left == NULL)&&(root->right == NULL)) {
        return;
    }


    left = root->left;
    right = root->right;

    query_token_stream(left);
    query_token_stream(right);

    if (root->node_type == NODE_STR) {
        process_token(root, 0, QUERY_INSTR);
        query_token_children(root->left);
        query_token_children(root->right);
    }
}

void allocate_reg_table(TreeNode *node) {
    TermData* term;
    DataType* data;
    Tag* new_tag;

    if (node->node_type == NODE_TERM) {
        term = node->term_data;
        if (term != NULL) {
            data = create_data(REF_SYMBOL, -1, NULL);
            table_add_reg(term->nome, data);
        }
    } else if (node->node_type == NODE_STR) {
        new_tag = create_tag(node->str_data->nome, node->str_data->arity);
        data = create_data(STR_SYMBOL, -2, new_tag);
        table_add_reg(node->str_data->nome, data);
    }

}

void register_names(TreeNode* root) {
    TreeNode *node;
    Queue *head = NULL;
    Queue *current = NULL;
    Queue *str;

    str = (Queue*)malloc(sizeof *str);
    str->data = root;
    LL_APPEND(head, str);

    while (head != NULL) {
        current = head;
        LL_DELETE(head, current);
        node = current->data;

        allocate_reg_table(node);

        if (node->left != NULL) {
            str = (Queue*)malloc(sizeof *str);
            str->data = node->left;
            LL_APPEND(head, str);
        }

        if (node->right != NULL) {
            str = (Queue*)malloc(sizeof *str);
            str->data = node->right;
            LL_APPEND(head, str);
        }
    }
}

// p(Z, h(Z, W), f(W)).
//
// Flattened
//
// X3 = h(X2, X5)
// X4 = f(X5)
// X1 = p(X2, X3, X4)
//
// Tokenized
//
// [X3 (h/3), X2, X5, X4(f/1), X5, X1(p/3), X2, X3, X4]
