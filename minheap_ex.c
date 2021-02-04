

/* *********************** IMPLEMENTATION NOTES ************************ */
/* --------------------------------------------------------------------- */
/* The heap, as implemented here, relies on the structure of a binary tree.
   That is to say, the nodes in the heap are linked together explicitly
   by means of pointers, rather than implicitly - when the heap is stored
   compactly, as an implicit data structure, inside an array.

   There are two additional constraints to be complied with for a binary tree
   to be a heap:
   - the shape property : the heap is an almost complete binary tree, with
   new nodes having to be added as far left as possible, until the last 
   level is filled, before starting a new level. In other words, insertions
   are done at the bottom, from left to right. 
   - the heap property: this is the relative ordering of the heap, where
   each node is either greater than its children (in a max heap) or smaller
   than them (in a min heap).

   The two 'properties' above, while straightforward to understand, pose
   perhaps surprisingly challenging issues to explicit, binary-tree -based
   heap implementations, as discussed below. 


   ------------------Explicit and Implicit Heaps--------------------------
   It's safe to say Heaps are usually implemented as implicit data structures
   inside an array, where the heap operations manipulate the array indexes
   mathematically, to modify the array in place. 
   This is as opposed to the explicit implementation of the heap, where use
   of pointers is involved in a binary-tree structure, which is comparatively
   only very rarely the approach taken, in practice.

   While the implicit implementation does have its own issues - mainly 
   having to grow the array when new items are inserted into the heap (
   which is a costly operation) - the reason heaps are all but always
   implemented implicitly rather than explicitly becomes apparent
   when trying to take the latter approach. 


   --------------Explicit Heap Implementation Challenges-----------------

   The issue of having to resize the array aside, array-based heaps afford
   constant time insertion of new nodes at the end of the array. 
   This is the fundamental issue, really. Since array manipulation
   boils down to indexes, and since the relationship between the nodes
   in an array-stored heap can be determined easily in terms of indexes,
   one can easily determine the position of the last heap node. 
   This is of the utmost importance, because various fundamental operations
   are centered around it: removing the root replaces it with the last node,
   which is then 'sifted down'; inserting a new node is done at the free 
   slot following the last-added node, and then the new node is 'sifted up'.

   The last sentence encapsulates both, on the one hand, the ease and 
   convenience of implementing a heap as an array-based structure, and
   on the other hand, the difficulties and pains of doing it in a
   binary-tree context.

   Knowing the position of the last-added item, the next open slot is
   the one immediately following it. 
   In an array, the former is easily determined, and the latter is 
   simply x+1, where x is the index of the last-added item. 
   In a tree, it's comparatively more difficult to find the last-added
   node, and arguably even more so finding the open slot after it,
   where the new node is supposed to be inserted. 

   The 'shape property' of the heap thus proves difficult to maintain. 
   
   There are a few things that could be done, ranging in complexity
   and overhead. 

   Probably the most obvious solution would be to traverse the tree
   breadth-first. But this might mean unacceptable or prohibitive 
   overhead in the case of a very large heap: building a queue data
   structure where, on by one, all the nodes are enqueued, then
   examining each node until the free slot is found. This free slot
   will invariably be found only at the very end of the heap,
   which means you ALWAYS have to carry out a complete traversal
   going over every single node. Knowing this, you could simply
   enqueue all the nodes, but jump ahead to some calculated point deep
   into the queue, where you know the last level of the tree is
   supposed to start. That notwithstanding, this is an expensive 
   option.

   You could try and keep a pointer tracking the last-added node, 
   and then starting from there, find the next free slot. 
   But this pointer adds space overhead, has to be updated every
   time an insertion occurs, and solves only half of the problem:
   you still have to find the next free slot from there, which 
   is easier said than done. 
   A related, more full-fledged, alternative is to use a threaded
   binary tree where the leaves have pointers to their successor/
   predecessor. This would again though add substantial overhead
   and complexity as the threaded-tree apparatus has to be 
   maintained.

   After figuring these two out, and are in a position where you
   can relatively efficiently find the slot where a new node needs 
   to go, you then have to 'sift up' or 'percolate up' this node, 
   so as to find its correct position in the heap and abide by the 
   'heap property'.
   That is to say, a node is swapped with its parent if its value
   is larger (max-heap) or smaller (min-heap).

   Each parent has a pointer to each one of its children, but
   this relationship is one-way only: a child has no pointer to
   its parent. 
   To move a node UP the heap then, you need the parent of said node.
   You could write a routine tracking the node immediately preceding
   the last node, and then you could compare and potentially swap
   the newly-inserted node with its parent. 
   But you need not only its immediate parent, but all the parent nodes
   on the way to root, potentially. 
   This means that this routine would have to be called 
   repeatedly (a number of times proportional to the height of
   the tree) each time the node has moved up one position, in order
   to find its next parent up the tree. Needless to say, this would
   make sift-up a very costly operation.
   A better solution, but which would add space overhead, would be
   to have each node also have a pointer to its parent, making
   movement up and down the tree trivial.

   
   ---------- Specific Implementation -------------------------

   No matter the route taken, there's going to be some overhead
   involved, and normally some time-space tradeoffs. 

   This specific implementation doesn't add extra 'parent'
   pointers to the nodes, nor does it keep a 'last' pointer,
   though these would both (especially the former) be valid
   options, depending on the implementation and its goals.

   Instead it relies on the idea that, like in an array, the
   next free slot can be determined mathematically, and the
   same applies to the position of the parent of any node. 

   Given a heap of 7 items, the next slot, will of course, be
   slot 7+1, just like in an array. 
   This answers part of the question. The part it doesn't answer
   is how you find slot 7+1.
   This can be easily determined as well, based on the observation
   that the position of each parent is equal to the position of its
   child divided by two, disregarding any remainder. 

   (Node positions are numbered breadth-first, from left to right).
                       1
                    2     3
                   4 5   6 7
    The parent of node seven is 7/2 = 3 (remainder of 1, dropped). 

    Building on this, the WHOLE PATH from this node to the root can
    be traced, by combining these observations.
    1. Each new insertion happens at n+1, where n is the total number
    of nodes in the heap. 
    So given the heap above where n=7, a new node '8' will be inserted 
    at n+1 = 8. 
    2. The parent of '8' will be at 8 / 2 = 4, and the parent of 4 will
    be at 4/2 = 2, and the parent of 2 will be at 2/2 = 1, which is root. 
    
    Therefore the path from root to '8' is 1-2-4-8. 
    As can be seen above, this is all the way down the tree, hugging the left
    side. All the divisions above left no remainder. This is important: if the 
    division leaves no remainder, then the number is even, and starting at
    root, the path goes to the left. Conversely, if there's a remainder of 1, 
    the path is to the right.

    So the modulo operator will come in handy here, for tracing the path
    to the node, starting off at root.
    Alternatively, you can use bitwise shifting. It will probably not lead
    to better performance though, as the compiler is in all likelihood already
    using this for the modulo operation anyway.

    Given the same example above, you can trace the path from root to node '8'
    as follows. 
        8 in binary: 1000
    You can use a mask of 0001 and AND the two. If the bitwise result is a 1,
    you go right, while if it's a 0, you go left. 
    Then shift 8 left by one (8 >>= 1).
    Notice that when you've reached the slot, '8' still has a bit (the left-most, 1)
    left in it. This isn't necessary, since you've already reached the end
    of the path. So the left-most bit is to be disregarded.

    ***********************************************
    This specific implementation uses the approach above to find the next free 
    slot for insertions, and then trace the path from root to where this new
    node is to be inserted. This path is needed twice, in opposite directions: 
    coming down from root to find the slot to insert the new node, and going back 
    up the path during sift-up, after the node is inserted. 

    To avoid computing the path twice, a stack data structure is used. 
    The top of the stack will be the slot itself, the top-1 will be
    its parent, top-2 the next parent up the tree, and so on.
    
    compute the path with bitwise shift:
    push0, push0, push1, for example.
    
    stack1                      
    1  pop1 - go right 
    0  pop0 - go left  
    0  pop0 - go left 

    Going back up:
    stack 2:
    0
    0
    1
    pop0, pop0,pop1

    In the implementation below, only one stack is used.
    It looks like stack1. The top of the stack is discarded,
    and top-1 (which is the parent of the free slot) is only
    peeked at, not popped off the stack. 
    Then the stack is reversed (turned upside down, upended),
    and fed to the sift-up routine as the path back up the tree
    to root.
    
    The above is just very simplified pseudocode, to give an idea. 
    In the actual implementation 1 means a pointer to the right child,
    and 0 means a pointer to the left child. That means the each child
    along the path has a pointer to the parent above it by popping this 
    pointer off the stack. 
    The path from root to the next free slot is therefore created and 
    kept on the stack, as a series of pointers. 

    To avoid having to count the number of nodes in the tree for every 
    insertion (necessary for finding the next free slot), there are two
    separate structs being used: one for heap nodes, which has a key and 
    right and left pointers, and a separate struct for the heap itself,
    having a pointer to the root of the heap, and a count member, storing
    the number of nodes and thus making determining the size of the tree
    a matter of looking it up rather than computing it every time.

    ----------------------CONCLUSION----------------------------------

    Implementing a heap explicitly as a binary tree is something
    that's very rarely done in practice, and there are good reasons
    for that. It does have the upside of being able to better
    visualize the heap structure (which is why heaps are often shown
    in tree form in diagrams) as well as being able to grow the 
    heap without having to resize an array. 

    On the flip side, the fundamental heap operations are more complicated
    and awkward to reason about in a tree-based heap, specifically
    finding the next free slot for new node insertions and finding
    the next parent up the tree for the sift-up routine.

    *********************************************************************   
*/


