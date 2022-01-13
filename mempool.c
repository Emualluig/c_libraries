#include "mempool.h"

#include <stdlib.h>

struct {
	unsigned int id;
	unsigned int size;
	
	// Uses this as a bool
	char is_occupied;
	char _a;
	char _b;
	char _c;
} typedef MemorySection;

typedef struct _mempool {
	// This is the pool of memory
	void* memory;
	unsigned int size;
	unsigned int unused_size;

	// For now the linked list will use malloc and free
	// TODO
	MemorySection sections[100];
};


