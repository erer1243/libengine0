#include <stdlib.h>
#include <stdbool.h>

#include "engine0/engine0.h"
#include "engine0/xmalloc.h"
#include "engine0/global_object_array.h"

/* Rudimentary implementation of a global object array.

   In objects, if the data pointer is NULL
   then the object is not occupied. next_free keeps track of the index of the lowest unoccupied
   object. It is updated in append_object_GOA and remove_object_GOA.

   destroying is true if destroy_GOA is being executed. e0_destroy{Shader,Mesh,Texture} all call
   remove_object_GOA, which is unnecessary if the GOA is being destroyed anyways. */
static struct {
	size_t capacity, next_free;
	bool destroying;

	struct object {
		e0_object_type type;
		void *data;
	} *objects;
} goa;

static void double_GOA_size()
{
	size_t old_capacity = goa.capacity;
	goa.capacity *= 2;
	goa.objects = xrealloc(goa.objects, goa.capacity * sizeof(struct object));

	for (size_t i = old_capacity; i < goa.capacity; i++)
		goa.objects[i].data = NULL;
}

static const size_t initial_object_count = 10;
void initialize_GOA()
{
	goa.capacity   = initial_object_count;
	goa.objects    = xmalloc(initial_object_count * sizeof(struct object));
	goa.next_free  = 0;
	goa.destroying = false;

	for (size_t i = 0; i < initial_object_count; i++)
		goa.objects[i].data = NULL;
}

void append_object_GOA(void *object, e0_object_type object_type)
{
	goa.objects[goa.next_free].type = object_type;
	goa.objects[goa.next_free].data = object;

	// Move next_free to next free space in objects
	for (; goa.next_free < goa.capacity && goa.objects[goa.next_free].data != NULL;
	       goa.next_free++);

	// If there are no more free objects, double the capacity
	if (goa.next_free == goa.capacity)
		double_GOA_size();
}

void remove_object_GOA(void *object)
{
	// Currently in destruction process, removing object is not needed.
	if (goa.destroying)
		return;

	// Linear search for pointer in objects
	for (size_t i = 0; i < goa.capacity; i++)
		if (goa.objects[i].data == object) {
			goa.objects[i].data = NULL;

			// If next_free is ahead of this object, move it back
			if (i < goa.next_free)
				goa.next_free = i;

			return;
		}
}

void destroy_GOA()
{
	goa.destroying = true;
	for (size_t i = 0; i < goa.capacity && goa.objects[i].data != NULL; i++)
		switch (goa.objects[i].type) {
		case SHADER:
			e0_destroyShader(goa.objects[i].data);
			break;
		case MESH:
			e0_destroyMesh(goa.objects[i].data);
			break;
		case TEXTURE:
			e0_destroyTexture(goa.objects[i].data);
			break;
		default:
			// Should never happen
			break;
		}

	free(goa.objects);
}