#include "minheap_ex.h"
#include <stdint.h>
#include <stdlib.h>
#include "stack.h"
#include "queue.h"
#include <stdio.h>

// structure of a Min-Heap node
struct minheap{
    HeapNode root;
    uint16_t count;
};

struct minheapnode{
    char data;
    HeapNode left;
    HeapNode right;
};



uint16_t Heap_count(Heap the_heap){
    return the_heap->count;
};




void Heap_init(Heap *heap_ref){
    /* Allocate memory for a Heap and initialize it with
       the correct values
    */
    Heap newheap = malloc(sizeof(struct minheap));
    
    if (newheap){
        newheap->root = NULL;
        newheap->count = 0;
    }

    (*heap_ref) = newheap;
};

static Stack Heap_find_path_to_node_P(Heap the_heap, int16_t node_position){
    Stack way_down;   
    Stack_init(&way_down);
    // trace the path from the root to the free slot by pushing each node along the path
    // on the stack. They can then be popped one by one to get from
    // the root to the free slot


    // push root onto the stack first 
    StackItem node = Stack_make_item(the_heap->root); 
    // the_heap->root is stored as a void pointer in 'node', which is returned by
    // Stack_pop() and Stack_peek()
    Stack_push(way_down, node);     // push the node onto the way_down stack

    
    uint16_t temp;   // used to test the result of the bit shift in the while loop below
    HeapNode current = (HeapNode)Stack_peek(way_down); 
    
    // turn the bits in node_position around
    {
        uint16_t bits_flipped = 1;
        while (node_position){
            temp = node_position & 1;

            if(temp){
                bits_flipped <<= 1;
                bits_flipped += 1;
            }else{
                bits_flipped <<= 1;
            }
            node_position >>= 1;
        }
        bits_flipped>>=1;
        node_position = bits_flipped;
    }


    while (node_position > 1){  // > than 1 because the leftmost bit in heap_size needs to be disregarded
        temp = node_position & 1;    // return the bitwise not of the AND operation between node_position and 1
        /* the reason we take the bitwise not instead of the actual result is the in which
         * the path from root to the insert spot is put onto the stack. The first AND gets
         * us the immediate parent of node_position - NOT the first child of root that we
         * should go down (left or right). That is, each bitwise shift here goes UP the
         * tree, not DOWN the tree -- it goes from node_position to root, not from root to
         * node_position. We need the latter, not the former. But if the route we get is
         * right-left-right-left (1-0-1-0), where we start at node_position and end at
         * root, then inverting this to left-right-left-right (0-1-0-1) gives us the path
         * from root to node_position.
                Uninverted: 
                root
            left    right
        left slot

        The starts from the point of view of slot: 
        - right (it's the right child of its parent)
        - left (its parent is the left child of its parent)
        If we then were to traverse the tree down that way -- go right, go left, we would be inserting
        the new node as the left child of of the right child of root! That's because the going down is
        from the perspective of the parent, not the child. 

        If we INVERT that, we change the 'perspective' of the child node to that of the parent:
         right, left becomes left, right.
        Go 1 to the left, then one to the right. Sure enough, that gets us to 'slot', which is the
        spot we needed to get to.
         */  
        if (temp == 0){
            node = Stack_make_item(current->left);
            current = current->left;
        }else{
            node = Stack_make_item(current->right);
            current = current->right;
        };
        node_position >>= 1;
        Stack_push(way_down, node);
    };
    return way_down;    
};



