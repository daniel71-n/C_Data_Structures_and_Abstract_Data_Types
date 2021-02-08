#include <assert.h>

#include "vect.h"




/* ******************************************************************************* */
/* ----------------------------- PRIVATE FUNCTIONS ------------------------------- */

void static Vect_shrink_P(Vect target_vector, void *managed_array, size_t size_of_item){
    /*  Halve the amount of memory allocated to the managed array in Vect.

        Called by Vect_check_size_shrink_P().
    */
    void *temp = realloc(managed_array, target_vector->total_array_length/2 * size_of_item);
    if (!temp){
        exit(EXIT_FAILURE);
    }
    else{
        if (target_vector->type == C_ARRAY) {target_vector->dynarray.c=temp;}
        else if (target_vector->type == I_ARRAY) {target_vector->dynarray.i=temp;}
        target_vector->total_array_length /= 2;
    }
}



void static Vect_check_size_shrink_P(Vect target_vector, vect_type INNER_ARRAY_TYPE){
    /*  Check if the last_index value is less than half that of total_array_length,
        i.e. if the managed array is using less than half of the memory allocated, 
        and call Vect_shrink_P if that's found to be the case. 

        Called by the likes of Vect_c_pop() and Vect_i_pop().
    */
    switch(INNER_ARRAY_TYPE){

        case C_ARRAY:
            //-2 because last_index starts counting at 0, whereas total_array_length counts from 1
            // and the last element in a string has to be Nul. That makes last_index ==
            // total_array_length-2
            // the reason I make it so that the array gets shrunk only if LESS THAN HALF
            // of the array capacity is used, as opposed to exactly half of the capacity,
            // is to leave some room for at least another insertion before the vector
            // needs to be regrown again
            if (target_vector->last_index < target_vector->total_array_length/2 - 2){   // if only less than half the array capacity is being used 
                Vect_shrink_P(target_vector, target_vector->dynarray.c, sizeof(char));
            }
            break;

        case I_ARRAY:
            if (target_vector->last_index < target_vector->total_array_length/2 - 1){       // no terminating Nul in an int array
                Vect_shrink_P(target_vector, target_vector->dynarray.i, sizeof(int32_t));
            }
            break;
    }
}



void static Vect_grow_P(Vect target_vector, void *managed_array, size_t size_of_item){
    /*  Double the amount of memory allocated to the managed array in Vect.

        Called by Vect_check_size_grow_P().
    */
    void *temp = realloc(managed_array, target_vector->total_array_length * 2 * size_of_item);
    if (!temp){
        exit(EXIT_FAILURE);
    }
    
    if (target_vector->type == C_ARRAY) {target_vector->dynarray.c=temp;}
    else if (target_vector->type == I_ARRAY) {target_vector->dynarray.i=temp;}
    target_vector->total_array_length *= 2;
}



void static Vect_check_size_grow_P(Vect target_vector, vect_type INNER_ARRAY_TYPE){
    /* Check whether the array needs to be doubled in size.

       Double the size of the array as soon as last_index + 2 = total_array_length (i.e.
       last_index + 3 > total_array_length).
       Given an array that can hold 7 chars, and assuming we have 'abcdNUL', 5 out of the 
       7 slots in the array are filled. Last_index will be 3, since counting starts at 0,
       and last_index tracks the index of the item BEFORE Nul. If we insert another item,
       'e', getting 'abcdeNul', 6/7 slots will be filled, with last_index being 4.
       If we do yet another insertion, 'f', getting 'abcdefNUL', the whole array is now
       filled, and last_index is 5, pointing to the sixt element in the array, with Nul
       being the seventh. Thus last_index (which is 5) + 3 > total_array_length (which
       is 7)becomes true, and the array is doubled in size.

                         * * *  

       The way the function determines this is by evaluating if last_index+3 > total_array_length. 
       In more detail: given 5-item array, total_array_length will be 5. But the last possible index,
       since counting starts at 0, is 4. This is either filled by a NUL (in a char array), or empty
       (left so, for simplicity's sake, so that the array has the same length regardless of whether
       it's a char or an int array). 
       So the last actual index filled with a value is 3, which is the fourth item in the array.
       Therefore when a value is assigned to index 3 (the last item bar NUL), the array should be 
       doubled in size. The smallest value that would satisfy the condition given above, upon evaluation,
       and trigger the calling of Vect_grow_P is 3. If the value were 2 instead, last_index
       would be incremented by one again up to 4 (continuing with the 5-item array example above),
       which would make it the last allowable index inside the array bounds, and NUL
       would be overwritten. So Vect_grow_P needs to be called BEFORE this point,
       when index 3, rather than 4, is assigned a value.
    */
    if (target_vector->last_index+3 > target_vector->total_array_length){

        switch(INNER_ARRAY_TYPE){
            case C_ARRAY:
                Vect_grow_P(target_vector, target_vector->dynarray.c, sizeof(char)); 
                break;
            case I_ARRAY:
                Vect_grow_P(target_vector, target_vector->dynarray.i, sizeof(int32_t));
                break;
            }
    }
}


