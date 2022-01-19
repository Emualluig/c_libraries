#include "mempool.h"

#include <stdlib.h>
#include <assert.h>

struct {
	unsigned int size;
	
	// Uses this as a bool
	char is_occupied;
} typedef MemorySection;

typedef struct _mempool {
	// This is the pool of memory
	void* memory;
	unsigned int size;
	unsigned int unused_size;

	// There will be a 2500 byte buffer

	// There will be a 1600 byte base for overhead (might grow larger)
	// Each object on memory takes an overhead of sizeof(MemorySection)


	// For now the linked list will use malloc and free
	// TODO
	MemorySection sections[100];
	unsigned int mem_sec_length;
};

mempool* mempool_create(unsigned int size) {
	assert(0 < size);

	void* block = malloc(size + sizeof(mempool));
	mempool* base = block;
	if (base != NULL) {
		base->size = size + sizeof(mempool);
		base->mem_sec_length = 0;

		MemorySection mempool_section = { .size = sizeof(mempool), .is_occupied = 1 };
		base->sections[base->mem_sec_length] = mempool_section;
		base->mem_sec_length++;

		MemorySection empty_section = { .size = base->size - sizeof(mempool), .is_occupied = 0 };
		base->sections[base->mem_sec_length] = empty_section;
		base->mem_sec_length++;
	}

	return base;
};

int mempool_destroy(mempool* memory_pool) {
	assert(memory_pool);

	free(memory_pool);
	return 0;
}

void* mempool_malloc(unsigned int size, mempool* memory_pool) {
	assert(0 < size);
	assert(memory_pool);
	assert(memory_pool->mem_sec_length + 1 < 100);

	void* pointer = NULL;
	unsigned long long loc = memory_pool->memory;
	for (unsigned int i = 0; i < memory_pool->mem_sec_length; i++) {
		MemorySection current_section = memory_pool->sections[i];

		// Get first unoccupied section of correct size
		if ((current_section.is_occupied == 0) && (current_section.size >= size)) {

			// If the size is equal, simply make the section occupied
			if (current_section.size == size) {
				memory_pool->sections[i].is_occupied = 1;
			} else {
				MemorySection new_section = { .size = size, .is_occupied = 1 };
				// Shift everything to the right
				for (unsigned int j = memory_pool->mem_sec_length; j > i; j--) {
					memory_pool->sections[j] = memory_pool->sections[j - 1];
				}

				memory_pool->sections[i] = new_section;
				memory_pool->mem_sec_length += 1;
				memory_pool->sections[i + 1].size -= size;
			}

			pointer = loc;
			break;
		}
		loc += current_section.size;
	}
	return pointer;
}

int mempool_free(void* pointer, mempool* memory_pool) {
	unsigned long long loc = (unsigned long long)memory_pool->memory + sizeof(mempool);
	for (int i = 1; i < memory_pool->mem_sec_length; i++) {
		if (loc == (unsigned long long)pointer) {
			memory_pool->sections[i].is_occupied = 0;

			// Merge with adjacent unoccupied memory sections

			// Merge with above unoccupied block
			if ((i + 1 < memory_pool->mem_sec_length) && 
				(memory_pool->sections[i + 1].is_occupied == 0)) {
				memory_pool->sections[i].size += memory_pool->sections[i + 1].size;
				// Remove section at i + 1 by 
				// moving remaining memory section to the left
				for (int j = i + 2; j < memory_pool->mem_sec_length; j++) {
					memory_pool->sections[j - 1] = memory_pool->sections[j];
				}
				memory_pool->mem_sec_length -= 1;
			}

			// Merge with below unoccupied block
			if ((i - 1 >= 0) && 
				(memory_pool->sections[i - 1].is_occupied == 0)) {
				memory_pool->sections[i].size += memory_pool->sections[i - 1].size;
				// Move to the left to overwrite section at i - 1
				for (int j = i; j < memory_pool->mem_sec_length; j++) {
					memory_pool->sections[j - 1] = memory_pool->sections[j];
				}
				memory_pool->mem_sec_length -= 1;
			}

			break;
		}
		loc += memory_pool->sections[i].size;
	}

	return 0;
}

void mempool_print(mempool* memory_pool) {
	assert(memory_pool);
	printf("==========MEMPOOL PRINT START===========\n");
	unsigned long long loc = memory_pool->memory;
	for (int i = 0; i < memory_pool->mem_sec_length; i++) {
		printf("========================================\n");
		printf("MEMORY BLOCK OF SIZE: %d. STARTS AT %p. IS OCCUPIED: %d\n", memory_pool->sections[i].size, loc, memory_pool->sections[i].is_occupied);
		printf("========================================\n");
		loc += memory_pool->sections[i].size;
	}
	printf("===========MEMPOOL PRINT END============\n\n");
}