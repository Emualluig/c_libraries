
#define debugging true
#if debugging
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // debugging

#include <stdlib.h>
#include <stdio.h>


// START OF TESTS FOR VECTOR
void double_n(unsigned int i, int *n) {
	*n *= 2;
}
void print_n(unsigned int i, int *n) {
	printf("%d: %d\n", i, *n);
}
int comp_n(int a, int b) {
	if (a == b) {
		return 0;
	} else if (a < b) {
		return -1;
	} else {
		return 1;
	}
}
#define TYPE int
#include "vector.h"
#undef TYPE
void vector_test() {
	vector_int vi = vector_int_create(NULL, comp_n, NULL);

	vector_int_push(&vi, 6);
	vector_int_push(&vi, 1);
	vector_int_push(&vi, 4);
	vector_int_push(&vi, 9);
	vector_int_push(&vi, 2);
	vector_int_push(&vi, 7);
	vector_int_push(&vi, 0);
	vector_int_push(&vi, 5);
	vector_int_push(&vi, 8);
	vector_int_push(&vi, 3);

	vector_int vi2 = vector_int_copy(&vi);

	vector_int_foreach(&vi, double_n);

	vector_int_append(&vi, &vi2);

	vector_int_sort(&vi);

	vector_int_remove(&vi, 10);

	vector_int_foreach(&vi, print_n);


	vector_int_destroy(&vi);
	vector_int_destroy(&vi2);
}

// TESTS FOR STRING
#include "string.h"
void string_test() {
	string* w = string_create("");

	string_destroy(w);
}

// TESTS FOR MEMPOOL
#include "mempool.h"
void mempool_test() {

}

int main()
{

	mempool_test();

#if debugging
	_CrtDumpMemoryLeaks();
#endif // debugging

	return 0;
}