static void Heap_sift_up_P(Stack path_to_root, HeapNode current){
    HeapNode parent = (HeapNode)Stack_pop(path_to_root); // Stack_pop() returns a void pointer to the stack top
    while (1){
        if (parent){

            if (current->data < parent->data){
                char temp = parent->data;
                parent->data = current->data;
                current->data = temp;

                current = parent;
                parent = (HeapNode)Stack_pop(path_to_root);
            }else{
                break;
            }

        }
        else{
            break;
        }
   }

};


static HeapNode Heap_make_new_node_P(char the_value){
    HeapNode newnode = malloc(sizeof(struct minheapnode));
    if (newnode){
        newnode->data = the_value;
        newnode->left = newnode->right = NULL;
    }
    return newnode;
}

Heap Heap_insert(Heap the_heap, char the_value){
    /* Insert a new node into the_heap such that the shape property 
       of the heap is respected. 
       The new node's key will be the_value.
       The new heap is then returned (Heap is actually a typdef
       for a pointer to a struct heap).

       It calls Heap_find_path_to_node_P() to map a path from
       root down to the free slot where the insertion is to occur.

       The path is saved on a Stack data structure as a series of pointers 
       -- a pointer for each node along the path, starting with root. 

       A second stack is created, for use as explained below. 
       After the path from root to the free slot is computed and
       put on the stack, moving down the tree is done by popping
       the top of the stack and following the returned pointer
       until the parent of the free slot is reached. 
       The insertion is done for the child pointer that's NULL,
       going from the left one to the right one. 

       As each pointer is popped off the stack, it's pushed onto
       the second stack mentioned above. This essentially reverses
       the array. The purpose of this is to remember the way back up
       from the newly inserted node to root - which is needed in
       the sift-up routine, and which is the inverse of the path
       down from root. 

       The Heap_sift_up_P() function is then called with this second
       stack as an argument, to find the correct position of the node
       in the tree, such that the heap property is maintained or
       restored.
    */
   
    if (!the_heap){     // heap not initialized properly
        return NULL;
    }

    if (the_heap->count == 0){      // heap empty - no root. 
        HeapNode newnode = Heap_make_new_node_P(the_value);
        the_heap->root = newnode;
        the_heap->count++;
        return the_heap;
    }
    // declare and initialize a Stack type
    Stack path_to_node; 
    Stack_init(&path_to_node);
    // compute the path from root to node (the next insertion spot)
    path_to_node = Heap_find_path_to_node_P(the_heap, the_heap->count+1);  
    // the new insertion spot is always n+1, where n is the number of nodes in the heap

    // pop off the top - not needed. This is the actual current node - which is NULL. 
    // we need the parent instead
    Stack_pop(path_to_node); 
    HeapNode parent = (HeapNode)Stack_peek(path_to_node);   // Stack_peek() returns a void pointer to the stack top
    
    // create new heap node
    HeapNode newnode = Heap_make_new_node_P(the_value);

    if (newnode){
        if (parent->left == NULL){
            parent->left = newnode;
        }
        else if (parent->right == NULL){
            parent->right = newnode;
        }

        // sift up the new node

        // flip the stack upside down
         // path_to_node = Stack_upend(path_to_node);   // path_to_node is now the path from node to root
        Heap_sift_up_P(path_to_node, newnode);
        
        // the stack is no longer needed : deallocate it
        Stack_destroy(&path_to_node);
    };
    the_heap->count++;
    return the_heap;
};
    

