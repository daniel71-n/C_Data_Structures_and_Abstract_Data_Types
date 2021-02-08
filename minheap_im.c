#include "minheap_im.h"
#include <stdlib.h>



struct min_heap_implicit{
    int32_t size;
    int32_t last_index;
    char *array;    // array that needs to be regrown
};



/* ********************************************************************************* */
/* ---------------------------- PRIVATE FUNCTIONS ---------------------------------- */

static void Heap_sift_up(char the_array[], int32_t current_index){
    /* Sift the value at current index up in the array */
    char temp;
    int32_t parent = (current_index-1)>>1;  // parent is (current-1) / 2 (floor division)

    while( parent >= 0){
        if(the_array[parent] > the_array[current_index]){
           temp = the_array[current_index];
           the_array[current_index] = the_array[parent]; 
           the_array[parent] = temp;

           current_index = parent;
           parent = (current_index-1)>>1;
        }
        else{   // don't sift if parent !< current
            break;
        }

    } 
};



static void Heap_sift_down(char the_array[], int32_t last_index){
    /* Sift down the root to its correct position in the_array so
       as to repair and uphold the heap property (children > parent).
    */
    int32_t current = 0;
    int32_t left_child = 1 + (current<<1);
    int32_t right_child = 2 + (current<<1);
   
    char temp;
    while(left_child <= last_index && right_child <= last_index){
        int32_t smaller = (the_array[left_child] < the_array[right_child]) ? left_child : right_child;
        if (the_array[smaller] < the_array[current]){
            temp = the_array[smaller];
            the_array[smaller] = the_array[current];
            the_array[current] = temp;
        
            current = smaller;
            left_child = 1 + (current<<1);
            right_child = 2 + (current<<1);
        }
        else{
            break;
        }
            
    }
}
 
/* ------------------------------- END PRIVATE ------------------------------------- */
/* ********************************************************************************* */


void Heap_init(Heap *heap_ref, int32_t initial_size){
    // allocate memory for a min_heap_implicit struct
    Heap new_min_heap = malloc(sizeof(struct min_heap_implicit));
    char *new_array= malloc(sizeof(char) * initial_size);
    if (!(new_min_heap && new_array)){
        exit(EXIT_FAILURE);
    }
    
    new_min_heap->array = new_array;
    
    new_min_heap->last_index = -1;
    new_min_heap->size = initial_size;
    new_array[0] = '\0';   // in an array of n items, n+1 will be Nul. This has to be maintained

    *heap_ref = new_min_heap;
}

   
    


void Heap_insert(Heap the_heap, char the_value){
    /* Insert a new node into the heap */
    the_heap->array[++the_heap->last_index] = the_value; 
    the_heap->array[the_heap->last_index+1] = '\0';

    if(the_heap->last_index+3 > the_heap->size){
        // double the size of the array
        char *temp = realloc(the_heap->array, sizeof(char) * the_heap->size*2);
        
        if (!temp){
            exit(EXIT_FAILURE);
        }

        the_heap->array = temp;
        the_heap->size *= 2;
    }
    // if last_index is only 0, the heap only has root so far : no sift-up necessary
    if (the_heap->last_index > 0){
        Heap_sift_up(the_heap->array, the_heap->last_index);
    }
};
    


char Heap_pop(Heap the_heap){
    /* Remove and return the root of the_heap,
       then repair the heap_property.
    */
    char val = the_heap->array[0];  // get the root
    
    // reestablish the heap property, broken by having removed the root
    // put the last element in root's place
    the_heap->array[0] = the_heap->array[the_heap->last_index];
    the_heap->array[the_heap->last_index] = '\0';
    the_heap->last_index--;     // one less element in the heap now

    // check if the array needs shrinking
    if (the_heap->last_index < (the_heap->size/2)-2){
        // halve the size of the array
        the_heap->size /= 2;
        char *temp = realloc(the_heap->array, sizeof(char) * the_heap->size);
            
        if (!temp){
            exit(EXIT_FAILURE);
        }
    
        the_heap->array = temp;
    }

    // sift down the new root to the correct place in the array
    if (the_heap->last_index < 2){  
    // if there's only root and nothing else, not necessary
        if(the_heap->last_index == 0){
            return val;
        }
        else{ // root only has one child. Check if it needs swapping
            if (the_heap->array[0] > the_heap->array[1]){
                char temp = the_heap->array[1];
                the_heap->array[1] = the_heap->array[0];
                the_heap->array[0] = temp;
            }
            return val;
        }
    }
                
    Heap_sift_down(the_heap->array, the_heap->last_index);
    return val;
}


void Heap_destroy(Heap *heap_ref){
    /* Free all meory assocaited with the heap
       and set *heap_ref to NULL.
    */
    if (!(*heap_ref)){
        return;
    }

    free((*heap_ref)->array);
    free(*heap_ref);
    *heap_ref = NULL;
}


