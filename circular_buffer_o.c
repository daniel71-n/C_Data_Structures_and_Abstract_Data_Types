#include "circular_buffer_o.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

CircularBuffer *CircularBuffer_init(void){
    /* Create a CircularBuffer instance. Allocate space on the heap for a
     * CircularBuffer type, using malloc, and initialize the internal struct members by
     * setting free_count to BUFFER_SIZE (buffer is empty) and read_index and write_index
     * to 0. The inner_array (which is the buffer proper) is initialized to 0 with memset.
     */
 
    CircularBuffer *tmp_ptr;  // temporary pointer used to test that malloc successfully allocates the needed memory
    CircularBuffer *circular_buffer_ptr = NULL;   // initialize pointer to NULL; 

    if ((tmp_ptr = malloc(sizeof(CircularBuffer)))){  // if the call to malloc didn'return a NULL, meaning it was successful
       printf("malloc worked; size of tmp_ptr is %zu\n", sizeof(CircularBuffer)); // error code to mean that the malloc-based memory allocation failed;
       circular_buffer_ptr = tmp_ptr;
       circular_buffer_ptr->free_count = BUFFER_SIZE;
       circular_buffer_ptr->read_index=0;
       circular_buffer_ptr->write_index = 0;
       memset(circular_buffer_ptr->inner_array, 0, BUFFER_SIZE);    // initialize the inner array to all 0s
    }; 
    
    return circular_buffer_ptr;
}



bool CircularBuffer_is_empty(CircularBuffer *cb_ptr){
    /* Return true (1) if the buffer is empty or false (0) if not */
    bool is_empty;  
    is_empty = (cb_ptr->read_index == cb_ptr->write_index? true : false); // assign 1 to is_empty if free_count is equal to BUFFER_SIZE
    
    return is_empty;
};


bool CircularBuffer_is_full(CircularBuffer *cb_ptr){
    /* Return true (1) if the buffer is full or false (0) if not */
    bool is_full;  
    is_full = (cb_ptr->free_count == 0 ? true : false); 
    
    return is_full;
};



char CircularBuffer_read(CircularBuffer *cb_ptr){
    /* A read operation consists of:
        - indexing the inner_array struct member with read_index
        - incrementing read_index in preparation for the next read (advance it in the buffer)
        - incrementing the free_counts variable, to signal that another slot has been freed
       No reads are done when read_index == write_index, which practically means that the 
       buffer is empty.

       Returns 0 when the read failed (such as when the buffer is empty), or the read value, 
       if successful.
     */
    char return_val = 0;    // the value to be returned -- either 0, in case of failure, or the char in the buffer

    if (cb_ptr->read_index == cb_ptr->write_index){
        // the read_index should never be incremented past the write_index
        return return_val;
    };
    return_val = cb_ptr->inner_array[cb_ptr->read_index];   // else, read one item from the array as indexed by read_index
    
    // update the internal variables of the buffer:
    cb_ptr->free_count++;       // indicate that there's room for another item to be written
    cb_ptr->read_index++;       // advance the read index, BUT:
    /*some bounds checking needs to be done: if the read_index becomes greater than
     * MAX_INDEX, after being incremented, then it needs to be reset to 0, so that 
     * the reading wraps around and starts reading from the beginning of the buffer - 
     * i.e. the start of the array - again.
     */
    if (cb_ptr->read_index > MAX_INDEX){
        cb_ptr->read_index = 0; 
    };
    return return_val;
}

uint8_t CircularBuffer_write(CircularBuffer *cb_ptr, char char_to_write){
    /* Return 0 on success and 1 on failure.

       A write operation consists of the following: 
        - index the inner_array struct member with write_index;
        - write the actual value; 
        - increment write_index in preparation for the next write;
        - decrement free_count to indicate that there's one less slot available for future writes.

       Writes are always done. When the buffer is full, items will start getting overwritten,
       beginning with the oldest one. If the previous batch of items are being overwritten
       due to the buffer being full, and the read_index goes past the read_index, only
       the new items being written will be accessible. The old items are considered to
       not be there. See the header file (circular_buffer_o.h) for a more detailed explanation.
        */
    uint8_t return_val = 1; 
    printf("char to write is %c\n", char_to_write);
    // ELSE:
    cb_ptr->inner_array[cb_ptr->write_index] = char_to_write;   // write char_to_write to the buffer
    if (cb_ptr->free_count > 0){
        cb_ptr->free_count--;
    };  // only decrement free-count if >0; otherwise, the buffer is already empty and it's being overwritten;
    cb_ptr->write_index++;  // advance the write_index value, BUT:
    /* if the new value is > MAX_INDEX, meaning that it's out of bounds,
     * reset the value to 0, so that it wraps around and new writes 
     * go back to the start of the buffer (inner array)
     */
    if (cb_ptr->write_index > MAX_INDEX){
        cb_ptr->write_index=0;
    };
    return 0;   // success
};



void CircularBuffer_destroy_buffer(CircularBuffer *cb_ptr){
    /* call free on the CircularBuffer instance to free the heap memory */
    free(cb_ptr);
    cb_ptr = NULL;  // set cb_ptr to NULL so that subsequent access attempts cause an immediate memory access fault
};


