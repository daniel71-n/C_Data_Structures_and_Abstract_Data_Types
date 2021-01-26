#include "binary_search_tree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


BinaryTree *BT_append(BinaryTree *tree, char the_value){
    if (!tree){
        BinaryTree *newnode;
        BinaryTree *temp = NULL;
        
        if (!(temp = malloc(sizeof(BinaryTree)))){
        return NULL;
        };
    
        newnode = temp;
        newnode->left_child = NULL;
        newnode->right_child = NULL; 
        newnode->data = the_value;

        tree = newnode; 
        return tree;
        }

    else{

        if (the_value <= tree->data){
            tree->left_child = BT_append(tree->left_child, the_value);
        }else{
            tree->right_child= BT_append(tree->right_child, the_value);
        };
    };
    
    return tree;    // return the whole tree back to the caller
};




bool BT_contains(BinaryTree *tree, char the_value){
    if (!tree){
        return false;
    };

    if (tree->data == the_value){
        return true;
    }else if (the_value < tree->data){
        BT_contains(tree->left_child, the_value); 
    }else{
        BT_contains(tree->right_child, the_value);
    };
    return true;
};



uint16_t BT_count_nodes(BinaryTree *tree){
    /* Count the number of nodes in the tree recursively */
    if (!tree){     // if tree is null
        return 0;
    }else{
        return 1 + BT_count_nodes(tree->left_child) + BT_count_nodes(tree->right_child);
    };
};



char BT_find_min(BinaryTree *tree){
    /* Find and return the minimum value in tree.
       This function assumes the tree contains at lesat one node,
       and thus a crash will happen if the argument is a NULL BinaryTree pointer.

       Since a Binary Search Tree keeps its items ordered, with
       the smaller ones being on the left and larger ones on the right, 
       the item with the smallest value will be the left-most node. 

       This can easily be written both either recursively or iteratively. 

       Operation of the recursive version:
        - if the left child is a null pointer (i.e. there's no left child),
            return the data value of the current node (the parent of said child node)
        - otherwise, if the left child is NOT a null pointer, recurse (until the 
            aforementioned condition becomes true)
    */
    if(!tree->left_child){
        return tree->data;
    };
   return BT_find_min(tree->left_child);    // recurse until the condition above is true

/* // iterative version 
   char val = tree->data;
   BinaryTree *current;
   while (current->left_child){
       val = tree->left_child->data;
       current = tree->left_child;
   };
*/

};

char BT_find_max(BinaryTree *tree){
    /* Find and return the minimum value in tree.
       This function assumes the tree contains at lesat one node,
       and thus a crash will happen if the argument is a NULL BinaryTree pointer.

       See the description of the BT_find_min complementary function above.
       
       Since a Binary Search Tree keeps its items ordered, with
       the smaller ones being on the left and larger ones on the right, 
       the item with the largest value will be the right-most node. 
    */
    if (!tree->right_child){
        return tree->data;
    };
    return BT_find_max(tree->right_child);
};



void BT_invert(BinaryTree *tree){
    /* Switch the positions of each left and right child of each node 
       in the tree. 
       Also known as 'mirroring'.
    */
    if (!tree){
        return;
    };

    BinaryTree *temp;
    temp = tree->left_child;
    tree->left_child = tree->right_child;
    tree->right_child = temp;

    BT_invert(tree->left_child);
    BT_invert(tree->right_child);
};


uint16_t BT_max_depth(BinaryTree *tree){

    if (!tree){
        return 0;
     };
   
    uint16_t depth_left = BT_max_depth(tree->left_child);
    uint16_t depth_right = BT_max_depth(tree->right_child);
    // return left if larger than right, otherwise right
    return depth_left > depth_right ? depth_left++ : depth_right++; 
    // Note: at some point, both depth_left and depth_right will be 0 - when the recursion
    // finishes. Since they're equal, the function will retur depth_right, according to
    // the conditional above. This is irrelevant. The function is supposed to return the
    // maximum depth- it doesn't have to be a singular quantity: that is, if both
    // left_depth and right_depth are actually 7 nodes deep, and thus are equal,
    // and the function returns 7, it doesn't matter which of the paths it refers to.
};


void BT_print(BinaryTree *tree){
   /* Print out the values of all the nodes in tree in ascending order */ 
    if (!tree){
        return;
    }else{
        BT_print(tree->left_child);     // smallest value or equal to the following
        printf("%c", tree->data);   // greater than or equal to left child
        BT_print(tree->right_child);    // > than the parent and thus the left sibling
    };
};





// bool BT_same_tree(BinaryTree *tree1, BinaryTree *tree2);
