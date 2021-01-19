#include "singly_linked_list.h"
#include <stdlib.h> // to be able to use NULL
#include <stdio.h>

Head *SLL_init(void){ // create and initialize a singly linked list
    Head *head_ptr = NULL; 
    if ((head_ptr = malloc(sizeof(Head)))){   // if malloc was successful, initialize the head struct instance as follows
        head_ptr->firstNode_ptr = NULL;
        head_ptr->lastNode_ptr = NULL; 
        head_ptr->list_length = 0; 
    };
    return head_ptr;
};


uint8_t SLL_prepend(Head *head_ptr, char the_value){ // add a node storing the_value to the head end of the list
    /*return 1 if failed, 0 if successful */
    uint8_t error_code = 1;
    Node *the_new_node = malloc(sizeof(Node)); 
    if (the_new_node){  // only do this if the call to malloc was successful
        printf("the allocation for node was successful \n");
        //initialize the variables inside the_new_node
        the_new_node->data = the_value;
        the_new_node->next_ptr = head_ptr->firstNode_ptr; 
        //point head_ptr's first_node as well as last_node (if it's the only node so far) to it
        // and increment the length
        head_ptr->firstNode_ptr = the_new_node;
        if(!head_ptr->list_length){     // if the list length is 0, i.e. 
            head_ptr->lastNode_ptr = the_new_node;      // if this is the first (and thus only) none in the list, then it's also the last node
        };
        head_ptr->list_length++;
        // set error_code to 0, as everything has worked, and return it
        error_code=0;
        return error_code;
    };
    printf("prepend failed\n");
    return error_code; // the function will only get here if the if conditional above is false, meaning malloc failed
};

unsigned int SLL_get_length(Head *head_ptr){ // return an integer denoting the number of nodes in the list
    return head_ptr->list_length;
};


uint8_t SLL_append(Head *head_ptr, char the_value){ // add a node storing the_value to the head end of the list
    /*return 1 if failed, 0 if successful */
    uint8_t error_code = 1;
    Node *the_new_node = malloc(sizeof(Node)); 
    if (the_new_node){  // only do this if the call to malloc was successful
        //initialize the variables inside the_new_node
        the_new_node->data = the_value;
        the_new_node->next_ptr = NULL; 
        //point head_ptr's last_node to it
        // and increment the length
        head_ptr->lastNode_ptr->next_ptr = the_new_node; // make the current tail point to the new tail
        head_ptr->lastNode_ptr = the_new_node;
        head_ptr->list_length++;
        // set error_code to 0, as everything has worked, and return it
        error_code=0;
        return error_code;
    };
    return error_code; // the function will only get here if the if conditional above is false, meaning malloc failed
};



bool SLL_contains(Head *head_ptr, char the_value){
    /* Iterate through the list looking for the_value.
       If found, return true. Otherwise, return false
    */
    bool return_value = false; 
    if (!head_ptr->list_length){    // if the list is empty, return false right away
        return return_value;
    };
    Node *current_node = head_ptr->firstNode_ptr;
    do{  // using a do while loop allows executing the body once at least - i.e. in the case where the list has only one item
        if (current_node->data == the_value){
            return_value = true;
            return return_value;
           };
        current_node=current_node->next_ptr; // advance the loop. current node might now be NULL if its 'next_ptr' was NULL;
    }while(current_node); // while current_node != NULL
    
    return return_value;
};



static Node *SLL_find_index_P(Head *head_ptr, unsigned int the_index){      // not declared in the header file. PRIVATE. file scope.
   /* Return a pointer to the node before index (i.e. the node at index the_index-1).
      Knowing the node pointing TO the node currently at index the_index, we can easily
      insert a new node after it or pop the one it currently points to.

      This is a private function meant to be called from other functions such as 
      SLL_insert_index and SLL_pop_index.

      Return NULL if unsuccessful, e.g. the list is empty.
   */
    Node *node_before_index = NULL; // this is the node whose next_ptr points to the item at index the_index
    Node *current_node = head_ptr->firstNode_ptr; // node_before_index points to the node before current_node
    unsigned int current_index = 0;
    
    if (!(the_index < head_ptr->list_length)){
        return node_before_index;
    };
    
    node_before_index = current_node;
    while(current_node){
        if (current_index == the_index){
            return node_before_index; // when the condition above becomes true, node_before_index will be equal to the value of the_index in the
                                      // previous iteration of the loop
        };
        current_index++;
        node_before_index = current_node; 
        current_node = current_node->next_ptr;
    };
    return node_before_index;
};

