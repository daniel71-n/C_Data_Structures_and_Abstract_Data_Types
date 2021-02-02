#include "binary_search_tree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



/* ***************************** Private ****************************** */
/* -------------------------------------------------------------------- */

static char BST_get_in_order_successor_P(BinaryTree *tree_ptr){
    /* Return the value of the in-order successor of tree_ptr.
       Called by the node removal routine.
    */
    if(!tree_ptr->left_child){  // can't go any farther left, so this is the in-order successor
        return tree_ptr->data;        //  to the right, there are only larger values
    }
    return BST_get_in_order_successor_P(tree_ptr->left_child);  // tail recursion
};



static uint16_t BST_find_depth_P(BinaryTree *tree){
    /* Traverse the tree depth-first and return when reaching a leaf. 
       On the way back up the call chain, each return value (1) from
       the returning function is added up. The sum total is then
       returned*.
       
       * Returned to the public function BT_max_depth(), which is
       the way BST_find_depth_P() is supposed to be called
    */  
    if (!tree){
        return 0;
    };
   
    uint16_t depth_left = BST_find_depth_P(tree->left_child);
    uint16_t depth_right = BST_find_depth_P(tree->right_child);

    // return left if larger than right, otherwise right
    return (depth_left > depth_right) ? ++depth_left : ++depth_right; 

    // Note: at some point, both depth_left and depth_right will be 0 - when the recursion
    // finishes. Since they're equal, the function will return depth_right, according to
    // the conditional above. This is irrelevant. The function is supposed to return the
    // maximum depth- it doesn't have to be a singular quantity: that is, if both
    // left_depth and right_depth are actually 7 nodes deep, and thus are equal,
    // and the function returns 7, it doesn't matter which of the paths it refers to.
}



static void BST_cut_down_P(BinaryTree *tree_ptr){
    /* Traverse the tree in post-order (left-right-root)
       and free all the nodes. 

       So the left node is freed first, then the right node, then the root. 
       The free operations are done on the way back up the call chain, so
       it all happens from the bottom-up -- from the leaves to root

       The base case is a NULL pointer. The function will go as far left as possible. 
       When it runs into a dead end (NULL), it will then try to go as far right
       as possible, with the same outcome. The next thing to do is then to free
       the current node, as it's a childless node (no left or right children). 
    */
    if (!tree_ptr){
        return;
    }
    else{ // go as far left as possible
        BST_cut_down_P(tree_ptr->left_child);
        //go as far right as possible
        BST_cut_down_P(tree_ptr->right_child); 
        // nowhere left to go: tree_ptr is a leaf (childless node). Delete it and return
        free(tree_ptr);
        return;
    }
};


/* ---------------------------------------------------------------- */
/* ***************************** End Private ********************** */




BinaryTree *BST_insert(BinaryTree *tree, char the_value){
    /* Insert the_value into the tree. 
    
       The insertion operation is such that the sorted order of
       the tree is maintained. 
    */
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
            tree->left_child = BST_insert(tree->left_child, the_value);
        }else{
            tree->right_child= BST_insert(tree->right_child, the_value);
        };
    };
    
    return tree;    
};





bool BST_contains(BinaryTree *tree, char the_value){
    /* Return true if the tree contains the_value, false otherwise */

    if (!tree){
        return false;
    };

    if (tree->data == the_value){
        return true;
    }else if (the_value < tree->data){
       return BST_contains(tree->left_child, the_value); 
    }else{
        return BST_contains(tree->right_child, the_value);
    };
};




uint16_t BST_count_nodes(BinaryTree *tree){
    /* Count the number of nodes in the tree recursively */
    if (!tree){     // if tree is null
        return 0;
    }else{
        return 1 + BST_count_nodes(tree->left_child) + BST_count_nodes(tree->right_child);
    };
};



char BST_find_min(BinaryTree *tree){
    /* Find and return the minimum value in tree.
       This function assumes the tree contains at least one node,
       and thus a crash will happen if the argument is a NULL BinaryTree pointer.

       Since a Binary Search Tree keeps its items ordered, with
       the smaller ones being on the left and larger ones on the right, 
       the item with the smallest value will be the left-most node. 

       This can easily be written either recursively or iteratively. 

       Operation of the recursive version:
        - if the left child is a null pointer (i.e. there's no left child),
            return the data value of the current node (the parent of said child node)
        - otherwise, if the left child is NOT a null pointer, recurse (until the 
            aforementioned condition becomes true)
    */
    if(!tree->left_child){
        return tree->data;
    };
   return BST_find_min(tree->left_child);    // tail-recurse until the condition above is true

/* // iterative version 
   char val = tree->data;
   BinaryTree *current;
   while (current->left_child){
       val = tree->left_child->data;
       current = tree->left_child;
   };
*/
};




char BST_find_max(BinaryTree *tree){
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
    return BST_find_max(tree->right_child);
};





