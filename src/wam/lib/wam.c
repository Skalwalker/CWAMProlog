#include "../include/wam.h"

void token_children(TreeNode* root) {
    if (root == NULL) {
        return;
    }

    if (root->node_type == NODE_TERM) {
        if (root->term_data != NULL) {
            RegTable *reg = table_reg_lookup(root->term_data->nome);
            add_stream(reg->reg);
            printf("%s ", root->term_data->nome);
            return;
        }
    } else if (root->node_type == NODE_STR) {
        RegTable *reg = table_reg_lookup(root->term_data->nome);
        add_stream(reg->reg);
        printf("%s ", root->str_data->nome);
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
        RegTable *reg = table_reg_lookup(root->str_data->nome);
        add_stream(reg->reg);
        printf("%s ", root->str_data->nome);
        token_children(root->left);
        token_children(root->right);
    }
}

void allocate_reg_table(TreeNode *node) {
    TermData* term;
    DataType* data;
    int value;

    if (node->node_type == NODE_TERM) {
        term = node->term_data;
        if (term != NULL) {
            data = create_data(REF_SYMBOL, 0, NULL);
            value = table_add_reg(term->nome, data);
        }
    } else if (node->node_type == NODE_STR) {
        data = create_data(STR_SYMBOL, 0, NULL);
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


// void build_heap(){

// }


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
