#include <types.h>

#define HEAP_START 0x800000
#define HEAP_MAX_SIZE 0x200000
#define BYTE_ALIGNMENT 8 
#define BYTE_ALIGNMENT_MASK 0x0007

#define BLOCK_ALLOCATED_BITMASK                ( ( ( size_t ) 1 ) << ( ( sizeof( size_t ) * 8 ) - 1 ) )
#define BLOCK_SIZE_IS_VALID(size)              ( ( ( size ) & BLOCK_ALLOCATED_BITMASK ) == 0 )
#define BLOCK_IS_ALLOCATED(memoryBlock)        ( ( ( memoryBlock->size ) & BLOCK_ALLOCATED_BITMASK ) != 0 )
#define ALLOCATE_BLOCK(memoryBlock)            ( ( memoryBlock->size ) |= BLOCK_ALLOCATED_BITMASK )
#define FREE_BLOCK(memoryBlock)                ( ( memoryBlock->size ) &= ~BLOCK_ALLOCATED_BITMASK )

typedef struct memoryBlock{
    struct memoryBlock * next;
    size_t size;
} memoryBlock_t;

static memoryBlock_t firstBlock;
static memoryBlock_t lastBlock;

static size_t freeBytesRemaining = HEAP_MAX_SIZE;

static const uint16_t MemoryBlockStructSize = ( ( sizeof( memoryBlock_t ) + ( BYTE_ALIGNMENT - 1 ) ) & ~( ( size_t ) BYTE_ALIGNMENT_MASK ) );

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

void free(void * pv){
    uint8_t * startBlockAddress = (uint8_t *) pv;
    memoryBlock_t * blockToFree;

    if(pv != NULL){
        startBlockAddress -= MemoryBlockStructSize;

        blockToFree = (void *) startBlockAddress;

        // configASSERT( heapBLOCK_IS_ALLOCATED( pxLink ) != 0 );
        // configASSERT( pxLink->pxNextFreeBlock == NULL );
        
        if( BLOCK_IS_ALLOCATED(blockToFree) != 0 ){
            if( blockToFree->next == NULL ){
                /* The block is being returned to the heap - it is no longer allocated. */
                FREE_BLOCK(blockToFree);

                // #if ( configHEAP_CLEAR_MEMORY_ON_FREE == 1 ){
                //     ( void ) memset( puc + heapSTRUCT_SIZE, 0, pxLink->xBlockSize - heapSTRUCT_SIZE );
                // }
                // #endif

                /* Add this block to the list of free blocks. */
                prvInsertBlockIntoFreeList( ( ( memoryBlock_t * ) blockToFree ) );
                freeBytesRemaining += blockToFree->size;

                // traceFREE( pv, pxLink->xBlockSize );
            }
        }
    }
}