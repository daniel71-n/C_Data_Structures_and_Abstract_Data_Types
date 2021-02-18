#ifndef C_VECT_H
#define C_VECT_H

#include <stdint.h>
#include <stdlib.h>

/* **************************************** * * * ****************************************** */
/* -------------------------------------- OVERVIEW ----------------------------------------- */
/*
 * This is an implementation of a dynamic array - an array that's managed, being dynamically
 * shrunk or grown in size according to the runtime needs (according to how much of its
 * capacity is used up).
 *
 * This dynamic resizing capability is offered through the interface presented down below on
 * this page. 
 * 
 * There interface allows the management of two different types of array (only one at the same
 * time) : a char array, and an int32_t array. The size of an int32_t is platform-dependent.
 * It'll be 4 bytes, if available. Not that it's crucial that vect be initialized and the 
 * associated Vect functions be called with the right type (int32_t). Failure to do so might
 * either lead to an error, or a subtle bug. For example, passing a signed long long instead
 * of an int32_t is undefined behavior, as far as this implementation goes, as  the size of
 * a long long won't fit insode the memory reserved for a int32_t.
 *
 * The length of the array - whether char or int32_t - is limited by the size of 
 * int32_t (as int32_t is also the type used to hold the length and indexes of the managed
 * array). If 4 bytes, then the array is able to hold ~2^(32-1) items.
 *
 * The type of array to be managed can be specified using the C_ARRAY or I_ARRAY enum
 * constants (to be passed to Vect_init()), which stand for character array and 
 * integer (as mentioned, it's actually a int32_t array) array, respectively. 
 *
 * Vect_destroy() has to be called on the used Vect when no longer needed , so that all
 * the associated memory can be deallocated. 
 *
 *                              
 *                              * * *
 * Usage example
 *
 *      Vect myvect;     // declaring a vector
 *      // initializing myvect as a character array with an initial size of 20
 *      Vect_init(&myvect, C_ARRAY, 20);  // argument is a Vect pointer
 *
 *      char mychar = 'z';
 *      Vect_append(myvect, &mychar);  // the Vect_append() argumetn is a pointer to char
 *
 *      // deallocate myvect
 *      Vect_destroy(&myvect);  // argument is a Vect pointer
 *  
* ***************************************************************************************** */





/* ************************************************************************** */
/* ------------------------- Structs and Enums ------------------------ ----- */

enum array_types{C_ARRAY, I_ARRAY};


struct vector{

    union{
        char *c;             // inner array to be resized dynamically : used if type == C_ARRAY; 
        int32_t *i;             // memory will be allocated to this instead if type == I_ARRAY;
    }dynarray;          // managed array. To be initialized by Vect_init() as either an int or a char array
    
    // enum specifying the type of the managed array (and so which inner array gets allocated memory)
    enum array_types type;  
    // reference variable holding the size of the array. 
    // This is not the number of bytes, but the total number of 'positions'/indexes in the array;
    int32_t total_array_length;  
    // stores the last non-Nul index that currently has a value assigned to it. 
    // It'll at most be total_array_length - 3 before the array is automatically grown.
    int32_t last_index;             
};




/* ************************************************************************** */
/* ----------------------------- Type defs ---------------------------------- */
typedef struct vector *Vect;
typedef enum array_types vect_type;




/* ************************************************************************** */
/* ----------------------- Function Prototypes ------------------------------ */

/*  * * *  IMPORTANT NOTES  * * *  */
// val should be a pointer to either a char or an int32_t

void Vect_init(Vect *vector_to_initialize, vect_type INNER_ARRAY_TYPE, int32_t initial_size);
void Vect_destroy(Vect *target_vector_ref);

void Vect_rem(Vect target_vector, int32_t index);
void Vect_range_rem(Vect target_vector, int32_t starting_index, int32_t ending_index);

void Vect_append(Vect target_vector, void *val);
int32_t Vect_contains(Vect target_vector, void *val);
void Vect_set(Vect target_vector, void *val, int32_t index);    

char Vect_c_pop(Vect target_vector);
int32_t Vect_i_pop(Vect target_vector);

void Vect_c_add(Vect target_vector, char *string_to_append);
void Vect_i_add(Vect target_vector, int32_t *int_array_to_append, int32_t how_many);


#endif


