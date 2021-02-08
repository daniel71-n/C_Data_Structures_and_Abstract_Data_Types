#include <stdint.h>
#include <stdlib.h>

#include "mutable_set.h"
#include "binary_search_tree.h"




struct dynamic_set{
    BinaryTree items;
};


void Set_init(DSet *the_set){
    /* Initializes a Dset to the correct values after allocating
       memory. 
       This includes initializing the BinaryTree member.
    */
    // allocate memory for a dynamic_set struct
    DSet newset = malloc(sizeof(struct dynamic_set));
    if (!newset){
        exit(EXIT_FAILURE);
    }
    // initialize the inner BinaryTree member
    BST_init(&newset->items);
    *the_set = newset;
    return;
}


uint16_t Set_size(DSet the_set){
    /* Return the number of items in the set */
    BinaryTree inner_tree = the_set->items;
    return BST_count_nodes(inner_tree);
}


bool Set_is_empty(DSet the_set){
    /* Return true if the_set is empty, false otherwise */
    return (the_set->items) ? false : true;
}


bool Set_contains(DSet the_set, char the_value){
    /* Return true if the_value is found in the_set, false otherwise */
    BinaryTree inner_tree = the_set->items;
    return BST_contains(inner_tree, the_value);     // true if item if the_value is found
}


void Set_insert(DSet the_set, char the_value){
    /* Insert the_value into the set, if not already there */
    BinaryTree inner_tree = the_set->items;
    the_set->items = BST_insert_nd(inner_tree, the_value);   // insert_no_duplicates
}


void Set_del(DSet the_set, char the_value){
    /* Delete the_value from the_set, if found */
    BinaryTree inner_tree = the_set->items;
    the_set->items = BST_remove_node(inner_tree, the_value);
}


char *Set_items(DSet the_set){
    /* Returns a dynamically-allocated char array containing all the items
       in the_set. 
       It's the responsibility of the valler to call free() on this returned 
       array when no longer needed.
    */
    BinaryTree inner_tree = the_set->items;
    uint16_t size = Set_size(the_set);  // calling Set_size instead of BST_count_nodes() for the sake of maintainability
    char *items_array = malloc(sizeof(char) * (size+1));    // +1 for the terminating null char

    BST_to_array(inner_tree, items_array, 0); // traverse the tree in-order and write all the values to items_array
    items_array[size] = '\0';
    
    return items_array;
}



bool Set_is_same(DSet set1, DSet set2){
    uint16_t set1_size = Set_size(set1);
    if (set1_size != Set_size(set2)){
        return false;
    } // the sets differ in size, so they're different. No further checking required

    //else, they're the same size. Compare elements 
    char *set1_items = malloc(sizeof(char) * set1_size);
    char *set2_items = malloc(sizeof(char) * set1_size);
    
    if (!(set1_items && set2_items)){
        exit(EXIT_FAILURE);  // one of the allocations above failed
    }

    BST_to_array(set1->items, set1_items, 0); // get all the items in set1 in sorted order
    BST_to_array(set2->items, set2_items, 0); // ditto

    // iterate over the arrays and compare. If any incongruity is found, return false
    for (uint16_t i = 0; i < set1_size; i++){
        if (set1_items[i] != set2_items[i]){
            return false;
        }
    }
    free(set1_items);
    free(set2_items);
    // if the flow of control makes it thus far, return true
    return true;
};


bool Set_is_subset(DSet set1, DSet set2){
    /* Return true if set1 is a subset of set2.
       Otherwise, return false.
    */
    uint16_t set1_size = Set_size(set1);
    if (set1_size >= Set_size(set2)){
        return false;
    } //set1 can't possibly be a subset of subset2, since it's larger 

    //else, set1 is smaller, check elements 
    char *set1_items = malloc(sizeof(char) * set1_size);
    char *set2_items = malloc(sizeof(char) * set1_size);
    
    if (!(set1_items && set2_items)){
        exit(EXIT_FAILURE);  // one of the allocations above failed
    }

    BST_to_array(set1->items, set1_items, 0); // get all the items in set1 in sorted order
    BST_to_array(set2->items, set2_items, 0); // ditto

    // iterate over the arrays and compare. If any incongruity is found, return false
    for (uint16_t i = 0; i < set1_size; i++){
        if (set1_items[i] != set2_items[i]){
            return false;
        }
    }
    free(set1_items);
    free(set2_items);
    // if the flow of control makes it thus far, return true
    return true;
};