/* -------------------------------- END PRIVATE ---------------------------------- */
/* ******************************************************************************* */



void Vect_init(Vect *vector_to_initialize_ref, vect_type INNER_ARRAY_TYPE, int32_t initial_array_size){
   /* */ 
    
    Vect new = malloc(sizeof(struct vector));
    if (!new){
        exit(EXIT_FAILURE);
    }
    *vector_to_initialize_ref = new;

    Vect vector_to_initialize = *vector_to_initialize_ref;
    vector_to_initialize->type = INNER_ARRAY_TYPE;  // set the type of the array based on the type enum passed as a parameter;
    
    switch (vector_to_initialize->type) {

        case C_ARRAY:   // if the argument specifies a char array
            {   // can't declare variables inside a 'case' if not wrapped in curly braces (so as to define a new scope)
                char *check_not_null;
                if (!(check_not_null = calloc(initial_array_size, sizeof(char)))){     // if the returned value is NULL
                    exit(EXIT_FAILURE);
                }
                vector_to_initialize->dynarray.c = check_not_null;
                vector_to_initialize->total_array_length = initial_array_size;
                vector_to_initialize->last_index = -1;
                break;
            }

            case I_ARRAY:   // if the argument specifies an int array
            {
                int32_t *check_not_null; 
                if (!(check_not_null = calloc(initial_array_size, sizeof(int32_t)))){     // if the returned value is NULL
                    exit(EXIT_FAILURE);
                }
                vector_to_initialize->dynarray.i = check_not_null; 
                vector_to_initialize->total_array_length=initial_array_size;
                vector_to_initialize->last_index = -1;
                break;
            }
    }
}



void Vect_append(Vect target_vector, void *val){
    /* Append a value (specified by the val argument) to the managed array 
       in Vect as specified by the target_vector argument.

      The index the val argument is assigned to is the last_index member in the Vect struct, + 1.  
      The type of the managaded array is determined by looking at the 'type' memeber: I_ARRAY, C_ARRAY.
    */
    switch(target_vector->type){

        case C_ARRAY:
            {
            char value = *(char*)val;
            // last_index's initial value is set to -1, so the first value will be inserted at index -1+1 => 0.
            target_vector->dynarray.c[target_vector->last_index+1] = value;       
            // increment last_index, and consequently the position of the next append operation. 
            target_vector->last_index++;      
            // set the value of the index immediately following last_index to NUL.
            target_vector->dynarray.c[target_vector->last_index+1]= '\0';       // Strings MUST be Nul-terminated
            Vect_check_size_grow_P(target_vector, C_ARRAY);
            // check if the managed array needs growing.
            }
            break;

        case I_ARRAY:
            {
            int32_t value = *(int32_t*)val;
            target_vector->dynarray.i[target_vector->last_index+1] = value;       
            target_vector->last_index++;                         
            target_vector->dynarray.i[target_vector->last_index+1] = 0;          // redundant, since this will never be read
            Vect_check_size_grow_P(target_vector, I_ARRAY);
            }
            break;
    }
}



void Vect_c_add(Vect target_vector, char *string_to_append){
    /* Append each char in string_to_append to target_vector's managed array  */ 
    assert(target_vector->type==C_ARRAY && "is C_ARRAY");   // abort if .type !=  C_ARRAY

    for (int32_t ind = 0; string_to_append[ind] != '\0'; ind++) {
        Vect_append(target_vector, &string_to_append[ind]);
    }
}



