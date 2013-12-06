#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "malloc.h"
#include "memreq.c"

#define BLOCK_SIZE sizeof(struct Node)  //which is 24
#define PAGE_SIZE 4096  //typical page size

//align the memory so you dont get weird segmentened memory
size_t align(size_t num) {
    if (num % (2*sizeof(size_t)) == 0)
        return (num/(2*sizeof(size_t)))*(2*sizeof(size_t));
    else
        return ((num/(2*sizeof(size_t))) + 1)*(2*sizeof(size_t));
    
}

void print(void * ptr, unsigned size){
    
    char buffer[200] = {0};
    sprintf(buffer, "Pointer at %p size %u \n", ptr, size);
    puts(buffer);
}

//24 bites of memory
struct Node{
    unsigned size;
    int free;
    struct Node * next;
    struct Node * prev;
};

struct Node *rootNode= NULL;


/*Split the current Node into 2 parts within the linked list: one of size "size"
 and the other has the space.  The extra space node is marked free */
struct Node *split(struct Node* newNode, size_t size){

    // must have at least 24 or it's not worth it
    if ((newNode->size - size) >= (BLOCK_SIZE + (2*sizeof(size_t)))){
        struct Node * new;
        new = (struct Node *)((char*)newNode + size+BLOCK_SIZE);
        new->size = newNode->size - size - BLOCK_SIZE;
        new->next = newNode->next;
        new->prev = newNode;
        new->free = 1;
        newNode->size = size;
        newNode->next = new;
    }
    newNode->free = 0;
    return newNode + 1;
    
}

/* Gets the number of pages of memory needed for this size num*/
size_t getPages(size_t num) {
    if (num % PAGE_SIZE == 0)
    return num/PAGE_SIZE;
    else
    return num/PAGE_SIZE + 1;
        
}

void *malloc(size_t size) {
    size = align(size);
    
        //Check to see if your linked list has any free nodes of a large enough size for what we need
        struct Node *last = NULL;
        struct Node *tempNode = rootNode;
        while (tempNode != NULL){
            if(tempNode->size >= size && tempNode->free){
                //we return the split node so that we can save any wasted space
                return split(tempNode,size);
            }
            last = tempNode;
            tempNode = tempNode->next;
           
        }
    //otherwise we have not returned yet and need to get more memory and add this to our linked list
    
    //gets the memory for the newNode.  Get_memory retrives in size of pages so we need to determine how many pages are needed for this memory size and get that many pages worth
    struct Node *newNode = (struct Node *)get_memory(PAGE_SIZE*getPages(size + BLOCK_SIZE));
    //if we have gotten memory then proceed
    if(newNode != NULL){
        //if this is the first node of the linked list do this
        if (rootNode == NULL){
            rootNode = newNode;
            newNode->size = (PAGE_SIZE*getPages(size + BLOCK_SIZE)) - BLOCK_SIZE;
            newNode->next = NULL;
            newNode->prev = NULL;
            newNode->free = 0;
            return split(rootNode,size);
        } //otherwise we already have a root node, so set the node using this
            last->next = newNode;
            newNode->size = (PAGE_SIZE*getPages(size + BLOCK_SIZE)) - BLOCK_SIZE;
            newNode->next = NULL;
            newNode->prev = last;
            newNode->free = 0;
        return split(newNode,size);
    }
    else{
        //no momory left! Error out
        errno = ENOMEM;
        return NULL;
    }
}

//get block from a pointer
void * get_block (void *p)
{
    char *tmp;
    tmp = p;
    tmp -= BLOCK_SIZE;
    return tmp;
}

//merge blocks of memory within the list together into one large block
struct Node * mergeBlock(struct Node *n){
    if (n->next && n->next->free){
        n->size += BLOCK_SIZE + n->next->size;
        n->next = n->next ->next;
        if (n->next != NULL)
            n->next->prev = n;
    }
     return n;
}

/* free a pointers memory, making it available to be used again*/
void free(void* ptr) {
    //for efficency, check these two really quick first
    //If the pointer is null it is invalid, error out
    if (ptr == NULL){
        errno = ENOMEM;
        return;
    }
    //if the pointer is outside the bounds of memory, error out
    if ((void*)ptr < (void*)rootNode || (void*)ptr > (void*)get_memory(0)){
        errno = ENOMEM;
        return;
    }
    
    //check to see that the pointer is valid by making sure it is within our linked list
    struct Node * temp = (struct Node *)get_block(ptr);
    int valid= 0;
    struct Node *tempNode = rootNode;
    //this also makes sure we don't free something that is alraedy free
    while (tempNode != NULL){
        if(temp == tempNode && temp->free == 0){ 
            valid = 1; break;
        }
        tempNode = tempNode->next;
    }
    
    //if we have a valid pointer, we need to set it to be free.  We already checked that it's not already free
    if (valid){
        temp->free = 1;
        
        //merge the block with any free blocks nearby to create large blocks, minimalizing fragmentation.
        //We found this to be a better solution than the ones provided
        
        //merge with previous block, if possible
        if(temp->prev != NULL && temp->prev->free)
            temp = mergeBlock(temp->prev);
        
        //merge with next block, if possible
        if (temp->next != NULL) mergeBlock(temp);
        
    
    }
    else{
        //if not valid then error out
        errno = ENOMEM;
        return;   
    }
    
}

static size_t highest(size_t in) {
    size_t num_bits = 0;
    
    while (in != 0) {
        ++num_bits;
        in >>= 1;
    }
    
    return num_bits;
}

void* calloc(size_t number, size_t size) {
    size_t number_size = 0;
    
    /* This prevents an integer overflow.  A size_t is a typedef to an integer
     * large enough to index all of memory.  If we cannot fit in a size_t, then
     * we need to fail.
     */
    if (highest(number) + highest(size) > sizeof(size_t) * CHAR_BIT) {
        errno = ENOMEM;
        return NULL;
    }
    
    number_size = number * size;
    void* ret = malloc(number_size);
    
    if (ret) {
        memset(ret, 0, number_size);
    }
    
    return ret;
}


void* realloc(void *ptr, size_t size) {
    size = align(size);
    
    struct Node * temp = (struct Node *)get_block(ptr);
    if (ptr == NULL){
        errno = ENOMEM;
        return NULL;
    }
    
    if ((void*)ptr < (void*)rootNode || (void*)ptr > (void*)get_memory(0)){
        errno = ENOMEM;
        return NULL;
    }

    int valid= 0;
    struct Node *tempNode = rootNode;
    while (tempNode != NULL){
        if(temp == tempNode){
            valid = 1;
            break;
        }
        tempNode = tempNode->next;
    }

    if (valid){
        size_t old_size = temp->size; /* XXX Set this to the size of the buffer pointed to by   ptr */
        void* ret = malloc(size);
    
        if (ret) {
            if (ptr) {
                memmove(ret, ptr, old_size < size ? old_size : size);
                free(ptr);
            }
        
            return ret;

        } else {
            errno = ENOMEM;
            return NULL;
        }
    }else {
        errno = ENOMEM;
        return NULL;
    }

}


