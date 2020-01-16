#include <stdlib.h>

#include "engine0/xmalloc.h"
#include "engine0/util.h"

void *xmalloc(size_t size)
{
	void *ptr = malloc(size);
	if (ptr == NULL)
		die("Out of memory");
	return ptr;
}

void *xrealloc(void *ptr, size_t size)
{
	void *new = realloc(ptr, size);
	if (size != 0 && new == NULL)
		die("Out of memory");
	return new;
}
