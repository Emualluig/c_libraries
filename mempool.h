#ifndef _HEADER_MEMPOOL_H_
#define _HEADER_MEMPOOL_H_

typedef struct _mempool;
typedef struct _mempool mempool;

//
mempool* mempool_create(unsigned int size);

//
void* mempool_malloc(unsigned int size, mempool* memory_pool);

//
void* mempool_realloc(void* pointer, unsigned int size, mempool* memory_pool);

// This function will consolidate memory by moving already allocated blocks to
// the bottom, thereby possibly freeing up a large continuous block, but
// making future reallocs slower
// Returns 0 is unable to condense
int mempool_condense(mempool* memory_pool);

//
void mempool_print(mempool* memory_pool);

// Returns 0 if pointer does not pointer to a allocated block of memory
int mempool_free(void* pointer, unsigned int size, mempool* memory_pool);

// Returns 1 if all memory was empty
int mempool_destroy(mempool* memory_pool);

#endif // !_HEADER_MEMPOOL_H_
