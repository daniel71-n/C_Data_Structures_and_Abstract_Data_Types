
/* Implementation of a stack ADT (Abstract Data Type). 
 * A stack offers an interface for manipulating data in
 * a LIFO - last-in-first-out - manner, always removing 
 * ('popping', in stack terminology) the most recently added
 * item -- the item that was last 'pushed' onto the stack. 
*/

typedef struct stack Stack; 
typedef struct item StackItem;

struct stack{
    unsigned int count;     // number of items in the queue
    StackItem *top;
};
    
struct item{
    void *contents;     // a void pointer is used so that any type can be pointed to and thus enqueued;
    StackItem *previous;
};


/* 'object' is used very loosely below, of course, as C doesn't have 'objects' (it's not an OOP language) */
void Stack_init(Stack **stack_ptr);  // initialize a stack object*
void Stack_init_local(Stack *stack_ptr);  // initialize a stack object*
void Stack_destroy(Stack **stack_ptr);  // tear down the stack object by freeing all the malloc-ated memory
void Stack_push(Stack *stack_ptr, StackItem *item_ptr);  // add a new item to the stack 
StackItem *Stack_pop(Stack *stack_ptr);  // remove and return the last item on the stack
StackItem *Stack_peek(Stack *stack_ptr);  // return the last item on the stack, but without removing it
unsigned int Stack_count(Stack *stack_ptr); // return the number of items on the stack
Stack *Stack_upend(Stack *stack_ptr);      // upend a stack - turn it upside down, so that the items are in reverse order
StackItem *Stack_new_node(void *data);



