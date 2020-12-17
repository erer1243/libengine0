#include <stdbool.h>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "engine0/texture.h"
#include "engine0/xmalloc.h"
#include "engine0/error.h"
#include "engine0/global_object_array.h"

e0_texture *e0_createTexture(const char *path, GLenum format)
{
	stbi_set_flip_vertically_on_load(true);

	// Generate texture and set parameters
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load image data from file
	int w, h, channels;
	unsigned char *data = stbi_load(path, &w, &h, &channels, 0);

	// If the data couldn't be loaded, abort
	if (!data) {
		e0_setError("Loading texture from %s: %s", path, stbi_failure_reason());
		return NULL;
	}

	// Load image data into texture object
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, format,
	             GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Create texture object
	e0_texture *t = xmalloc(sizeof(e0_texture));
	t->id = id;

	// Cleanup image data
	stbi_image_free(data);

	// Append to global object array
	append_object_GOA(t, TEXTURE);

	return t;
}

void e0_destroyTexture(e0_texture *t)
{
	// Remove from global object array
	remove_object_GOA(t);

	free(t);
}

void e0_bindTexture(e0_texture *t, unsigned int number)
{
	if (!t) return;
	glActiveTexture(GL_TEXTURE0 + number);
	glBindTexture(GL_TEXTURE_2D, t->id);
}
