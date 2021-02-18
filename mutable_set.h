#ifndef MUTSET_H
#define MUTSET_H

#include <stdbool.h>
#include <stdint.h>


/* ***************************************************************** */
/*                          * * *                                    */
/*                   Dynamic (Mutable) Set
 *
 *  This is an implementation of a mutable, aka dynamic Set ADT 
 *  (abstract data type), which operates on char primitives.
 *
 *  Being an ADT, a set is merely an interface consisting of 
 *  a set of operations. Under the hood, as it were, a Set can be 
 *  implemented using a number of different data structures. 
 *
 *  This particular implementation uses a Binary Search Tree,
 *  which is a common and natural way of implementing sets. 
 *  The BST offers logarithmic time complexity for insertion
 *  and lookup operations.
 *
 *  The set is SORTED.
 * 
 * *************************************************************** */

// a mutable, sorted set
typedef struct dynamic_set *DSet;


void Set_init(DSet *the_set);
bool Set_contains(DSet the_set, char the_value); 
char *Set_items(DSet the_set); // returns a dynamically - allocated char array containing all the items in the set
uint16_t Set_size(DSet the_set);
bool Set_is_empty(DSet the_set); 
bool Set_is_same(DSet set1, DSet set2);
void Set_insert(DSet the_set, char the_value);
void Set_del(DSet the_set, char the_value);
bool Set_is_subset(DSet set1, DSet set2);
char *Set_union(DSet set1,  DSet set2);
char *Set_difference(DSet set1,  DSet set2);
char *Set_intersection(DSet set1,  DSet set2);
void Set_destroy(DSet *the_set);   // destroy a DSet and free all memory associated with it
//void Set_destroy_items(DSetItems *set_items); // destroy a DSetItems and free all associated memory








#endif







