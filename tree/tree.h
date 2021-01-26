#include <stdbool.h>
#include <stdint.h>

typedef struct binary_tree BinaryTree; 

struct binary_tree{
    char data;
    BinaryTree *left_child;
    BinaryTree *right_child;

};

BinaryTree *BT_append(BinaryTree *tree, char the_value);
bool BT_contains(BinaryTree *tree, char the_value);
void BT_invert(BinaryTree *tree); 
uint16_t BT_count_nodes(BinaryTree *tree);
uint16_t BT_max_depth(BinaryTree *tree);
char BT_find_min(BinaryTree *tree);
char BT_find_max(BinaryTree *tree);
void BT_print(BinaryTree *tree);

