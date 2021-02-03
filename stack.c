#include "stack.h"
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
    This implementation of a stack doesn't have a fixed size, i.e. 
    there's no stack overflow scenario that's accounted for.
    This could easily be incorporated, however. For example, 
    by having the Stack_push() function check the count member 
    and stop pushing new items when its value has reached a defined limit.

    Stack 'underflow', is, on the other hand, accounted for, as Stack_pop() 
    will return a NULL pointer when called on an empty queue.

*/


void Stack_init(Stack **stack_ptr){
    /* Initialize a Stack object. 

       Takes a Stack reference pointer and calls malloc to allocate
       heap space for a Stack struct. If successful, 
       the inner members are initialized to zero
       or NULL, as appropriate.

       This function is called when the Stack is meant to be
       dynamic - that is to say, heap memory is allocated to 
       it.

       Example
       Stack *somestack;
       Stack_init(&somestack);  // allocates heap memory to *somestack

       Since dynamic memory is allocated, it's imperative that
       Stack_destroy() be called to tear down the stack and
       free all the memory when the stack is no longer used.


       The Stack_init_local() function (below) should be called instead when 
       the stack is meant to have local scope and thus automatic duration. 
        
    */
    Stack *temp = malloc(sizeof(Stack));

    if (!temp){
        *stack_ptr = NULL;
        return;
    };

    *stack_ptr = temp; 
    (*stack_ptr)->count = 0;
    (*stack_ptr)->top= NULL;
};



void Stack_init_local(Stack *stack_ptr){
    /* Initialize stack_ptr by initializing its values. 

       Malloc is not called - heap memory is not allocated. 
       This means the stack has local scope (defined in a function)
       and thus automatic duration: i.e. it will be automatically
       deallocated when it goes out of scope. 
       Therefore you don't need (you musn't) call Stack_destroy
       on a stack initialized with Stack_init_local().

       Calling free() (which Stack_destroy() does internally)
       on memory that wasn't allocated dynamically is UNDEFINED
       BEHAVIOR.

       If the stack being initialized is instead supposed to
       be heap-allocated, use Stack_init() instead.
    */
    stack_ptr->count = 0;
    stack_ptr->top = NULL;
};



unsigned int Stack_count(Stack *stack_ptr){
    /* Return the number of items on the stack*/
    return stack_ptr->count;
};


void Stack_destroy(Stack **stack_ptr){
    /* Call free on *stack_ptr, and set it to NULL. 
    // to do
    */
    free(*stack_ptr);
    *stack_ptr = NULL;

};



void Stack_push(Stack *stack_ptr, StackItem *item_ptr){
    /* Add an item to the stack.
    
    Note that the second argument is a StackItem pointer. 
    The StackItem struct has a 'contents' field which is 
    a void pointer. This can be used to point to any data type,
    be it char, int, whatever. 
    This means that if you want to push X onto the stack, you will
    have to declare a StackItem and set it to point to X.

    For example: 

        int someint = 6;
        StackItem item1; 
        item1.contents = &someint; 
        Stack_push(&item1);


    Note that since items1.contents is a void pointer, 
    the consumer will have to know how to handle this 
    when returned by the Stack_pop() subroutine.
    */
    item_ptr->previous = stack_ptr->top;
    stack_ptr->top= item_ptr;
    stack_ptr->count++;
};



StackItem *Stack_pop(Stack *stack_ptr){
    /* Remove and return an item from the stack.
       This item is always the most recently added item, 
       according to the FIFO principle.

       If called on an empty stack, NULL is returned.
    */
    if (!stack_ptr->count){
        return stack_ptr->top;
    };
    stack_ptr->count--;
    StackItem *item = stack_ptr->top;
    stack_ptr->top = stack_ptr->top->previous;
    return item;
};



StackItem *Stack_peek(Stack *stack_ptr){
    /* Return the top of the stack (most recently
       added item) but without removing it from 
       the stack, such that this is the item that will 
       be returned by Stack_pop(), when called.

       Returns NULL if called on an empty queue.
    */
    return stack_ptr->top; 
};




Stack *Stack_upend(Stack *stack_ptr){
    /* Upend a stack and return the new stack. 

       This is done by initializing a second stack first,
       with Stack_init_local().
       The items are popped off the first stack one by one
       and pushed onto the second one as this happens.

       The first stack is then pointed to the top of this second 
       stack. 
       The second stack has automatic duration- so it will go
       out of scope when the function exits. 
    */
    Stack newstack;     // local-scope, automatic-duration stack
    Stack_init_local(&newstack);
    
    StackItem *current = Stack_pop(stack_ptr);

    while (current){
        Stack_push(&newstack, current);
        current = Stack_pop(stack_ptr);
    };

    stack_ptr->top = newstack.top;
    stack_ptr->count = newstack.count;

    return stack_ptr;
};


StackItem *Stack_new_node(void *data){
    /* Dynamically allocate memory for a single node
       and return a pointer to it.

       The 'contents' member of the node, which is
       a void pointer, is pointer to the function 
       parameter.

       The caller should then manually call Stack_push()
       with the value returned by this function to actually
       push the node onto the stack.

       --------------------------------------
       Example

       int myint = 5;
       StackItem *node = Stack_new_node(&myint); 
       Stack_push(somestack_ptr, node);
    */
    StackItem *newnode = malloc(sizeof(StackItem));
    if (newnode){
        newnode->contents = data;
    };
    return newnode;
};







