#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdio.h>
#include <stdlib.h>

#define ELEM_T double
#define FORM_T "%7.4lf"
#define FORM_O "%lf"

#define RED           "\x1b[31;2m"
#define YELLOW        "\x1b[33;2m"
#define GREEN         "\x1b[32;2m"
#define BLUE          "\x1b[34;2m"
#define END_OF_COLOUR "\x1b[39;49m"

typedef struct _tree
{
    ELEM_T value;
    struct _tree* right;
    struct _tree* left;
    int height;
}TREE;

enum ROTATION
{
    right_rotate = 1,
    left_rotate = 2,
};

TREE* tree_ctor(ELEM_T source);

void tree_dump(TREE* tree);

void tree_insert(TREE** tree, ELEM_T insert_value);

void tree_right_rotation(TREE** tree);

void tree_left_rotation(TREE** tree);

void check_tree_balance(TREE** tree);

void update_height(TREE* tree);

void print_sorted_tree(TREE* tree);

void tree_dtor(TREE** tree);

ELEM_T tree_median(TREE** tree, int n_elems);

void tree_pop(TREE** tree);

#endif
