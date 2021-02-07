#include <stdbool.h>
#include <stdint.h>

typedef struct binary_tree *BinaryTree; 

// structure of a binary tree node
struct binary_tree{
    char data;
    BinaryTree left_child;
    BinaryTree right_child;
};



void BST_init(BinaryTree *tree_ref);
BinaryTree BST_insert(BinaryTree tree, char the_value);
bool BST_contains(BinaryTree tree, char the_value);
void BST_invert(BinaryTree tree); 
uint16_t BST_count_nodes(BinaryTree tree);
uint16_t BST_max_depth(BinaryTree tree);
char BST_find_min(BinaryTree tree);
char BST_find_max(BinaryTree tree);
void BST_print(BinaryTree tree);
bool BST_is_same(BinaryTree tree1, BinaryTree tree2);
BinaryTree BST_remove_node(BinaryTree tree, char the_value);
unsigned int BST_to_array(BinaryTree the_tree, char the_array[], unsigned int index);
BinaryTree BST_from_array(char the_array[], unsigned int array_length);
void BST_destroy(BinaryTree *tree_ref);






