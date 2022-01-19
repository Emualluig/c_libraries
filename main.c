
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

	{
		mempool* mp = mempool_create(5000);

		void* a = mempool_malloc(128, mp);
		void* b = mempool_malloc(128, mp);
		void* c = mempool_malloc(4744, mp);
		void* d = mempool_malloc(128, mp);

		assert(a);
		assert(b);
		assert(c);
		assert(!d);

		//mempool_print(mp);

		mempool_destroy(mp);
	}
	{
		mempool* mp = mempool_create(5000);

		void* a = mempool_malloc(128, mp);
		void* b = mempool_malloc(128, mp);
		void* c = mempool_malloc(128, mp);
		void* d = mempool_malloc(128, mp);
		void* e = mempool_malloc(128, mp);
		void* f = mempool_malloc(128, mp);
		void* g = mempool_malloc(128, mp);
		void* h = mempool_malloc(128, mp);

		printf("a: %p, b: %p, c: %p\n", a, b, c);

		mempool_free(c, mp);
		mempool_free(b, mp);
		mempool_free(a, mp);
		mempool_print(mp);

		mempool_free(e, mp);
		mempool_free(g, mp);
		mempool_free(f, mp);
		mempool_print(mp);

		mempool_free(h, mp);
		mempool_free(d, mp);
		mempool_print(mp);

		mempool_destroy(mp);
	}


}

int main()
{

	mempool_test();

#if debugging
	_CrtDumpMemoryLeaks();
#endif // debugging

	return 0;
}

