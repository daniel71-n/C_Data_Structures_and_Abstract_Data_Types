#include "tree.c"
#include <stdio.h>

int main(void){
    BinaryTree *t;
    bool a;
    a = BT_contains(t, 'c');
    printf("status is %i\n", a);

    t = BT_append(t, 'c');
    a = BT_contains(t, 'c');
    printf("status is %i\n", a);

    BT_print(t);

    t = BT_append(t, 'e');
    t = BT_append(t, 'a');
    t = BT_append(t, 'b');
    t = BT_append(t, 'f');
    t = BT_append(t, 'd');
    BT_append(t, 'a');
    BT_append(t, 'm');

    unsigned int depth = BT_max_depth(t);
    printf("max depth is %u\n", depth);


    depth = BT_find_max(t); 
    printf("max is %u\n", depth);


    depth = BT_find_min(t); 
    printf("min is %u\n", depth);

    BT_print(t);
    BT_invert(t);
    BT_print(t);

    a = BT_contains(t, 'm');
    printf("status is %i\n", a);

    int count;
    count = BT_count_nodes(t);





};
