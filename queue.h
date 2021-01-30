
/* Implementation of a queue ADT (Abstract Data Type). 
 * A queue organizes its data in a FIFO -- First-in-first-out - manner, 
 * always removing ('dequeing', in queue terminology) 
 * the oldest item (the item that was 'enqued' first). 
*/

struct queue;
struct item;
typedef struct queue Queue; 
typedef struct item QueueItem;

void Queue_init(Queue **queue_ptr);  // initialize a queue object
void Queue_destroy(Queue **queue_ptr);  // tear down the queue object by freeing all the malloc-ated memory
void Queue_enqueue(Queue *queue_ptr, QueueItem *item_ptr);  // add a new item to the queue
QueueItem *Queue_dequeue(Queue *queue_ptr);  // remove and return the oldest (i.e. next)item in the queue
QueueItem *Queue_peek(Queue *queue_ptr);  // return the oldest item in the list, but without removing it
unsigned int Queue_count(Queue *queue_ptr); // return the number of items in the queue
