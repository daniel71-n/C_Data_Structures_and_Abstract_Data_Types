

/* ***************************************** Notes ******************************** *
 * ----------------------------------------------------------------------------------
 *
 * This is an implementation of a min-heap. That is, each parent is smaller than both
 * of its child nodes, going back to root. This means that the root of the heap
 * has the smallest value.
 *
 * Note that a heap is only a partially-ordered data structure. That is to say,
 * aside from the heap property where the child nodes are smaller than the parent
 * (max-heap) or greater than it (min-heap), there's no particular order among
 * sibling nodes. For a sorted data structure, a BST is the way to go. 
 *
 * In this implementation the heap is an EXPLICIT data structure, being built 
 * as a binary tree structure, using pointers to link the heap elements together.
 * This is in contrast to the heap as an IMPLICIT data structure: that is to say,
 * heaps can and are often stored implicitly in an array, especially for use in
 * the heapsort algorithm, where they thus entail no additional overhead beyond
 * the constant space complexity of the array itself. 
 *
 * ******************************************************************************** */



/* ******* Includes ****** */
#include <stdint.h>





/* ******* Typedefs ******* */
typedef struct minheap *Heap;
typedef struct minheapnode *HeapNode;




/* ******** Function prototypes ********* */
void Heap_init(Heap *heap_ref);
uint16_t Heap_count(Heap the_heap);
Heap Heap_insert(Heap the_heap, char the_value);
char Heap_delete(Heap *the_heap, char the_value);
char Heap_pop_root(Heap the_heap);
char Heap_peek_root(Heap *the_heap);
void Heap_maxheapify(Heap *the_heap);
void Heap_minheapify(Heap *the_heap);
void Heap_destroy(Heap **heap_ref);
void Heap_print(Heap the_heap);   // traverse the heap in order and print out the value of each node to stdout
void Heap_print_BF(Heap the_heap);   // traverse the heap in order and print out the value of each node to stdout

