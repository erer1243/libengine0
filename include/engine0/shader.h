#ifndef e0_shader_h
#define e0_shader_h

#include <stdlib.h>
#include <stdbool.h>
#include <cglm/types.h>
#include <glad/glad.h>

/* Uniform locations are cached in a linked-list hashmap (hashmap where, on
   collision, a node is added to a linked list). This is the list node. */
struct e0_uniformMapNode;
typedef struct e0_uniformMapNode e0_uniformMapNode;
struct e0_uniformMapNode {
	GLchar *name;
	GLint location;
	e0_uniformMapNode *next;
};

typedef struct {
	// Shader program ID
	GLuint program;

	// Uniform location cache map
	struct {
		size_t size;
		e0_uniformMapNode **nodes;
	} uniform_map;
} e0_shader;

/**
 * Creates a shader from the given sources. The source strings will not be
 * touched by this function.
 * @param  vert_shader_src Vertex shader source code
 * @param  frag_shader_src Fragment shader source code
 * @return                 A shader object on success, NULL on failure
 *                         (get error with e0_getError())
 */
e0_shader *e0_createShaderFromSrc(const GLchar *vert_shader_src,
                                  const GLchar *frag_shader_src);

/**
 * Creates a shader from the given files.
 * @param  vert_shader_path Vertex shader source file path
 * @param  frag_shader_path Fragment shader source file path
 * @return                  A shader object on success, NULL on failure
 *                          (get error with e0_getError())
 */
e0_shader *e0_createShaderFromPath(const char *vert_shader_path,
                                   const char *frag_shader_path);

/**
 * Destroys and frees memory of a given shader object. Assumes the shader
 * object is valid and non-NULL.
 * @param s Shader to be destroyed
 */
void e0_destroyShader(e0_shader *s);

void e0_useShader(e0_shader *s);

void e0_setu_bool(e0_shader  *s, const GLchar *uniform_name, bool  data);
void e0_setu_int(e0_shader   *s, const GLchar *uniform_name, int   data);
void e0_setu_float(e0_shader *s, const GLchar *uniform_name, float data);
void e0_setu_vec3(e0_shader  *s, const GLchar *uniform_name, vec3  data);
void e0_setu_mat4(e0_shader  *s, const GLchar *uniform_name, mat4  data);

#endif
