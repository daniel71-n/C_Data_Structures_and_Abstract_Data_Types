
/* *********************
 * Implementation Notes.
 * ********************
 */



/* ************ Abbreviations ************** */
/* Sll = singly linked list 
 */




/* ************ Type Defs and definitions  ************** */
struct ListNode{
    char data;
    struct ListNode *next_ptr;   // pointer to the next node in the list
};
struct ListHead{
    struct ListNode *firstNode_ptr; // the first node in the list (actual head)
    struct ListNode *lastNode_ptr;  // the last node in the list (tail)
    unsigned int list_length; // the number of nodes in the list 
};

typedef struct ListNode Node; 
typedef struct ListHead Head; 




/* ************ Includes ************** */
#include <stdbool.h>
#include <stdint.h>



/* ************ Function Prototypes ************** */
Head *SLL_init(void); // create and initialize a singly linked list
uint8_t SLL_append(Head *head_ptr, char the_value); // add a node storing the_value with to the tail end of the list a
uint8_t SLL_prepend(Head *head_ptr, char the_value); // add a node storing the_value to the head end of the list
uint8_t SLL_insert_index(Head *head_ptr, char the_value, unsigned int the_index); // insert the_value at index the_index
unsigned int SLL_get_length(Head *head_ptr); // return an integer denoting the number of nodes in the list
bool SLL_contains(Head *head_ptr, char the_value); // return true if the_value is in the list, false otherwise
char SLL_pop_head(Head *head_ptr); // get the value of the first node in the list and remove the node
char SLL_pop_tail(Head *head_ptr); // get the value of the last node in the list and remove the node
char SLL_pop_index(Head *head_ptr, unsigned int the_index); // get the value of the node at index the_index and remove the node
unsigned int SLL_occurence_count(Head *head_ptr, char the_value); // count how many times the_value appears in the list
void SLL_remove_duplicates(Head *head_ptr); // make sure there's a single instance of any one item in the list
void SLL_Ireverse_list(Head **head_ref); // reverse the list iteratively, in place
void SLL_Rreverse_list(Head **head_ref); // in-place recursive reversion 
void SLL_destroy_list(Head *head_ptr); // free all the memory associated with the linked list