void Vect_i_add(Vect target_vector, int32_t *int_array_to_append, int32_t how_many){
    /* Append how_many items from int_array_to_append to target_vector's managed array.

       Since '\0' (NUL) can't be used to mark and find the end of the array, like 
       with strings, Vect_i_add - unlike Vect_c_add - has a third parameter (how_many),
       specifying the number of items to be copied from int_array_to_append.

       An alternative would be using another sentinel value e.g. -1, but that's 
       counting on the int_array_to_append not forgetting to include one,
       which is more precarious than simply passing an argument.
    */ 
    assert(target_vector->type==I_ARRAY && "is I_ARRAY");   // abort if .type !=  C_ARRAY

    for (int32_t ind = 0; ind < how_many; ind++) {     
        Vect_append(target_vector, &int_array_to_append[ind]);
    }
}



char Vect_c_pop(Vect target_vector){
    /* Get the last char in the managed array of the Vect struct and return it. 

       The char will be REMOVED from the array by setting that index position to NUL,
       and decrementing last_index, thus practically shrinking the length of the string by 1.
    */ 
    assert(target_vector->type==C_ARRAY && "is C_ARRAY");   // abort if .type !=  C_ARRAY

    char popped = target_vector->dynarray.c[target_vector->last_index];
    target_vector->dynarray.c[target_vector->last_index] = '\0';
    target_vector->last_index --;
    Vect_check_size_shrink_P(target_vector, C_ARRAY);
    return popped;
}



int32_t Vect_i_pop(Vect target_vector){
    /* Find the last int in the managed array of the vector and return it. 

       The int will be REMOVED from the array, and last_index will be decremented, 
       shrinking the length of the array (i.e. the value of total_array_length) by 1.
    */ 
    assert(target_vector->type==I_ARRAY && "is I_ARRAY");   // abort if .type !=  C_ARRAY
   
    int popped = target_vector->dynarray.i[target_vector->last_index];
    target_vector->dynarray.i[target_vector->last_index] = 0;   // never read afterward, only written
    target_vector->last_index --;
    Vect_check_size_shrink_P(target_vector, I_ARRAY);

    return popped;
}




void Vect_rem(Vect target_vector, int32_t index){
    /* REMOVE the value at index INDEX in the managed array of the vector.

       This will be a comparatively costly operation, since all the values at index n > INDEX 
       have to be shifted back by 1.

       Also call private_vectCheckSize to determine whether the array needs to be shrunk.
    */

    switch(target_vector->type){

        case C_ARRAY:   // if the managed array is a char array
            if (index <= target_vector->last_index){
                for( int32_t ind = index; ind <= target_vector->last_index; ind++){
                    target_vector->dynarray.c[ind] = target_vector->dynarray.c[ind+1];  // shift back one position all values from start_index onward ;
                }
                target_vector->dynarray.c[target_vector->last_index] = '\0'; 
                target_vector->last_index--; 
                Vect_check_size_shrink_P(target_vector, C_ARRAY);
            }
            else{
                 return;
            }
            break; 

        case I_ARRAY:   // else if the managed array is an int array
            if (index <= target_vector->last_index){
                for( unsigned int ind = index; ind<= target_vector->last_index; ind++){
                    target_vector->dynarray.i[ind] = target_vector->dynarray.i[ind+1];  // shift back on position all values from start_index onward ;
                }
                target_vector->last_index--;
                Vect_check_size_shrink_P(target_vector, I_ARRAY); 
            }
            else{
                return;
            }
            break;
    }
}




