#include "../include/wam.h"

Heap *heap = NULL;
PDLNode *pdl = NULL;

void execute_wam(TreeNode *root, int query_prog){
    if (query_prog == 0) {
        destroy_heap();
        reg_counter = 1;
        free_reg_map();
        free_xregisters();
        heap = create_heap();
        register_names(root);
        printf("\n======================== Instruções da Query ========================\n");
        query_token_stream(root);
        printf("=====================================================================\n");
    } else if (query_prog == 1) {
        reg_counter = 1;
        free_reg_map();
        register_names(root);
        printf("\n======================== Instruções de Unificação ========================\n");
        prog_token_stream(root);
        printf("==========================================================================\n");
        printf("\n======================== Heap Após Unificar ========================\n");
        print_heap_reversed();
        printf("======================================================================\n");
    }
}

void map_instruction(TreeNode *str, RegMapping *reg_map, int inst_type, int data_type) {
    int reg_num = reg_map->reg_num;

    if (inst_type == QUERY_INSTR) {
        if (reg_map->occ != 0){
            printf("set_value X%d\n", reg_num);
            set_value(reg_num);
        } else {
            if (data_type == STR_SYMBOL) {
                printf("put_structure %s X%d\n", str->str_data->nome, reg_num);
                put_structure(str->str_data, reg_num);
            } else if (data_type == REF_SYMBOL){
                printf("set_variable X%d\n", reg_num);
                set_variable(reg_num);
            }
        }
    } else if (inst_type == PROG_INSTR) {
        if ((reg_map->occ != 0)&&(data_type == REF_SYMBOL)){
            printf("unify_value X%d\n", reg_num);
            unify_value(reg_num);
        } else {
            if (data_type == STR_SYMBOL) {
                if ((reg_map->occ != 0)||(reg_num == 1)){
                    printf("get_structure %s X%d\n", str->str_data->nome, reg_num);
                    get_structure(str->str_data, reg_num);
                } else {
                    printf("unify_variable X%d\n", reg_num);
                    unify_variable(reg_num);
                }
            } else if (data_type == REF_SYMBOL){
                printf("unify_variable X%d\n", reg_num);
                unify_variable(reg_num);
            }
        }
    }
}

void process_token(TreeNode *node, int inst_type) {
    RegMapping *reg_map;

    if (node->node_type == NODE_TERM) {
        reg_map = find_reg_map(node->term_data->nome);
        map_instruction(NULL, reg_map, inst_type, REF_SYMBOL);
        reg_map->occ = 1;
    } else if (node->node_type == NODE_STR) {
        reg_map = find_reg_map(node->str_data->nome);
        map_instruction(node, reg_map, inst_type, STR_SYMBOL);
        reg_map->occ = 1;
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
                process_token(node, PROG_INSTR);
            }
        } else if (node->node_type == NODE_STR) {
            process_token(node, PROG_INSTR);
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
            process_token(root, QUERY_INSTR);
            return;
        }
    } else if (root->node_type == NODE_STR) {
        process_token(root, QUERY_INSTR);
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
        process_token(root, QUERY_INSTR);
        query_token_children(root->left);
        query_token_children(root->right);
    }
}

void allocate_reg_table(TreeNode *node) {
    TermData* term;
    // DataType* data;
    // Tag* new_tag;

    if (node->node_type == NODE_TERM) {
        term = node->term_data;
        if (term != NULL) {
            // data = create_data(REF_SYMBOL, -1, NULL);
            map_reg(term->nome);
        }
    } else if (node->node_type == NODE_STR) {
        // new_tag = create_tag(node->str_data->nome, node->str_data->arity);
        // data = create_data(STR_SYMBOL, -2, new_tag);
        map_reg(node->str_data->nome);
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
