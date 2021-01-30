#include "queue.h"
#include <stdlib.h>

/* ********************************************************** */
/*                  Implementation Notes

    Stacks and queues are essentially the inverse of one another 
    in that the former always removes the oldest item  added, 
    while the latter removes the latest item. 
   
    It's worth noting that stacks and queues are not 
    data structures, but abstract data types. Conceptually, 
    stacks and queues are to be thought of as an interface rather 
    than in implementation terms. That is to say, both of them can 
    easily be implemented using either arrays or linked lists 
    at the implementation level. 
    Arrays arguably lend themselves better to stacks and queues that 
    hold a fixed number of items (since arrays are fixed in size, 
    unless they're dynamic arrays) whereas linked lists are 
    best used when the number of items is indefinite. 

    Whatever the underlying data structure, stacks and queues are 
    defined by the interface they provide, which is in terms of 
    the data flow: first-in-first-first-out for queues and 
    last-in-first-out for stacks.
*/


struct queue{
    unsigned int count;     // number of items in the queue
    QueueItem *head;
    QueueItem *tail;
};
    
struct item{
    void *contents;     // a void pointer is used so that any type can be pointed to and thus enqueued;
    QueueItem *next;
};



void Queue_init(Queue **queue_ptr){
    /* Initialize a Queue object. 

       Takes a Queue reference pointer and calls malloc to allocate
       heap space for a Queue struct. If successful, 
       the, the inner members are initialized to zero
       or NULL, as appropriate.
    */
    Queue *temp = malloc(sizeof(Queue));

    if (!temp){
        *queue_ptr = NULL;
        return;
    };

    *queue_ptr = temp; 
    (*queue_ptr)->count = 0;
    (*queue_ptr)->head = NULL;
    (*queue_ptr)->tail= NULL;
};




unsigned int Queue_count(Queue *queue_ptr){
    /* Return the number of items in the queue */
    return queue_ptr->count;
};




void Queue_destroy(Queue **queue_ptr){
    /* Call free on all the items in the queue,
       and then finally on the Queue pointer itself,
       before setting the pointer to NULL
    */
    QueueItem *current = (*queue_ptr)->head;
    QueueItem *next;

    while (current){
        next = current->next;
        free(current);
        current = next;
    };
    free(*queue_ptr);
    *queue_ptr = NULL;

};



void Queue_enqueue(Queue *queue_ptr, QueueItem *item_ptr){
    /* Add an item to the queue.
    
    Note that the second argument is a QueueItem pointer. 
    The QueueItem struct has a 'contents' field which is 
    a void pointer. This can be used to point to any data type,
    be it char, int, whatever. 
    This means that if you want to enqueue X, you will have to 
    declare a QueueItem and set it to point to X.

    For example: 

        int someint = 6;
        Queueitem item1; 
        item1.contents = &someint; 
        Queue_enqueue(&item1);


    Note that since items1.contents is a void pointer, 
    the consumer will have to know how to handle this 
    when returned by the Queue_dequeue subroutine.
    */
    if (!queue_ptr->count){
        queue_ptr->head = item_ptr, queue_ptr->tail = item_ptr;
        item_ptr->next = NULL;
    }else{
        QueueItem *temp;
        queue_ptr->tail->next = item_ptr;
        queue_ptr->tail = item_ptr; 
    };

    queue_ptr->count++;
};

QueueItem *Queue_dequeue(Queue *queue_ptr){
    /* Remove and return an item from the queue.
       This item is always the oldest item, according
       to the FIFO principle.

       If called on an empty queue, NULL is returned.
    */
    QueueItem *oldest_item = queue_ptr->head;
    if (!oldest_item){  // if there are no items return NULL
        return NULL;
    };
    queue_ptr->head = queue_ptr->head->next;    // if this was the only item in the queue, its 'next' will have been
                                                // set to NULL, so this takes care of setting head to NULL as well
    queue_ptr->count--;
    
    if (!queue_ptr->count){    // if the queue is empty, then the tail needs to be NULL, just as the head
        queue_ptr->tail = NULL;
    };
    return oldest_item; 
};



QueueItem *Queue_peek(Queue *queue_ptr){
    /* Return the oldest item in the queue
       but without removing it from the queue,
       so that this is the item that will be returned
       by Queue_dequeue, when called.

       Returns NULL if called on an empty queue.
    */
    return queue_ptr->head; 
};













