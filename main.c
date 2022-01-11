
#define debugging true
#if debugging
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // debugging


#include <stdlib.h>
#include <stdio.h>

#define TYPE int
#include "vector.h"
#undef TYPE

void double_n(unsigned int i, int *n) {
	*n *= 2;
}

void print_n(unsigned int i, int *n) {
	printf("%d: %d\n", i % 2, *n);
}

int main()
{
	vector_int vi = vector_int_create(NULL);
	
	vector_int_push(&vi, 0);
	vector_int_push(&vi, 1);
	vector_int_push(&vi, 2);
	vector_int_push(&vi, 3);
	vector_int_push(&vi, 4);
	vector_int_push(&vi, 5);
	vector_int_push(&vi, 6);
	vector_int_push(&vi, 7);
	vector_int_push(&vi, 8);
	vector_int_push(&vi, 9);

	vector_int_foreach(&vi, double_n);
	vector_int_foreach(&vi, print_n);
	
	vector_int_destroy(&vi);

#if debugging
	_CrtDumpMemoryLeaks();
#endif // debugging


	return 0;
}

