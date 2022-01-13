#include "string.h"

#include <stdlib.h>

struct {
	char letter;
} typedef CharWrapper;

#define TYPE CharWrapper
#include "vector.h"
#undef TYPE

#include <stdio.h>

typedef struct {
	vector_CharWrapper char_vector;
	CharWrapper k;
} _string;

//
// time: O(1) if string_literal is NULL, O(n) if not
string* string_create(char *string_literal) {

	// TODO

	if (string_literal != NULL) {
		unsigned int index = 0;
		while (string_literal[index] != '\0') {
			printf("%c\n", string_literal[index]);
			index++;
		}
	}

	return NULL;
}

/*
 * Convert to string functions
 * Currently available types:
 *  int, unsigned int, float, double
 */

//
string* string_from_int() {
	// TODO
	return NULL;
}

//
string* string_clone(string* str) {
	// TODO
	return NULL;
}

//
void string_destroy(string* str) {
	if (str == NULL) {
		return;
	}
	// TODO

}
