
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
    int  (*compare)(TYPE, TYPE);
    TYPE (*copy)(TYPE);
} typedef VECTOR_TYPE(TYPE);

#define VECTOR_TYPE_CREATE(T) TOKENPASTE3(vector_, T, _create)
// vector_[type]_create(destructor, compare, copy) creates a vector that contains type
// notes: destructor, compare, copy may be NULL but the following functions may not work
//        vector_[type]_exists  (will not work, depends on compare)
//        vector_[type]_sort    (will not work, depends on comapre)
//        vector_[type]_copy    (will perform a shallow copy instead of deep copy)
//        vector_[type]_destroy (will not deallocate the each individual element)
// effects: will allocate memory
// time: O(1)
VECTOR_TYPE(TYPE) VECTOR_TYPE_CREATE(TYPE)(void (*destructor)(TYPE), int (*compare)(TYPE, TYPE), TYPE (*copy)(TYPE)) {
    VECTOR_TYPE(TYPE) vector = {
        .length = 0,
        .size = 1,
        .data = malloc(sizeof(TYPE) * vector.size),
        .destructor = destructor,
        .compare = compare,
        .copy = copy,
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
// effect: modifies vector
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

#define VECTOR_TYPE_REVERSE(T) TOKENPASTE3(vector_, T, _reverse)
// vector_[type]_reverse(vector, from, to) reverses the subsection [from, to) of vector
// effects: will modify vector
// requires: vector is not NULL
// time: O(n)
void VECTOR_TYPE_REVERSE(TYPE)(VECTOR_TYPE(TYPE)* vector, unsigned int from, unsigned int to) {
    assert(vector);
    assert(vector->data);
    
    to -= 1;
    while (from < to) {
        TYPE tmp = vector->data[from];
        vector->data[from] = vector->data[to];
        vector->data[to] = tmp;
        from++;
        to--;
    }
    
}

#define VECTOR_TYPE_ROTATE(T) TOKENPASTE3(vector_, T, _rotate)
// vector_[type]_rotate(vector, rotate) rotates the vector by rotate
// effects: modifies vector
// requires: rotate >= 0
// time: O(n)
void VECTOR_TYPE_ROTATE(TYPE)(VECTOR_TYPE(TYPE)* vector, int rotate) {
    assert(vector);
    assert(vector->data);
    assert(rotate >= 0);

    if ((rotate == 0) || (vector->length < 2)) {
        return;
    }

    unsigned int real_rotate = rotate % (vector->length);
    VECTOR_TYPE_REVERSE(TYPE)(vector, 0, vector->length);
    VECTOR_TYPE_REVERSE(TYPE)(vector, 0, real_rotate);
    VECTOR_TYPE_REVERSE(TYPE)(vector, real_rotate, vector->length);
}

#define VECTOR_TYPE_INSERT(T) TOKENPASTE3(vector_, T, _insert)
// vector_[type]_insert(vector, index, value) inserts into vector at index the value, return false if memory allocation fails
// effects: modifies vector
// requires: vector is not NULL, 0 <= index <= vector->length
// time: O(n)
bool VECTOR_TYPE_INSERT(TYPE)(VECTOR_TYPE(TYPE)* vector, unsigned int index, const TYPE value) {
    assert(vector);
    assert(vector->data);
    assert(0 <= index);

    TYPE* array_pointer = vector->data;
    // Use doubling strategy
    // This is O(n)
    if (vector->length + 1 >= vector->size) {
        vector->size *= 2;
        TYPE* tmp = realloc(vector->data, vector->size * sizeof(TYPE));
        if (tmp == NULL) {
            return false;
        } else {
            array_pointer = tmp;
        }
    }

    // Move values to the right of index to the right
    unsigned int current_index = vector->length;
    while (current_index > index) {
        vector->data[current_index] = vector->data[current_index - 1];
        current_index--;
    }
    // Add the value at index
    vector->data[index] = value;
    vector->length += 1;

    return true;
}

#define VECTOR_TYPE_EXISTS(T) TOKENPASTE3(vector_, T, _exists)
// vector_[type]_exists(vector, value, compare) returns the index of the first occurance of value in the vector
// requires: vector is not NULL, compare is not NULL.
// notes: If the vaue isn't in the vector, returns the length
//        compare(arg1, arg2) is negative when arg1 < arg2, 0 when arg1 == arg2, 1 elsewise
// time: O(n)
unsigned int VECTOR_TYPE_EXISTS(TYPE)(const VECTOR_TYPE(TYPE)* vector, const TYPE value) {
    assert(vector);
    assert(vector->data);
    assert(vector->compare);

    unsigned int i = 0;
    for (; i < vector->length; i++) {
        TYPE loop_value = vector->data[i];
        if (vector->compare(loop_value, value) == 0) {
            return i;
        }
    }
    return i;
}

#define VECTOR_TYPE_COPY(T) TOKENPASTE3(vector_, T, _copy)
// vector_[type]_copy(vector) returns a pointer to a copy of the input vector, null if allocation fails
// requires: vector is not NULL
// effects: will allocate memory
// notes: if copy is NULL, a shallow copy will be performed instead of a deep copy
// time: O(n)
VECTOR_TYPE(TYPE) VECTOR_TYPE_COPY(TYPE)(VECTOR_TYPE(TYPE)* vector) {
    assert(vector);
    assert(vector->data);

    // Copy data from the old array to the new one
    bool custom_copy = vector->copy == NULL ? false : true;
    TYPE* new_array = malloc(sizeof(TYPE) * vector->size);
    for (unsigned int i = 0; i < vector->length; i++) {
        TYPE val = vector->data[i];
        
        // Copy depending on to do shallow or deep copy
        if (custom_copy) {
            new_array[i] = vector->copy(val);
        } else {
            new_array[i] = val;
        }
    }

    // Create the new vector
    VECTOR_TYPE(TYPE) vc = {
        .length = vector->length,
        .size   = vector->size,
        .data   = new_array,
        .destructor = vector->destructor,
        .compare = vector->compare,
        .copy = vector->copy,
    };

    return vc;
}

#define VECTOR_TYPE_SORT(T) TOKENPASTE3(vector_, T, _sort)
// vector_[type]_sort(vector, compare) sorts the vector
// requires: vector is not NULL, compare is not NULL
// notes: compare(arg1, arg2) is negative when arg1 < arg2, 0 when arg1 == arg2, positive elsewise
// time: O(n*log(n))
void VECTOR_TYPE_SORT(TYPE)(VECTOR_TYPE(TYPE)* vector) {
    assert(vector);
    assert(vector->data);
    assert(vector->compare);

    // Uses Shellsort
#define SHELLSORTNUMBER 8
    unsigned int gaps[SHELLSORTNUMBER] = {701, 301, 132, 57, 23, 10, 4, 1};
    for (unsigned int gap_number = 0; gap_number < SHELLSORTNUMBER; gap_number++) {
        unsigned int gap = gaps[gap_number];

        for (unsigned int offset = 0; offset < gap; offset++) {
            for (unsigned int i = offset; i < vector->length; i += gap) {
                TYPE tmp = vector->data[i];

                unsigned int j = i;
                for (; j >= gap && vector->compare(vector->data[j - gap], tmp) > 0; j -= gap) {
                    vector->data[j] = vector->data[j - gap];
                }
                vector->data[j] = tmp;
            }
        }

    }
#undef SHELLSORTNUMBER
}

#define VECTOR_TYPE_FOREACH(T) TOKENPASTE3(vector_, T, _foreach)
// vector_[type]_foreach(vector, function) applies function to all element in the vector
// effects: may modify vector based on function
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

#define VECTOR_TYPE_APPEND(T) TOKENPASTE3(vector_, T, _append)
// vector_[type]_append(destination, source) takes the values from destination, and adds them to the end of source
// effects: will allocate memory
// notes: you caller will need to cell vector_[type]_destroy() on source (to obtain a deep copy)
// requires: destination is not NULL, source is not NULL
// time: O(n)
void VECTOR_TYPE_APPEND(TYPE)(VECTOR_TYPE(TYPE)* destination, VECTOR_TYPE(TYPE)* source) {
    assert(destination);
    assert(destination->data);
    assert(source);
    assert(source->data);

    for (unsigned int i = 0; i < source->length; i++) {
        VECTOR_TYPE_PUSH(TYPE)(destination, source->data[i]);
    }
}
