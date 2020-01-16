#include <stdlib.h>
#include <stdbool.h>

#include "xmalloc.h"
#include "vector.h"

typedef struct {
	size_t size, capacity;
	Item *data;
	void (*item_assign)(Item *left, Item right);
	void (*item_destroy)(Item item);
} vector;

Vector vector_with_capacity(size_t capacity,
                            vector_assign_fn assign,
                            vector_destroy_fn destroy)
{
	vector *v   = xmalloc(sizeof(vector));
	v->size     = 0;
	v->capacity = capacity;
	v->data     = xmalloc(capacity);
	return v;
}

void vector_destroy(Vector hv)
{
	vector *v = (vector*)hv;
	for (size_t i = 0; i < v->size; i++)
		v->item_destroy(v->data[i]);
	free(v->data);
	free(v);
}

void vector_push(Vector hv, Item item)
{
	vector *v = (vector*)hv;
	v->size += 1;
	if (v->size > v->capacity) {
		v->capacity *= 2;
		v->data = xrealloc(v->data, v->capacity);
	}
	v->item_assign(&v->data[v->size-1], item);
}

size_t vector_size(Vector hv)
{
	return ((vector*)hv)->size;
}

Item vector_at(Vector hv, size_t index)
{
	return ((vector*)hv)->data[index];
}

bool vector_empty(Vector hv)
{
	return ((vector*)hv)->size == 0;
}
