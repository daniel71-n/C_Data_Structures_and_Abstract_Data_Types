#ifndef CBNO_H
#define CBNO_H

/*----------------------------------------------Implementation notes-----------------------------------------
              ******************************************************************************
    This implementation of a circular buffer DOES NOT ALLOW OVERWRITES. 
    When the buffer has been filled and is thus out of room, no new write operations are possible, and the
    associated write function will return an error code. 
    Items will need to be read by the read function out of the buffer before any new writes can take place.

    
    -----------*** In more detail ***---------------
    The buffer is implemented using an inner array of size set by the BUFFER_SIZE symbolic constant. 
    Two variables, read_index and write_index track the position of the last read and write, respectively,
    in the array.
    A third variable, free_count, tracks the number of available positions in the array that can be written.


    Reading can never be done when free_count is equal to BUFFER_SIZE, and thus the buffer is empty. 
    Writing can never be done when free_count is 0, and thus the buffer is full. 

    Every time a read is done, read_index is incremented by one, so as to advance it in the array,
    and free_count is incremented by one as well.
    Similarly, every time a write is done, write_index is incremented by one, and free_count
    is decremented by one.
*/


/*-----------------------------------------------Includes------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>




/*-----------------------------------------------Symbolic constants------------------------------------------*/
#define BUFFER_SIZE 7
#define MAX_INDEX BUFFER_SIZE-1



/*--------------------------------------------Type Defs------------------------------------------*/
typedef struct{
    char inner_array[BUFFER_SIZE]; 
    unsigned int read_index; // used to track the current index associated with the read operation
    unsigned int write_index; // used to track the current index associated with the write operation
    unsigned int free_count; // tracks the number of free slots in the buffer (that is, in inner_array)
}CircularBuffer;


/*-----------------------------------------Function Prototypes------------------------------------*/
CircularBuffer *CircularBuffer_init(void);         // initialize a CircularBuffer instancae
char CircularBuffer_read(CircularBuffer *cb_ptr);  // read one item from the buffer
uint8_t CircularBuffer_write(CircularBuffer *cb_ptr, char char_to_write);  // write one item to the buffer
bool CircularBuffer_is_empty(CircularBuffer *cb_ptr);
bool CircularBuffer_is_full(CircularBuffer *cb_ptr);
void CircularBuffer_destroy_buffer(CircularBuffer *cb_ptr);  // free all the space that has been Malloc-ated.



#endif
