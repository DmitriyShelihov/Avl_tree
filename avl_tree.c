#include "avl_tree.h"

int node_dump(TREE* tree, int h);

void _tree_dtor(TREE* tree);

void _tree_insert(TREE** tree, ELEM_T insert_value);

TREE* tree_ctor(ELEM_T source)
{
    TREE* tree = (TREE*)malloc(sizeof(TREE));
    TREE* tree_left = (TREE*)malloc(sizeof(TREE));
    TREE* tree_right = (TREE*)malloc(sizeof(TREE));

    tree->left = tree_left;
    tree->right = tree_right;
    tree->height = 0;
    tree->value = source;

    tree->left->height = -1;
    tree->right->height = -1;

    return tree;
}

void tree_swap(TREE* a, TREE* b)
{
    ELEM_T save_elem = a->value;
    a->value = b->value;
    b->value = save_elem;
}

void tree_dtor(TREE** tree)
{
    _tree_dtor(P_TREE);
    free(P_TREE);
}

void _tree_dtor(TREE* tree)
{
    if (tree->height != -1)
    {
        _tree_dtor(tree->left);
        free(tree->left);
        free(tree->right);
        _tree_dtor(tree->right);
    }
}

void print_sorted_tree(TREE* tree)
{
    if (tree->height != -1)
    {
        print_sorted_tree(tree->left);
        printf("%lf ", tree->value);
        print_sorted_tree(tree->right);
    }
}

void update_height(TREE* tree)
{
    if (tree->left->height >= tree->right->height)
        tree->height = tree->left->height + 1;
    else
        tree->height = tree->right->height + 1;
}

void tree_pop(TREE** tree)
{
    if (P_TREE->height == 0)
    {
        free(TREE_L);
        free(TREE_R);
        P_TREE->height = -1;
    }
    else if (TREE_L->height == -1 && TREE_R->height != -1)
    {
        tree_swap(P_TREE, TREE_R);

        free(P_TREE->left);

        P_TREE->left = TREE_R->left;
        TREE* save_elem = TREE_R;
        P_TREE->right = TREE_R->right;

        free(save_elem);

        (P_TREE->height)--;

    }
    else
    {
        if (P_TREE->height != 0)
            tree_pop(&TREE_L);

        if (P_TREE->height != 0 )
            update_height(P_TREE);

        if (P_TREE->height != 0)
            check_tree_balance(&P_TREE);
    }
}

ELEM_T tree_median(TREE** tree, int n_elems)
{
    for (int i = 0; i < n_elems/2; i++)
        tree_pop(tree);
    return _tree_left_elem(*tree);
}

ELEM_T _tree_left_elem(TREE* tree)
{
    if (tree->height == 0 || tree->height == -1)
        return tree->value;
    else
        return _tree_left_elem(tree->left);
}

void tree_right_rotation(TREE** tree)
{
    tree_swap(*tree, TREE_L);
    TREE* save_tree = TREE_R;

    TREE_R = TREE_L;
    TREE_L = TREE_L->left;

    TREE_R->left = TREE_R->right;
    TREE_R->right = save_tree;

    (TREE_R->height) --;

    update_height(TREE_R);

    update_height(P_TREE);
}

void tree_left_rotation(TREE** tree)
{
    tree_swap(*tree, TREE_R);
    TREE* save_tree = TREE_L;

    TREE_L = TREE_R;
    TREE_R = TREE_R->right;

    TREE_L->right = TREE_L->left;
    TREE_L->left = save_tree;

    (TREE_L->height)--;

    update_height(TREE_L);

    update_height(P_TREE);
}

void tree_balance(TREE** tree, ROTATION type_of_rotate)
{
    switch(type_of_rotate)
    {
    case right_rotate:
        tree_right_rotation(tree);
        break;
    case left_rotate:
        tree_left_rotation(tree);
        break;
    default:
        printf(RED "ERROR: didn't found type of rotation in tree_balance\n" END_OF_COLOUR);
        break;
    }
}

void tree_insert(TREE** tree, ELEM_T insert_value)
{
    _tree_insert(tree, insert_value);

    check_tree_balance(tree);
}

void _tree_insert(TREE** tree, ELEM_T insert_value)
{
    if (compare_items(P_TREE->value, insert_value) <= 0)
    {
        if (TREE_R->height == -1)
        {
            P_TREE->height = 1;
            free(TREE_R);
            TREE_R = tree_ctor(insert_value);

            return;
        }
        else
        {
            _tree_insert(&TREE_R, insert_value);

            update_height(P_TREE);

            check_tree_balance(&P_TREE);

            return;
        }
    }
    else
    {
        if (TREE_L->height == -1)
        {
            P_TREE->height = 1;
            free(TREE_L);
            TREE_L = tree_ctor(insert_value);

            return;
        }
        else
        {
            _tree_insert(&TREE_L, insert_value);

            update_height(P_TREE);
            check_tree_balance(&P_TREE);
            return;
        }
    }
}

void check_tree_balance(TREE** tree)
{
    if (TREE_R->height - TREE_L->height == -2)
    {
        if (TREE_L->right->height - TREE_L->left->height == 1)
        {
            tree_balance(&(TREE_L), left_rotate);   //left-right rotate
            tree_balance(&P_TREE, right_rotate);
            return;
        }
        else
        {
            tree_balance(&P_TREE, right_rotate);     //right rotate
            return;
        }
    }
    else if (TREE_R->height - TREE_L->height == 2)
    {
        if (TREE_R->right->height - TREE_R->left->height == -1)
        {
            tree_balance(&TREE_R, right_rotate);  //right-left rotate
            tree_balance(&P_TREE, left_rotate);
            return;
        }
        else
        {
            tree_balance(&P_TREE, left_rotate);   //left rotate
            return;
        }
    }
}

void tree_dump(TREE* tree)
{
    printf(YELLOW "TREE DUMP:\n" END_OF_COLOUR);
    node_dump(tree, 0);
}

int node_dump(TREE* tree, int h)
{
    if (!tree || tree->height == -1)
        return 0;
    else
    {
        node_dump(tree->right, h+1);

        for (int i = 0; i < h; i++)
            printf("          ");

        printf(FORM_T "(%d)" "\n", tree->value, tree->height);
        node_dump(tree->left, h + 1);
    }
}

