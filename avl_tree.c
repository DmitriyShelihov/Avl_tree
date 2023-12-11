#include "avl_tree.h"

#define NODE_H(x) (((x)==NULL)?-1:((x)->height))
#define NODE_L ((*tree)->left)
#define NODE_R ((*tree)->right)
#define NODE_R_R ((*tree)->right->right)
#define NODE_L_L ((*tree)->left->left)
#define NODE_L_R ((*tree)->left->right)
#define NODE_R_L ((*tree)->right->left)

ELEM_T _tree_left_elem(TREE* tree);

void update_height(TREE* tree)
{
    if (NODE_H(tree->left) >= NODE_H(tree->right))
        tree->height = NODE_H(tree->left) + 1;
    else
        tree->height = NODE_H(tree->right) + 1;
}

TREE* tree_ctor(ELEM_T source)
{
    TREE* tree = (TREE*)malloc(sizeof(TREE));

    tree->left = NULL;
    tree->right = NULL;
    tree->height = 0;
    tree->value = source;

    return tree;
}

void tree_insert(TREE** tree, ELEM_T insert_value)
{
    if (compare_items((*tree)->value, insert_value) <= 0)
    {
        if (NODE_R == NULL)
        {
            (*tree)->height = 1;
            NODE_R = tree_ctor(insert_value);
        }
        else
            tree_insert(&NODE_R, insert_value);
    }
    else
    {
        if (NODE_L == NULL)
        {
            (*tree)->height = 1;
            NODE_L = tree_ctor(insert_value);
        }
        else
            tree_insert(&NODE_L, insert_value);
    }

    update_height(*tree);
    check_tree_balance(tree);

}

void tree_swap(TREE* a, TREE* b)
{
    ELEM_T save_elem = a->value;
    a->value = b->value;
    b->value = save_elem;
}

void tree_left_rotation(TREE** tree)
{
    tree_swap(*tree, NODE_R);
    TREE* save_tree = NODE_L;

    NODE_L = NODE_R;
    NODE_R = NODE_R_R;

    NODE_L_R = NODE_L_L;
    NODE_L_L = save_tree;

    update_height(NODE_L);

    update_height(*tree);
}

void tree_right_rotation(TREE** tree)
{
    tree_swap(*tree, NODE_L);
    TREE* save_tree = NODE_R;

    NODE_R = NODE_L;
    NODE_L = NODE_L_L;

    NODE_R_L = NODE_R_R;
    NODE_R_R = save_tree;

    update_height(NODE_R);

    update_height(*tree);
}

void check_tree_balance(TREE** tree)
{
    if (NODE_H(NODE_R) - NODE_H(NODE_L) == -2)
    {
        if (NODE_H(NODE_L_R) - NODE_H(NODE_L_L) == 1)
            tree_left_rotation(&NODE_L);   //left-right rotate
        tree_right_rotation(tree);
    }
    else if (NODE_H(NODE_R) - NODE_H(NODE_L) == 2)
    {
        if (NODE_H(NODE_R_R) - NODE_H(NODE_R_L) == -1)
            tree_right_rotation(&NODE_R);  //right-left rotate
        tree_left_rotation(tree);
    }
}

void print_sorted_tree(TREE* tree)
{
    if (tree != NULL)
    {
        print_sorted_tree(tree->left);
        printf(FORM_O " ", tree->value);
        print_sorted_tree(tree->right);
    }
}

void tree_dump(TREE* tree)
{
    printf(YELLOW "TREE DUMP:\n" END_OF_COLOUR);
    node_dump(tree, 0);
}

int node_dump(TREE* tree, int h)
{
    if (tree == NULL)
        return 0;
    node_dump(tree->right, h + 1);
    int i = 0;
    for(; i < h; i++)
        printf("          ");
    printf(FORM_T "(%d)" "\n", tree->value, tree->height);
    node_dump(tree->left, h + 1);
}


void tree_pop(TREE** tree)
{
    if (NODE_H(*tree) == 0)
    {
        free(*tree);
        *tree = NULL;
    }
    else if (NODE_H(NODE_L) == -1 && NODE_H(NODE_R) != -1)
    {
        tree_swap(*tree, NODE_R);

        free(NODE_L);

        NODE_L = NODE_R_L;
        NODE_R = NODE_R_R;

        ((*tree)->height)--;
    }
    else
    {
        if (NODE_H(*tree) != 0)
            tree_pop(&NODE_L);

        if (NODE_H(*tree) != 0)
            update_height(*tree);

        if (NODE_H(*tree) != 0)
            check_tree_balance(tree);
    }
}

ELEM_T tree_median(TREE** tree, int n_elems)
{
    int i = 0;
    for (; i < n_elems/2; i++)
        tree_pop(tree);
    return _tree_left_elem(*tree);
}

ELEM_T _tree_left_elem(TREE* tree)
{
    if (NODE_H(tree) == 0 || tree->left == NULL)
        return tree->value;
    else
        return _tree_left_elem(tree->left);
}

void tree_dtor(TREE** tree)
{
    if (*tree != NULL)
    {
        tree_dtor(&NODE_L);
        free(NODE_L);
        tree_dtor(&NODE_R);
        free(NODE_R);
    }
}
