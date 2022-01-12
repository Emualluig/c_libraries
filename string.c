#include "string.h"

#include <stdlib.h>

struct {
	char letter;
} typedef CharWrapper;

#define TYPE CharWrapper
#include "vector.h"
#undef TYPE

#include <stdio.h>

struct _string {
	vector_CharWrapper char_vector;
	CharWrapper k;
};

string* string_create(char *string_literal) {

	if (string_literal != NULL) {
		unsigned int index = 0;
		while (string_literal[index] != '\0') {
			printf("%c\n", string_literal[index]);
			index++;
		}
	}

	return NULL;
}
