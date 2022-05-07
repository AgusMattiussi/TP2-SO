#include <types.h>

#ifndef POINTER_SIZE_TYPE
    #define POINTER_SIZE_TYPE    uint32_t
#endif

#define HEAP_START 0x800000
#define HEAP_MAX_SIZE 0x200000

#define ADJUSTED_HEAP_SIZE (HEAP_MAX_SIZE - BYTE_ALIGNMENT)

typedef struct memoryBlock{
    struct memoryBlock * next;
    size_t size;
} memoryBlock_t;

static memoryBlock_t firstBlock;
static memoryBlock_t lastBlock;

static size_t freeBytesRemaining = HEAP_MAX_SIZE;
static uint8_t heap[ HEAP_MAX_SIZE ];

static void initializeHeap();

static wasHeapInitialized = FALSE;

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

void * malloc(size_t size){
    memoryBlock_t * block;
    memoryBlock_t * previousBlock;
    memoryBlock_t * newBlock;
}

static void initializeHeap() {
    memoryBlock_t *firstFreeBlock;
    uint8_t alignedHeap;

    /* Ensure the heap starts on a correctly aligned boundary. */
    alignedHeap = ( uint8_t * ) ( ( ( POINTER_SIZE_TYPE ) & heap[ BYTE_ALIGNMENT - 1 ] ) & ( ~( ( POINTER_SIZE_TYPE ) BYTE_ALIGNMENT_MASK ) ) );

    firstBlock.next = (void *) alignedHeap;
    firstBlock.size = (size_t) 0;

    lastBlock.size = ADJUSTED_HEAP_SIZE;
    lastBlock.next = NULL;
    
    firstFreeBlock = (void *) alignedHeap;
    firstFreeBlock->size = ADJUSTED_HEAP_SIZE;
    firstFreeBlock->next = &lastBlock;
}