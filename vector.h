
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define TOKENPASTE2(x, y) x##y
#define TOKENPASTE3(x, y, z) x##y##z

#define VECTOR_TYPE(T) TOKENPASTE2(vector_, T)
// Generic vector struct
struct {
    unsigned int length;
    unsigned int size;
    TYPE* data;
    void (*destructor)(TYPE);
} typedef VECTOR_TYPE(TYPE);

#define VECTOR_TYPE_CREATE(T) TOKENPASTE3(vector_, T, _create)
// vector_[type]_create(destructor) creates a vector that contains type
// effects: will allocate memory
// time: O(1)
VECTOR_TYPE(TYPE) VECTOR_TYPE_CREATE(TYPE)(void (*destructor)(TYPE)) {
    VECTOR_TYPE(TYPE) vector = {
        .length = 0,
        .size = 1,
        .data = malloc(sizeof(TYPE) * vector.size),
        .destructor = destructor,
    };
    return vector;
}

#define VECTOR_TYPE_GET(T) TOKENPASTE3(vector_, T, _get)
// vector_[type]_get(vector, index) returns the value at the specified index
// requires: vector is not NULL, index < vector->length
// time: O(1)
TYPE VECTOR_TYPE_GET(TYPE)(const VECTOR_TYPE(TYPE)* vector, unsigned int index) {
    assert(vector);
    assert(vector->data);
    assert(0 <= index);
    assert(index < vector->length);

    TYPE value = vector->data[index];

    return value;
}

#define VECTOR_TYPE_PUSH(T) TOKENPASTE3(vector_, T, _push)
// vector_[type]_push(vector, value) adds value to the end of vector, return false if fails, else true
// requires: vector is not null
// effects: will allocate memory, may fail
// time: O(1) amortized
bool VECTOR_TYPE_PUSH(TYPE)(VECTOR_TYPE(TYPE)* vector, const TYPE value) {
    assert(vector);
    assert(vector->data);

    TYPE* array_pointer = vector->data;
    // Use doubling strategy
    // This is O(n) when doubling
    if (vector->length + 1 >= vector->size) {
        vector->size *= 2;
        TYPE* tmp = realloc(vector->data, vector->size * sizeof(TYPE));

        // Determine whether the allocation failed
        if (tmp == NULL) {
            return false;
        } else {
            array_pointer = tmp;
        }
    }

    // If it didn't fail to allocate, add element to vector
    vector->data = array_pointer;
    vector->data[vector->length] = value;
    vector->length += 1;

    return true;
}

#define VECTOR_TYPE_POP(T) TOKENPASTE3(vector_, T, _pop)
// vector_[type]_pop(vector) 
// requires: vector is not NULL, there is an element to pop
// time: O(1)
TYPE VECTOR_TYPE_POP(TYPE)(VECTOR_TYPE(TYPE)* vector) {
    assert(vector);
    assert(vector->data);
    assert(vector->length > 0);

    // Get last value, then reduce the vector length
    TYPE value = vector->data[vector->length - 1];
    vector->length -= 1;

    return value;
}

#define VECTOR_TYPE_ROTATE(T) TOKENPASTE3(vector_, T, _rotate)
//
void VECTOR_TYPE_ROTATE(TYPE)(VECTOR_TYPE(TYPE)* vector, int rotate_by) {
    // TODO
}

#define VECTOR_TYPE_INSERT(T) TOKENPASTE3(vector_, T, _insert)
//
void VECTOR_TYPE_INSERT(TYPE)(VECTOR_TYPE(TYPE)* vector, int index, const TYPE value) {
    // TODO
}

#define VECTOR_TYPE_EXISTS(T) TOKENPASTE3(vector_, T, _exists)
//
//        compare(arg1, arg2) is negative when arg1 < arg2, 0 when arg1 == arg2, 1 elsewise
// time: O(n)
int VECTOR_TYPE_EXISTS(TYPE)(const VECTOR_TYPE(TYPE)* vector, const TYPE value, int (*compare)(TYPE, TYPE)) {
    // TODO
}

#define VECTOR_TYPE_APPEND(T) TOKENPASTE3(vector_, T, _append)
//
// effects: will destroy source
//
void VECTOR_TYPE_APPEND(TYPE)(VECTOR_TYPE(TYPE)* destination, VECTOR_TYPE(TYPE)* source) {
    // TODO
}

#define VECTOR_TYPE_CLONE(T) TOKENPASTE3(vector_, T, _clone)
// notes: if cloner is NULL, the values in the vector will simplied be copied to the new vector
VECTOR_TYPE(TYPE) VECTOR_TYPE_CLONE(TYPE)(VECTOR_TYPE(TYPE)* vector, TYPE (*cloner)(TYPE)) {
    // TODO
}

#define VECTOR_TYPE_SORT(T) TOKENPASTE3(vector_, T, _sort)
// vector_[type]_sort(vector, compare) sorts the vector
// requires: vector is not NULL, compare is not NULL
// notes: compare(arg1, arg2) is negative when arg1 < arg2, 0 when arg1 == arg2, positive elsewise
// time: O(n*log(n))
void VECTOR_TYPE_SORT(TYPE)(VECTOR_TYPE(TYPE)* vector, int (*compare)(TYPE, TYPE)) {
    assert(vector);
    assert(vector->data);
    assert(compare);

    // TODO
}

#define VECTOR_TYPE_FOREACH(T) TOKENPASTE3(vector_, T, _foreach)
// vector_[type]_foreach(vector, function) applies function to all element in the vector
// requires: vector is not NULL, function is not NULL
// time: O(n)
void VECTOR_TYPE_FOREACH(TYPE)(const VECTOR_TYPE(TYPE)* vector, void (*function)(unsigned int, TYPE*)) {
    assert(vector);
    assert(vector->data);
    assert(function);

    // Loop over elements in vector
    for (unsigned int i = 0; i < vector->length; i++) {
        function(i, &(vector->data[i]));
    }
}

#define VECTOR_TYPE_DESTROY(T) TOKENPASTE3(vector_, T, _destroy)
// vector_[type]_destroy(vector) destroy a vector
// requires: vector is not null
// effects: deallocates memory, if provided a destructor, all elements will be destroyed
// time: O(1) or O(n) with a destructor
void VECTOR_TYPE_DESTROY(TYPE)(VECTOR_TYPE(TYPE)* vector) {
    assert(vector);
    assert(vector->data);
    
    // If the destructor exists, free each element in the array
    if (vector->destructor != NULL) {
        // This is O(n)
        for (unsigned int i = 0; i < vector->length; i++) {
            vector->destructor(vector->data[i]);
        }
    }

    // Free the array
    free(vector->data);
}
