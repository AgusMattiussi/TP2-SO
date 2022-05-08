#include <types.h>

#define HEAP_START 0x800000
#define HEAP_MAX_SIZE 0x200000

#define BYTE_ALIGNMENT 8 
#define BYTE_ALIGNMENT_MASK 0x0007

/* Chequea que la suma entre a y b sea menor que HEAP_MAX_SIZE */
#define heapADD_WILL_OVERFLOW(a, b) ((a) > (HEAP_MAX_SIZE - (b)))

typedef struct memoryBlock{
    struct memoryBlock * next;
    size_t size;
} memoryBlock_t;

static const uint16_t MemoryBlockStructSize = ((sizeof(memoryBlock_t) + (BYTE_ALIGNMENT - 1)) & ~((size_t)BYTE_ALIGNMENT_MASK));
#define MINIMUM_BLOCK_SIZE ((size_t)(MemoryBlockStructSize * 2))

static memoryBlock_t firstBlock;
static memoryBlock_t lastBlock;

static size_t freeBytesRemaining = HEAP_MAX_SIZE;

static wasHeapInitialized = FALSE;

static void insertBlockIntoFreeList(memoryBlock_t * blockToInsert){                                                                                                                               
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

void * malloc(size_t wantedSize){
    memoryBlock_t * currentBlock;
    memoryBlock_t * previousBlock;
    void * retPointer = NULL;
    size_t aditionalRequiredSize;

    if(wasHeapInitialized = FALSE){
        /*** INICIALIZAR HEAP ***/
        wasHeapInitialized = TRUE;
    }

    if(wantedSize <= 0)
        return NULL;

    /* Al espacio que me pide el usuario, le sumo el tamanio de un bloque */
    aditionalRequiredSize = MemoryBlockStructSize + BYTE_ALIGNMENT - (wantedSize & BYTE_ALIGNMENT_MASK);

    /* Si no hay overflow, trabajo con este nuevo size */
    if(heapADD_WILL_OVERFLOW(wantedSize, aditionalRequiredSize) == FALSE)
        wantedSize += aditionalRequiredSize;
    else
        return NULL;

    if(wantedSize < freeBytesRemaining){
        /* Los bloques se ordenan de menor a mayor size. Recorro la lista
         * hasta encontrar uno de tamanio adecuado (Best Fit). */
        previousBlock = &firstBlock;
        currentBlock = firstBlock.next;
        
        while(currentBlock->size < wantedSize && currentBlock->next != NULL){
            previousBlock = currentBlock;
            currentBlock = currentBlock->next;
        }

        /* Si llegue a lastBlock, significa que no encontre un bloque adecuado*/
        if(currentBlock == &lastBlock)
            return NULL;
        
        retPointer = (void *)(currentBlock + MemoryBlockStructSize);
            
        /* Saco este bloque de la lista */
        previousBlock->next = currentBlock->next;
        currentBlock->next = NULL;

        /* Si este bloque es mas grande de lo necesario, lo divido en dos
        *  y asigno al bloque sobrante como un nuevo bloque libre */
        if(currentBlock->size - wantedSize > MINIMUM_BLOCK_SIZE) {
            memoryBlock_t * newBlock = (void *)(currentBlock + wantedSize); 

            newBlock->size = currentBlock->size - wantedSize;
            currentBlock->size = wantedSize;

            /* Inserto el nuevo bloque con el espacio sobrante en la
            * lista de bloques libres */
            insertBlockIntoFreeList(newBlock);
        }

        freeBytesRemaining -= currentBlock->size;
    }

    return retPointer;
}