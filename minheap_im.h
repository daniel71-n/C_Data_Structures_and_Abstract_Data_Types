#include <stdint.h>




/* *********************************************************************** */
/* ------------------ Implicit implementation of a min heap -------------- */
/* *********************************************************************** */

typedef struct min_heap_implicit *Heap;


void Heap_init(Heap *heap_ref, int32_t initial_size);
void Heap_insert(Heap the_heap, char the_value);
char Heap_pop(Heap the_heap);
void Heap_destroy(Heap *heap_ref);


















