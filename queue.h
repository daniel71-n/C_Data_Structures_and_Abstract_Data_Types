
/* Implementation of a queue ADT (Abstract Data Type). 
 * A queue organizes its data in a FIFO -- First-in-first-out - manner, 
 * always removing ('dequeing', in queue terminology) 
 * the oldest item (the item that was 'enqued' first). 
*/

struct queue;
struct queueitem;
typedef struct queue *Queue; 
typedef struct queueitem *QueueItem;

void Queue_init(Queue *queue_ptr);  // initialize a queue object

void Queue_destroy(Queue *queue_ptr);  // tear down the queue object by freeing all the malloc-ated memory
QueueItem Queue_make_item(void *the_value);
void Queue_enqueue(Queue the_queue, QueueItem the_item);  // add a new item to the queue
void *Queue_dequeue(Queue the_queue);  // remove and return the oldest (i.e. next)item in the queue
void *Queue_peek(Queue the_queue);  // return the oldest item in the list, but without removing it
unsigned int Queue_count(Queue the_queue); // return the number of items in the queue