uint8_t SLL_insert_index(Head *head_ptr, char the_value, unsigned int the_index){
    unsigned int return_value = 1; // error
    
    Node *node_before_index = SLL_find_index_P(head_ptr, the_index);
    if (!node_before_index){  // return error code 1 if node_before_index is NULL
        return return_value;
    };
    Node *the_new_node = malloc(sizeof(Node));
    if (!the_new_node){
        return return_value;
    };
    if (head_ptr->firstNode_ptr == node_before_index){
        // if node_before_index is actually the first and currently only node in the list
        // make the node_before_index the new head;
        the_new_node->next_ptr = node_before_index;
        head_ptr->firstNode_ptr = the_new_node;
        return return_value = 0;
    };
    the_new_node->next_ptr = node_before_index->next_ptr;
    node_before_index->next_ptr = the_new_node;
    head_ptr->list_length++;
    return return_value = 0;    // success
};


unsigned int SLL_occurence_count(Head *head_ptr, char the_value){ // count how many times the_value appears in the list
    /*  If the list is empty, return 0.
        If not, iterate through the list by making current_node equal to current_node->next.
        If current_node is null, stop. Otherwise, compare current_node->data with the_value,
        and increment the counter if they're the same.
    */
    unsigned int counter = 0;
    if (head_ptr->list_length == 0){
        return counter;
    };
    Node *current_node = head_ptr->firstNode_ptr;
    while (current_node){
        if (current_node->data == the_value){
            counter++;
        };
        current_node = current_node->next_ptr;
    };
    return counter;
};


char SLL_pop_head(Head *head_ptr){ 
    /* Return the value of the first node in the list, if successful,
       or Nul ('\0'), if unsuccessful, for whatever reason.
       The node is removed from the list and the list length is decremented.
    */
    char return_value = '\0';   // return Nul if unsuccessful
   
    if (head_ptr->list_length){     // if the list isn't empty
        return_value = head_ptr->firstNode_ptr->data;

        Node * temp_node_ptr = head_ptr->firstNode_ptr; // used to call free on the node
        head_ptr->firstNode_ptr = head_ptr->firstNode_ptr->next_ptr;    // remove the head
        free(temp_node_ptr);    // avoid dangling pointer
        head_ptr->list_length--;
            
        if (!head_ptr->list_length){    // if the list length is now 0, make the tail a NULL pointer 
            head_ptr->lastNode_ptr = NULL;
        };
        return return_value;
    };
    return return_value;
};


char SLL_pop_tail(Head *head_ptr){ 
    /* Return the value of the last node in the list, if successful,
       or Nul ('\0'), if unsuccessful, for whatever reason.
       The node is removed from the list and the list length is decremented.

       Popping the item at the tail end is more expensive than popping the one at the head end,
       because it involves iterating through the list and finding the node just BEFORE
       the current tail, and pointing head_ptr->last_node_ptr to it. 
       Popping at the head end doesn't present this problem and added overhead,
       since the first node is pointed to by head_ptr->first_node_ptr, and 
       the node after is pointed to by head_ptr->first_node_ptr->next_ptr,
       so no iteration has to take place in this case.
       This is an inherent weakness of singly linked lists, as opposed to doubly-linked lists.
    */
    char return_value = '\0';   // return Nul if unsuccessful

    if (head_ptr->list_length){     // if the list isn't empty
        return_value = head_ptr->lastNode_ptr->data; 

        // get the node before the current tail. This will have to ultimately be made the new tail
        Node *node_before_tail = head_ptr->firstNode_ptr;   // start at node 1
        // case 1: if node_before_tail->next_ptr is NULL, the while will crash.
        // it means this is the only node. It's a special case. 
        if (head_ptr->list_length == 1){    // if this is the only node
            return_value = head_ptr->lastNode_ptr->data;

            free(head_ptr->lastNode_ptr);
            
            head_ptr->firstNode_ptr= NULL; 
            head_ptr->lastNode_ptr = NULL;
            head_ptr->list_length--;

            return return_value;
        };
        while (node_before_tail->next_ptr->next_ptr != NULL){
            node_before_tail = node_before_tail->next_ptr;
        };  // the loop will stop at the next-to-last node, since that's when the while condition becomes false
            // which is exactly what we need. 
            // this also covers the case where there are only two nodes: the head and the
            // tail. Since the head is the one node before the tail, when the tail is
            // popped, the head will become the new tail as well (i.e. both first_node_ptr
            // and last_node_ptr will point to it);

        Node * temp_node_ptr = head_ptr->lastNode_ptr; // used to call free on the node
        head_ptr->lastNode_ptr = node_before_tail;    // remove the old tail 
        node_before_tail->next_ptr = NULL;      // the node before tail is now the tail and its next_ptr should be set to NULL
        free(temp_node_ptr);    // avoid dangling pointer
        head_ptr->list_length--;
            
     };
    return return_value;
};        
        