void Vect_range_rem(Vect target_vector, int32_t starting_index, int32_t ending_index){
    /* Range Remove : REMOVE the values in the managed array of the Vect struct that are between 
       starting_index (inclusive) and ending_index (exclusive).

       If less than half of the array capacity is used (call private_vectCheckSize to check), 
       halve the memory allocated to it (private_vectCheckSize will in turn call private_vectSrink).
    */
    assert(starting_index < ending_index);  // raise an exception if ending index <= starting_index
    
    if (! (starting_index < target_vector->last_index && ending_index <= target_vector->last_index+1)){
        exit(EXIT_FAILURE);
        return;
    }

    int32_t last_index_before_deletion = target_vector->last_index;
    
    switch(target_vector->type){


        case C_ARRAY:
            for (int32_t start = starting_index; start < ending_index; start++){
                target_vector->dynarray.c[start] = '\0';
                target_vector->last_index--;    // safe to be decremented since last_index_before_deletion stores the initial value from before any change
            }
            // determine how many items will be removed, and then how many items will need shifting back how many positions
            if (ending_index < last_index_before_deletion+1){    
            // if the range of deleted items is within the array, rather than up to the end of it (as in that case nothing would need to be shifted back)
                int32_t num_of_pos_to_shift_back = ending_index - starting_index;     // the number of positions all the items at index n>=ending_index need to be shifted back;
                for (int32_t start = ending_index; start <= last_index_before_deletion; start++){
                    target_vector->dynarray.c[start - num_of_pos_to_shift_back] = target_vector->dynarray.c[start];
                }
                for (int32_t start = target_vector->last_index+1; start <= last_index_before_deletion; start++){
                    // set to Nul everything after last_index up to and including last_index_after_deletion
                    target_vector->dynarray.c[start] = '\0';  
                }
            }           
            Vect_check_size_shrink_P(target_vector, C_ARRAY);
            break;
    

        case I_ARRAY:
            for (int32_t start = starting_index; start < ending_index; start++){
                target_vector->dynarray.i[start] = 0;
                target_vector->last_index--;    // safe to be decremented since last_index_before_deletion stores the initial value from before any change
            }
            if (ending_index < last_index_before_deletion+1){    
                int32_t num_of_pos_to_shift_back = ending_index - starting_index;     
                for (int32_t start = ending_index; start <= last_index_before_deletion; start++){
                    target_vector->dynarray.i[start - num_of_pos_to_shift_back] = target_vector->dynarray.i[start];
                }
                for (int32_t start = target_vector->last_index+1; start <= last_index_before_deletion; start++){
                    target_vector->dynarray.i[start] = 0;  // 0 everything after last_index up to and including last_index_after_deletion
                }
            }           
            Vect_check_size_shrink_P(target_vector, I_ARRAY); 
            break;
    }
}




int32_t Vect_contains(Vect target_vector, void *val){
    /* Return an integer representing the index of the first occurence of val, if found, 
       else -1. 
    */
    switch(target_vector->type){

        case C_ARRAY:{
            char value = *(char*)val;
            int32_t res;

            for (int32_t ind = 0; ind <= target_vector->last_index; ind++){
                if (target_vector->dynarray.c[ind] == value){
                    res = ind;
                    return res;
                    break;
                }
                else if (target_vector->dynarray.c[ind] == '\0'){
                    return -1;
                    break;
                }
            }
            return res;
        }
        break;


    case I_ARRAY:{
        int32_t value = *(int32_t*)val;
        int32_t res;
        for (int32_t ind = 0; ind <= target_vector->last_index; ind++){
            if (target_vector->dynarray.i[ind] == value){
                res=ind;
                return res;
                break;
            }
        }
        return -1;
        }
        break;
    }
}



void Vect_set(Vect target_vector, void *val, int32_t index){
    /* Assign val to the managed array of the vector at index INDEX,
       if index is <= last_index, else append the value instead.
    */
    switch(target_vector->type){

        case C_ARRAY:
            {
            char value = *(char*)val;   // cast the void pointer to a char ptr, then dereference that
            if (index <= target_vector->last_index){
                target_vector->dynarray.c[index] = value;
            }
            else{
                Vect_append(target_vector, &value);
            }
            }
            break;


        case I_ARRAY:
            {
            int32_t value = *(int32_t*)val;
            if (index <= target_vector->last_index){
                target_vector->dynarray.i[index] = value;
                }
            else{
                Vect_append(target_vector, &val);
            }
            }
            break;
        }
}


void Vect_destroy(Vect *target_vector_ref){
    /*  Free the memory allocated to the managed array of the Vect struct. */

    Vect target_vector = *target_vector_ref;

    switch(target_vector->type){
     
        case C_ARRAY:
            free(target_vector->dynarray.c);
            break;
        
        case I_ARRAY:   
            free(target_vector->dynarray.i);
            break;
        }

    free(target_vector);
    *target_vector_ref = NULL;
}


