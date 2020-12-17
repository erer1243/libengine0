#ifndef global_object_array_h
#define global_object_array_h

typedef enum {
	SHADER,
	MESH,
	TEXTURE,
} e0_object_type;

void initialize_GOA();
void append_object_GOA(void *object, e0_object_type object_type);
void remove_object_GOA(void *object);
void destroy_GOA();

#endif