char *Set_union(DSet set1,  DSet set2){
    /* Return a dynamically allocated char array containing
       the union of set1 and set2. 

       It's the responsibility of the caller to then free
       this array when no longer needed, by calling free
       on it.
    */
     uint16_t set1_size = Set_size(set1);
     uint16_t set2_size = Set_size(set2);

    char *set1_items = malloc(sizeof(char) * set1_size);
    char *set2_items = malloc(sizeof(char) * set2_size);

    if (!(set1_items && set2_items)){ 
        exit(EXIT_FAILURE);  // one of the allocations above failed
    }
    BST_to_array(set1->items, set1_items, 0); // get all the items in set1 in sorted order
    BST_to_array(set2->items, set2_items, 0); // ditto
    
    /* build a BinaryTree out of the items in set1 and the items in set2, using the nd (no
     * duplicates) BST insert function. This tree will contain the union of the other two
     * sets, since it doesn't allow duplicates */
    BinaryTree union_tree;
    BST_init(&union_tree);

    // insert the items in the first set into the union_tree
    for (uint16_t i = 0; i < set1_size; i++){
        union_tree = BST_insert_nd(union_tree, set1_items[i]);
    }
    // do the same for the items in the second set
    for (uint16_t i = 0; i < set2_size; i++){
        union_tree = BST_insert_nd(union_tree, set2_items[i]);
    }
    // the two arrays are no longer needed
    free(set1_items);
    free(set2_items);

    // create an array equal in size to the number of items in the union_tree
    uint16_t union_size = BST_count_nodes(union_tree);
    char *set_union = malloc(sizeof(char) * (union_size+1));    // +1 for the terminating Nul
    if (!set_union){
        exit(EXIT_FAILURE);
    }

   // write the union tree to the array, and return that
    BST_to_array(union_tree, set_union, 0);
    set_union[union_size] = '\0';
    BST_destroy(&union_tree);

    return set_union;
}


char *Set_difference(DSet set1,  DSet set2){
    /* Return a malloc-ed char array containing the 
       elements in set1 that aren't in set2
    */
    uint16_t set1_size = BST_count_nodes(set1->items);
    char *set1_items = malloc(sizeof(char) * set1_size);
    char *set_difference = malloc(sizeof(char) * (set1_size +1));   // +1 for the terminating Nul
    
    if (!(set1_items && set_difference)){
        exit(EXIT_FAILURE);
    }
    
    // write all the elements in the set1 tree to the set1_items array
    BST_to_array(set1->items, set1_items, 0);

    uint16_t set_difference_index = 0;
    char current = 0;
    for (uint16_t i = 0; i<set1_size; i++){
        current = set1_items[i]; 
        // if current is in set1 but not in set2, add it to set_difference
        if (!(BST_contains(set2->items, current))){
            set_difference[set_difference_index] = current;
            set_difference_index++;
        }
    };
    set_difference[set_difference_index] = '\0';
    free(set1_items);
    return set_difference;
}


char *Set_intersection(DSet set1,  DSet set2){
    /* Return a malloc-ed char array containing the 
       items that are both in set1 and set2.
    */

    uint16_t set1_size = BST_count_nodes(set1->items);
    char *set1_items = malloc(sizeof(char) * set1_size);
    char *set_intersection = malloc(sizeof(char) * (set1_size +1));   // +1 for the terminating Nul
    
    if (!(set1_items && set_intersection)){
        exit(EXIT_FAILURE);
    }
    
    // write all the elements in the set1 tree to the set1_items array
    BST_to_array(set1->items, set1_items, 0);

    uint16_t set_intersection_index = 0;
    char current = 0;
    for (uint16_t i = 0; i<set1_size; i++){
        current = set1_items[i]; 
        // if current is in set1 but not in set2, add it to set_difference
        if (BST_contains(set2->items, current)){
            set_intersection[set_intersection_index] = current;
            set_intersection_index++;
        }
    };
   set_intersection[set_intersection_index] = '\0';
    free(set1_items);
    return set_intersection;
}


void Set_destroy(DSet *the_set){
    /* Deallocate all associated heap memory and set 
       *the_set to NULL 
    */
    if (!(*the_set)){
        return;
    }

    BinaryTree inner_tree = (*the_set)->items;
    BST_destroy(&inner_tree);
    
    free(*the_set);
    *the_set = NULL;
}