static void Heap_print_P(HeapNode root){
    if (!root){
        return;
    }
    Heap_print_P(root->left);
    printf("%c\t", root->data);
    Heap_print_P(root->right);
};


void Heap_print(Heap the_heap){
    if (!the_heap){
        return;
    }
    Heap_print_P(the_heap->root);    
}

void Heap_print_BF(Heap the_heap){
    Queue q;
    Queue_init(&q);
    
    QueueItem item = Queue_make_item(the_heap->root);
    Queue_enqueue(q, item);
    HeapNode node = Queue_dequeue(q);
    QueueItem new;
    while (node){
        printf("%c-", node->data);
        new = Queue_make_item(node->left);
        Queue_enqueue(q, new);
        new = Queue_make_item(node->right);
        Queue_enqueue(q, new);

        node = Queue_dequeue(q);
    };
    
    Queue_destroy(&q);

    };
    




static void Heap_sift_down_P(HeapNode the_node){

    // sift down the value to the correct spot to maintain the heap property
    uint8_t keep_sifting = 1;

    while (keep_sifting){
        // trying to access the -> left or ->right field of a NULL Heap will crash the
        // system. So that needs to be checked against first

        // case 1: both kids are not NULL
        if (the_node->left && the_node->right){
            HeapNode smaller= (the_node->left->data < the_node->right->data) ? the_node->left : the_node->right; 

            if (smaller->data < the_node->data){
                char temp = smaller->data;
                smaller->data = the_node->data;
                the_node->data = temp;
                the_node = smaller;
            }
            else{
                break;
            }
        }

        // case 2: left child is not NULL
        else if (the_node->left){
            HeapNode smaller = (the_node->left->data < the_node->data) ? the_node->left : NULL; 

            if (smaller){
                char temp = smaller->data;
                smaller->data = the_node->data;
                the_node->data = temp;
                the_node = smaller;
            }
            else{
                break;
            }
        }
        
        // case 3: right child is not NULL
        else if (the_node->right){
            HeapNode smaller = (the_node->right->data < the_node->data) ? the_node->right: NULL; 

            if (smaller){
                char temp = smaller->data;
                smaller->data = the_node->data;
                the_node->data = temp;
                the_node = smaller;
            }
            else{
                break;
            }
        }
        else{
            break;
        }
    }
};
        



char Heap_pop_root(Heap the_heap){
    char return_val = the_heap->root->data;

    if (the_heap->count == 1){
        free(the_heap->root);
        the_heap->count--;
        return return_val;
    };

    Stack path_to_last_node = Heap_find_path_to_node_P(the_heap, the_heap->count);

    {
    // replace root with the last node
    HeapNode last_node = Stack_pop(path_to_last_node);
    the_heap->root->data = last_node->data;
    HeapNode parent = Stack_pop(path_to_last_node);
    if (parent->left == last_node){
        parent->left = NULL;
    }
    else{
        parent->right = NULL;
    }
    free(last_node);
    } 

    Stack_destroy(&path_to_last_node);
    the_heap->count--;

    Heap_sift_down_P(the_heap->root);
    return return_val;
};







/*
void Heap_insert(Heap *the_heap, char the_value);
char Heap_delete(Heap *the_heap, char the_value);
char Heap_peek_root(Heap *the_heap);
void Heap_maxheapify(Heap *the_heap);
void Heap_minheapify(Heap *the_heap);
void Heap_destroy(Heap **heap_ref);
*/

