#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "malloc.h"
#include "memreq.h"

#define BLOCK_SIZE sizeof(struct Node)
#define PAGE_SIZE 4096

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


typedef struct Node{
    unsigned size;
    int free;
    struct Node * next;
    struct Node * prev;
} Node;

struct Node *rootNode= NULL;

struct Node *split(struct Node* newNode, size_t size){
    
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


size_t getPages(size_t num) {
    if (num % PAGE_SIZE == 0)
    return num/PAGE_SIZE;
    else
    return num/PAGE_SIZE + 1;
        
}

void *malloc(size_t size) {
    size = align(size);
    
        // loop through nodes
        struct Node *last = NULL;
        struct Node *tempNode = rootNode;
        while (tempNode != NULL){
            if(tempNode->size >= size && tempNode->free){
                return split(tempNode,size);
            }
            last = tempNode;
            tempNode = tempNode->next;
           
        }
 
    struct Node *newNode = (struct Node *)get_memory(PAGE_SIZE*getPages(size + BLOCK_SIZE));
    if(newNode != NULL){
        if (rootNode == NULL){
            rootNode = newNode;
            newNode->size = (PAGE_SIZE*getPages(size + BLOCK_SIZE)) - BLOCK_SIZE;
            newNode->next = NULL;
            newNode->prev = NULL;
            newNode->free = 0;
            return split(rootNode,size);
        }
            last->next = newNode;
            newNode->size = (PAGE_SIZE*getPages(size + BLOCK_SIZE)) - BLOCK_SIZE;
            newNode->next = NULL;
            newNode->prev = last;
            newNode->free = 0;
        return split(newNode,size);
    }else{
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

//merge blocks
struct Node * mergeBlock(struct Node *n){
    if (n->next && n->next->free){
        n->size += BLOCK_SIZE + n->next->size;
        n->next = n->next ->next;
        if (n->next != NULL)
            n->next->prev = n;
    }
     return n;
}

void free(void* ptr) {
    if (ptr == NULL){
        errno = ENOMEM;
        return;
    }
    
    if ((void*)ptr < (void*)rootNode || (void*)ptr > (void*)get_memory(0)){
        errno = ENOMEM;
        return;
    }
    
    struct Node * temp = (struct Node *)get_block(ptr);
    int valid= 0;
    struct Node *tempNode = rootNode;
    while (tempNode != NULL){
        if(temp == tempNode){ valid = 1; break; }
        tempNode = tempNode->next;
    }
    if (valid){
        temp->free = 1;
        
        //merge blocks
        if(temp->prev != NULL && temp->prev->free)
            temp = mergeBlock(temp->prev);
        
        if (temp->next != NULL) mergeBlock(temp);
        
    
    }else{
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