void BST_invert(BinaryTree *tree){
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

    BST_invert(tree->left_child);
    BST_invert(tree->right_child);
};





uint16_t BST_max_depth(BinaryTree *tree){
    /* Decrement the value returned by BST_find_depth()
       by 1 and return that. 

       The decrement is neccessary because BST_find_depth()
       will give root a depth of 1, while root really is supposed
       to have depth 0, and so its result is off by 1.

       The purpose of this function is therefore to provide a 'nonlocal'
       scope to the private BST_find_depth() so as to conveniently 
       return the correct value.
    */
    if (!tree){
        return 0;
    }
    return BST_find_depth_P(tree) -1;
};





void BST_print(BinaryTree *tree){
   /* Print out the values of all the nodes in tree in ascending order.
      The tree is traversed - 'walked' - 'in-order' (left-root-right).
   */ 
    if (!tree){
        return;
    }else{
        BST_print(tree->left_child);     // smallest value or equal to the following
        printf("%c", tree->data);   // greater than or equal to left child
        BST_print(tree->right_child);    // > than the parent and thus the left sibling
    };
};




bool BST_is_same(BinaryTree *tree1, BinaryTree *tree2){     
    /* Return true if tree1 and tree2 are completely identical,
       both in terms of structure and values stored, and false otherwise.
    */
    if (tree1 == NULL && tree2 == NULL){    // both trees are null : this could be when both trees have reached their leaves
        return true;
    }
    else if (tree1 && tree2){   // neither tree is NULL
        return(tree1->data == tree2->data \
              && BST_is_same(tree1->left_child, tree2->left_child) \
              && BST_is_same(tree1->right_child, tree2->right_child));
    }
    else{   // one tree is null, one not
        return false;
    }
};






BinaryTree *BST_remove_node(BinaryTree *tree, char the_value){
    /* Remove the_value from the tree, and return a pointer to 
       the tree. 
       
       --------- NOTES -----------

       - only the first occurence of the_value is removed. If there
       are duplicates (like this tree implementation allows), the
       remaining ones are left intact. 

       - if the_value isn't found, nothing happens. Therefore, if
       you call this function twice with a certain value (provided
       there are no duplicates), the second call will effect no
       changes. In other words, the function is idempotent (again,
       in the absence of duplicates, that is).

       Internally, it calls BST_get_in_order_successor_P() when
       a node with two children has to be removed, and then
       recursively calls itself down the corresponding branch.
    */

    // Case A. tree is NULL : nothing to do -> return it as is
    if (!tree){
        return tree;
    }

    // case B. tree is not NULL : determine which path to take -- left or right, and
    // recurse
    if (the_value < tree->data){
        tree->left_child = BST_remove_node(tree->left_child, the_value);
        return tree;
    }else if (the_value > tree->data){
        tree->right_child = BST_remove_node(tree->right_child, the_value);
        return tree;
    }   

    // Case C. otherwise, it means the current root is the value 
    else{

        // case C1. no children -> simply return NULL
        if (tree->left_child == NULL && tree->right_child == NULL){
            free (tree);
            return NULL;
        }

        // case C2. two children -> find the in-order successor
        else if (tree->left_child && tree->right_child){
            tree->data = BST_get_in_order_successor_P(tree->right_child);
            tree->right_child = BST_remove_node(tree->right_child, tree->data);
        }
        
        // case C3. one child --> replace the node with its only child
        else{
            if (tree->left_child == NULL){
                BinaryTree *temp = tree->right_child;
                free(tree);
                tree = temp;    // make the current node its successor
                /*
                Seomthing like this:
                    tree->data = tree->right_child->data;
                    free(tree->right_child);
                    tree->right_child = NULL;
                won't work. Because if that child has children of its own and you free it, you lose all the children.
                You only retain the value of that child alone and none of its children.
                I.e. save the child pointer along with all its subtrees, not only its key.
                */
            }
            else if (tree->right_child == NULL){
                BinaryTree *temp = tree->left_child;
                free(tree);
                tree = temp;
            }
            return tree;
        }
    }
return tree;
};



void BST_destroy(BinaryTree **tree_ref){
    /* Free all the maclloc'ed memory associated with
       the tree, then set the tree pointer back in caller
       space to NULL (note that the function parameter here
       is a tree *reference* pointer).

       The actual freeing is done by BST_cut_down_P(),
       which recursively traverses the tree 'in post-order'
       and frees all the nodes.
    */
    BST_cut_down_P(*tree_ref);
    *tree_ref = NULL;
};


/*
To implement: 
BinaryTree *BST_from_array(char the_array[]);
void BST_to_array(char the_array[]);
BinaryTree *BST_remove_duplicates(BinaryTree* tree);
BinaryTree *BST_copy_tree(BinaryTree *tree);
char BST_find_nth_max(BinaryTree *tree);
char BST_find_nth_min(BinaryTree *tree);
*/





