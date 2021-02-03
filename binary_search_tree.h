#include <stdbool.h>
#include <stdint.h>

typedef struct binary_tree BinaryTree; 


BinaryTree *BST_append(BinaryTree *tree, char the_value);
bool BST_contains(BinaryTree *tree, char the_value);
void BST_invert(BinaryTree *tree); 
uint16_t BST_count_nodes(BinaryTree *tree);
uint16_t BST_max_depth(BinaryTree *tree);
char BST_find_min(BinaryTree *tree);
char BST_find_max(BinaryTree *tree);
void BST_print(BinaryTree *tree);
bool BST_is_same(BinaryTree *tree1, BinaryTree *tree2);
BinaryTree *BST_remove_node(BinaryTree *tree, char the_value);
void BST_destroy(BinaryTree **tree_ref);






