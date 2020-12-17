#ifndef e0_texture_h
#define e0_texture_h

#include <glad/glad.h>

typedef struct {
	GLuint id;
} e0_texture;

/**
 * Loads a texture from a file into an OpenGL texture object.
 * @param  path   Path of image to load texture from
 * @param  format Format of image data, as specified to glTexImage2D. Accepted
 *                values are: GL_RED, GL_RG, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA,
 *                GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_BGR_INTEGER,
 *                GL_RGBA_INTEGER, GL_BGRA_INTEGER, GL_STENCIL_INDEX,
 *                GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL
 * @return        A texture object or NULL on error (get error with e0_getError())
 */
e0_texture *e0_createTexture(const char *path, GLenum format);

/**
 * Destroys given texture object
 * @param t Texture object to be freed
 */
void e0_destroyTexture(e0_texture *t);

/**
 * Binds the given texture to slot GL_TEXTURE0 + number.
 * @param t      Texture object to bind
 * @param number Slot to bind to. According to OpenGL spec, there must be at
 *               least 80 slots available.
 */
void e0_bindTexture(e0_texture *t, unsigned int number);

#endif
