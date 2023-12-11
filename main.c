#include "avl_tree.h"

int compare_items(ELEM_T a, ELEM_T b)
{
    if (a > b)
        return 1;
    else if (a == b)
        return 0;
    return -1;
}

int main()
{
    ELEM_T a;
    scanf(FORM_O, &a);
    TREE* tree = tree_ctor(a);
    int n_elems = 1;

    while (scanf(FORM_O, &a) == 1)
    {
        tree_insert(&tree, a);
        n_elems++;
    }

    tree_dump(tree);
    print_sorted_tree(tree);
    printf(GREEN "\nMedian is: " FORM_T END_OF_COLOUR, tree_median(&tree, n_elems));
    tree_dtor(&tree);
    free(tree);
    return 0;
}
