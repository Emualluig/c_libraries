#include "string.h"

struct {
	char letter;
} typedef CharWrapper;

#define TYPE CharWrapper
#include "vector.h"
#undef TYPE

