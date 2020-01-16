#ifndef vector_h
#define vector_h

#include <stdlib.h>
#include <stdbool.h>

typedef void *Item;
typedef void *Vector;
typedef void (*vector_destroy_fn)(Item item);
typedef void (*vector_assign_fn)(Item *left, Item right);

Vector vector_with_capacity(size_t capacity, vector_assign_fn, vector_destroy_fn);
void vector_destroy(Vector v);

#endif
