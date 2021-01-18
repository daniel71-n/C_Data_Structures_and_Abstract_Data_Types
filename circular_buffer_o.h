
/*----------------------------------------------Implementation notes-----------------------------------------
              ******************************************************************************
    This implementation of a circular buffer ALLOWS OVERWRITES. 
    When the buffer has been filled and is thus out of room, the write operation is reset to the beginning
    of the buffer and starts overwriting the oldest items.
    
    
    -----------*** In more detail ***---------------
    The buffer is implemented using an inner array of size set by the BUFFER_SIZE symbolic constant. 
    Two variables, read_index and write_index track the position of the last read and write, respectively,
    in the array.

   
    Reading can never be done when read_index is equal to write_index, which essentially means there are
    no items to be read. For example, if there have been no writes, both read_index and write_index will
    be set to 0, in which case it's obvious there are no items for the read function to operate on. 
    
    At the same time, there's a second scenario: imagine 3 items have been written, and 2 have been read. 
    Write index will be 2, and read_index will be 1. If there are no reads for a period of time, and only
    writes, such that the buffer is filled and items start being overwritten, write_index will be reset to
    the beginning of the buffer, at which point we would have read_index 1, write_index 0.

    If another two write operations then take place on buffer[0] and buffer[1], setting write_index
    to 2 (in preparation for the next write operation), then another read can be done, and then 
    read_index wil be set to 2. 
    At this point read_index and write_index are equal, so no reads can be done anymore.
    The buffer isn't really empty, but buffer[2] hasn't been overwritten yet (like buffer[0]
    and buffer[1] have been) and so it contains an old value from the first write. 
    Buffer[1], which the read operation last read, contained the most recently written value, 
    while buffer[2] contains the oldest written value still present in the buffer. 
    Thus the disparity would be too great and it wouldn't make sense (depending on the use case, 
    of course) to do it. 

    
    What this means, is that: 
    - writes can be done without limit, with write_index being reset to the beginning of the buffer 
    when no room is left, and the oldest items start getting overwritten.
    - reads can only be done when read_index is either smaller than or greater than write_index,
    but never when it's equal.
    Note that the write index can be either in front (as when the buffer first starts being written to)
    of the read index, or behind it (i.e. when write_index has been set back to 0). It can go past the
    read index and then start over from the beginning as many times as necessary. The read index,
    on the other hand, can never itself go past the write_index. It's always because it's been
    passed BY the write index that it's in front of it, and not because it itself passed
    the write index.

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
/*-----------------------------------------------Symbolic constants------------------------------------------*/

