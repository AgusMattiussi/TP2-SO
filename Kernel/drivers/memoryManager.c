#include <types.h>

#define HEAP_START 0x800000
#define HEAP_MAX_SIZE 0x200000

typedef struct memoryBlock{
    struct memoryBlock * next;
    size_t size;
} memoryBlock_t;

static memoryBlock_t firstBlock;
static memoryBlock_t lastBlock;

static size_t freeBytesRemaining = HEAP_MAX_SIZE;


static void prvInsertBlockIntoFreeList(memoryBlock_t * blockToInsert){                                                                                                                               
    memoryBlock_t * blockIterator;                                                                                                   
    size_t size = blockToInsert->size;                                                                                                          
                                                                                                                                                                                                                       
                                                                                                                                    
    /* Iterate through the list until a block is found that has a larger size */                                                
    /* than the block we are inserting. */                                                                                      
    for(blockIterator = &firstBlock; blockIterator->next->size < size; blockIterator = blockIterator->next);                                                                                                                          
                                                                                                                                    
    /* Update the list to include the block being inserted in the correct */                                                    
    /* position. */                                                                                                             
    blockToInsert->next = blockIterator->next;                                                             
    blockIterator->next = blockToInsert;                                                                              
}