#include "queue.h"
#include <stdlib.h>

/* ********************************************************** */
/*                  Implementation Notes

    Stacks and queues are essentially the inverse of one another 
    in that the former always removes the oldest item  added, 
    while the latter removes the latest item. Both offer linear
    (sequential) access to the data, just like a linked list.
   
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

    --------------Overflow and underflow--------------------
    This implementation of a queue doesn't have a fixed size, i.e. 
    there's no queue overflow scenario that's accounted for.
    This could easily be incorporated, however. For example, 
    by having the Queue_enqueue() function check the count member 
    and stop enqueing new items when its value has reached a defined limit.

    Queue 'underflow', is, on the other hand, accounted for, as Queue_dequeue() 
    will return a NULL pointer when called on an empty queue.

*/


struct queue{
    unsigned int count;     // number of items in the queue
    QueueItem head;
    QueueItem tail;
};
    
struct queueitem{
    void *contents;     // a void pointer is used so that any type can be pointed to and thus enqueued;
    QueueItem next;
};







void Queue_init(Queue *queue_ptr){
    /* Initialize a Queue object. 

       Takes a Queue reference pointer and calls malloc to allocate
       heap space for a Queue struct. If successful, 
       the, the inner members are initialized to zero
       or NULL, as appropriate.
    */
    Queue temp = malloc(sizeof(struct queue));

    if (!temp){
        *queue_ptr = NULL;
        return;
    };

    *queue_ptr = temp; 
    (*queue_ptr)->count = 0;
    (*queue_ptr)->head = NULL;
    (*queue_ptr)->tail= NULL;
};




unsigned int Queue_count(Queue the_queue){
    /* Return the number of items in the queue */
    return the_queue->count;
};




void Queue_destroy(Queue *queue_ptr){
    /* Call free on *queue_ptr, and set it to NULL. 

       Note that the QueueItems in the queue aren't malloc'ed
       but declared manually outside the queue, and so 
       there's nothing else to free.
    */
    // nothing to free, there's no queue (queue is NULL)
    if (!(*queue_ptr)){
        return;
    }

    // there are still items on the queue -- free all of them
    if ((*queue_ptr)->head){
        QueueItem current = (*queue_ptr)->head;
        QueueItem temp;
        while (current){
            temp = current;
            current = temp->next;
            free(temp);
        };
    }

    // else there's a queue, but it's empty (Queue but no QueueItems). Deallocate that
    if (*queue_ptr){
        free(*queue_ptr);
        *queue_ptr = NULL;
    };
};




QueueItem Queue_make_item(void *the_value){
    QueueItem newitem = malloc(sizeof(struct queueitem));

    if (newitem){
        newitem->contents = the_value;
        newitem->next = NULL;
    }
    return newitem;
}


void Queue_enqueue(Queue the_queue, QueueItem the_item){
    /* Add an item to the queue.

       the_item needs to have been created with a call
       to Queue_make_item(). 
    */
    if (!the_queue->count){
        the_queue->head = the_item, the_queue->tail = the_item;
    }else{
        QueueItem *temp;
        the_queue->tail->next = the_item;
        the_queue->tail = the_item; 
    };

    the_queue->count++;
};



void *Queue_dequeue(Queue the_queue){
    /* Remove and return an item from the queue.
       This item is always the oldest item, according
       to the FIFO principle.

       If called on an empty queue, NULL is returned.

        NOTE
        what's returned is not a QueueItem, but its 'contents'
        field, which is a void pointer. This is the value that
        was passed to Queue_make_item(), the result of which
        was then appended to the queue with Queue_enqueue().

        In other words, Queue_dequeue() returns
        the_queue->head->contents, not the_queue->head.
        the_queue->head gets deallocated.
    */
    if (!the_queue->count){
        return the_queue->head;
    }
    void *item = the_queue->head->contents;
    QueueItem temp = the_queue->head;
    the_queue->head = the_queue->head->next;    // if this was the only item in the queue, its 'next' will have been
                                                // set to NULL, so this takes care of setting head to NULL as well
    free(temp);
    the_queue->count--;
    
    if (!the_queue->count){    // if the queue is empty, then the tail needs to be NULL, just as the head
        the_queue->tail = NULL;
    };
    return item; 
};



void *Queue_peek(Queue the_queue){
    /* Return the oldest item in the queue
       but without removing it from the queue,
       such that this is the item that will be returned
       by Queue_dequeue, when called.

       Returns NULL if called on an empty queue.
    */
    if (!the_queue){
        return NULL;
    }
    return the_queue->head->contents;
};













