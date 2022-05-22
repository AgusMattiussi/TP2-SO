#include <types.h>
#include <lib.h>

#ifndef POINTER_SIZE_TYPE
    #define POINTER_SIZE_TYPE uint64_t
#endif

#define HEAP_START 0x800000
#define HEAP_MAX_SIZE 0x200000
#define BYTE_ALIGNMENT 8 
#define BYTE_ALIGNMENT_MASK 0x0007
#define ADJUSTED_HEAP_SIZE (HEAP_MAX_SIZE - BYTE_ALIGNMENT)

/*
#define BLOCK_ALLOCATED_BITMASK                ( ( ( size_t ) 1 ) << ( ( sizeof( size_t ) * 8 ) - 1 ) )
#define BLOCK_SIZE_IS_VALID(size)              ( ( ( size ) & BLOCK_ALLOCATED_BITMASK ) == 0 )
#define BLOCK_IS_ALLOCATED(memoryBlock)        ( ( ( memoryBlock->size ) & BLOCK_ALLOCATED_BITMASK ) != 0 )
#define ALLOCATE_BLOCK(memoryBlock)            ( ( memoryBlock->size ) |= BLOCK_ALLOCATED_BITMASK )
#define FREE_BLOCK(memoryBlock)                ( ( memoryBlock->size ) &= ~BLOCK_ALLOCATED_BITMASK )
*/

/* Chequea que la suma entre a y b sea menor que HEAP_MAX_SIZE */
#define heapADD_WILL_OVERFLOW(a, b) ((a) > (HEAP_MAX_SIZE - (b)))
/* Chequea que el producto entre a y b sea menor que HEAP_MAX_SIZE */
#define heapMULTIPLY_WILL_OVERFLOW(a, b) (((a) > 0) && ((b) > (HEAP_MAX_SIZE/(a))))

typedef struct memoryBlock{
    struct memoryBlock * next;
    size_t size;
} memoryBlock_t;

static const uint16_t MemoryBlockStructSize = ((sizeof(memoryBlock_t) + (BYTE_ALIGNMENT - 1)) & ~((size_t)BYTE_ALIGNMENT_MASK));
#define MINIMUM_BLOCK_SIZE ((size_t)(MemoryBlockStructSize * 2))

/* Estos bloques se utilizan como 'marcadores', representando el comienzo
*  y fin de la lista de bloques respectivamente */
static memoryBlock_t firstBlock;
static memoryBlock_t lastBlock;

static size_t freeBytesRemaining = HEAP_MAX_SIZE;
//static uint8_t heap[ HEAP_MAX_SIZE ];
static uint8_t * heap = (uint8_t *) HEAP_START;
static wasHeapInitialized = FALSE; 

static void insertBlockIntoFreeList(memoryBlock_t * blockToInsert){                                                                                                                               
    memoryBlock_t * blockIterator;                                                                                                   
    size_t size = blockToInsert->size;                                                                                                          
                                                                                                                                                                                                                       
    /* La lista de bloques libres se ordena de menor a mayor tamanio. Entonces,
    *  se itera por la lista hasta encontrar un bloque cuyo tamanio sea mayor al
    *  tamanio del bloque a insertar */                                                                                                                                                                                                                  
    for(blockIterator = &firstBlock; blockIterator->next->size < size; blockIterator = blockIterator->next);                                                                                                                          
                                                                                                                                    
    /* Insertamos el bloque en la lista */                                                                                                             
    blockToInsert->next = blockIterator->next;                                                             
    blockIterator->next = blockToInsert;                                                                              
}

/* Se llama esta funcion la primera vez que se hace un malloc. Inicializa el firstBlock y
*  lastBlock, a la vez que crea un primer bloque libre cuyo tamanio es todo el heap */
static void initializeHeap() {
    memoryBlock_t * firstFreeBlock;
    uint8_t * alignedHeap;

    /* Me aseguro de que el heap comience en una posicion alineada */
    alignedHeap = (uint8_t *) (((POINTER_SIZE_TYPE) & heap[BYTE_ALIGNMENT-1]) & (~((POINTER_SIZE_TYPE) BYTE_ALIGNMENT_MASK)));

    firstBlock.next = (void *) alignedHeap;
    firstBlock.size = (size_t) 0;

    lastBlock.size = ADJUSTED_HEAP_SIZE;
    lastBlock.next = NULL;
    
    firstFreeBlock = (void *) alignedHeap;
    firstFreeBlock->size = ADJUSTED_HEAP_SIZE;
    firstFreeBlock->next = &lastBlock;
}

void * malloc(size_t wantedSize){
    memoryBlock_t * currentBlock;
    memoryBlock_t * previousBlock;
    void * retPointer = NULL;
    size_t aditionalRequiredSize;

    if(wasHeapInitialized == FALSE){
        initializeHeap();
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

    /* Puedo chequear que el puntero sea valido
    if(retPointer < HEAP_START || retPointer > HEAP_START + HEAP_MAX_SIZE)
        return NULL;
     */
    
    return retPointer;
}

void * calloc(size_t typeSize, size_t typeCount){
    if(heapMULTIPLY_WILL_OVERFLOW(typeSize, typeCount) == TRUE)
        return NULL;

    void * retPointer = malloc(typeSize * typeCount);

    if(retPointer != NULL)
        memset(retPointer, 0, typeSize * typeCount);
    
    return retPointer;
}

void free(void * pointer){
    memoryBlock_t * blockToFree;

    if(pointer != NULL){
        /* Antes de cada puntero entregado por malloc, se encuentra
        * un memoryBlock_t con la informacion del bloque entregado */
        blockToFree = (void *) (pointer - MemoryBlockStructSize);

        if(blockToFree->next == NULL ){
            /* Aniado este bloque a la lista de bloques libres */
            insertBlockIntoFreeList((memoryBlock_t *) blockToFree);
            freeBytesRemaining += blockToFree->size;
        }
    }
}