char SLL_pop_index(Head *head_ptr, unsigned int the_index){
    /* Get the value at index the_index. Return that, if successful, or Nul if not.
       If the_index is out of bounds (i.e. > head_ptr->list_length), Nul is returned.
    */
    char return_value = '\0';   // return Nul if unsuccessful

    if (head_ptr->list_length){     // if the list isn't empty
        return_value = head_ptr->lastNode_ptr->data; 

        // get the node before the current tail. This will have to ultimately be made the new tail
        Node *node_before_tail = head_ptr->firstNode_ptr;   // start at node 1
        while (node_before_tail->next_ptr != NULL){
            node_before_tail = node_before_tail->next_ptr;
        };  // the loop will stop at the next-to-last node, since that's when the while condition becomes false
            // which is exactly what we need. 
            // this also covers the case where there are only two nodes: the head and the
            // tail. Since the head is the one node before the tail, when the tail is
            // popped, the head will become the new tail as well (i.e. both first_node_ptr
            // and last_node_ptr will point to it);

        Node * temp_node_ptr = head_ptr->lastNode_ptr; // used to call free on the node
        head_ptr->lastNode_ptr = node_before_tail;    // remove the old tail 
        free(temp_node_ptr);    // avoid dangling pointer
        head_ptr->list_length--;
            
        if (!head_ptr->list_length){    // if the list length is now 0, make the tail a NULL pointer 
            head_ptr->lastNode_ptr = NULL;
        };
        return return_value;
    };
    return return_value;
}



void SLL_Ireverse_list(Head **head_ref){ 
    /* Iteratively reverse the list in place. 
       The argument is a pointer to a pointer, i.e. a reference pointer.

    head_ref is a pointer to Head pointer. This allows changing what the head_ref
    argument points to back in caller space 
    */
    Node *newhead;      // this will eventually be what is now the current tail
    Node *tail = NULL;  // start building a list from here, on to the left - i.e. tail to head
    Node *current = (*head_ref)->firstNode_ptr;     // tmp pointer used to advance through the list
    (*head_ref)->lastNode_ptr = (*head_ref)->firstNode_ptr;     // firstNode_ptr will end up being the tail of the reversed list,
                                                                // pointed to by lastNode_ptr
    while (current->next_ptr){   // this will stop at the last node, so that last case needs to be handled below
        newhead = current->next_ptr;  // used to remember the next node
        current->next_ptr = tail;   // essentially, make each node to the right point 
        tail = current;             // backward to the previous node 

        current = newhead;  // assign current the next node remember above; this advances the loop
    };
    // as mentioned, case wherecurrent->next_ptr is NULL (i.e. the last node) isn't
    // covered by the while loop, so needs to be handled here
    newhead->next_ptr = tail;       // newhead already points to tail from the last iteration of the while,
                                    // since it always points to the next node ahead of the current one
    (*head_ref)->firstNode_ptr = newhead;   // the reversal is complete
};





void SLL_destroy_list(Head *head_ptr){ // free all the memory associated with the linked list
    Node *current_node= head_ptr->firstNode_ptr;
    free(head_ptr);  // free the head struct
    Node *tmp_ptr; 
    if (!current_node){
        return;     // if the node is NULL it won't have a next_ptr (and will thus produce a segfault), and there's nothing to deallocate
    };
    while(current_node->next_ptr != NULL){
        tmp_ptr = current_node->next_ptr;
        free(current_node);
        current_node = tmp_ptr;        
    };
    // the very lost node in the list hasn't been deleted, since the while condition
    // doesn't hold, as last node's next_ptr is NULL. So it will stop when current_node =
    // last_node
    free(current_node); // last node, the tail (which the while loop didn't cover)

};




