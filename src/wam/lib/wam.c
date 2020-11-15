#include "../include/wam.h"

int heap_register = 0;
Heap *heap = NULL;

void execute_wam(TreeNode *root){
    heap = create_heap();
    register_names(root);
    print_registers();
    token_stream(root);
    print_stream();
    print_heap_reversed();
}

void map_instruction(TempRegister *reg, int occ) {

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
}

void process_token(TreeNode *node) {

    if (node->node_type == NODE_TERM) {
        RegTable *reg = table_reg_lookup(node->term_data->nome);
        add_stream(reg->reg);
        map_instruction(reg->reg, reg->on_stream);
        reg->on_stream = 1;
        // printf("%s ", node->term_data->nome);
    } else if (node->node_type == NODE_STR) {
        RegTable *reg = table_reg_lookup(node->str_data->nome);
        add_stream(reg->reg);
        map_instruction(reg->reg, reg->on_stream);
        reg->on_stream = 1;
        // printf("%s ", node->str_data->nome);
    }

    return;
}

void token_children(TreeNode* root) {
    if (root == NULL) {
        return;
    }

    if (root->node_type == NODE_TERM) {
        if (root->term_data != NULL) {
            process_token(root);
            return;
        }
    } else if (root->node_type == NODE_STR) {
        process_token(root);
        return;
    }

    token_children(root->left);
    token_children(root->right);
}

void token_stream(TreeNode* root) {
    TreeNode *left, *right;

    if (root == NULL) {
        return;
    }

    if ((root->left == NULL)&&(root->right == NULL)) {
        return;
    }


    left = root->left;
    right = root->right;

    token_stream(left);
    token_stream(right);

    if (root->node_type == NODE_STR) {
        process_token(root);
        token_children(root->left);
        token_children(root->right);
    }
}

void allocate_reg_table(TreeNode *node) {
    TermData* term;
    DataType* data;
    Tag* new_tag;
    int value;

    if (node->node_type == NODE_TERM) {
        term = node->term_data;
        if (term != NULL) {
            data = create_data(REF_SYMBOL, 0, NULL);
            value = table_add_reg(term->nome, data);
        }
    } else if (node->node_type == NODE_STR) {
        new_tag = create_tag(node->str_data->nome, node->str_data->arity);
        data = create_data(STR_SYMBOL, 0, new_tag);
        value = table_add_reg(node->str_data->nome, data);
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
