#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <glad/glad.h>

#include "engine0/shader.h"
#include "engine0/error.h"
#include "engine0/xmalloc.h"
#include "engine0/util.h"
#include "engine0/global_object_array.h"

/* Currently the uniform map size is a constant, never rehashing. It seems
   reasonable because how many uniforms are you going to have for one shader? */
static const size_t uniform_map_size = 10;

// Reads an entire file into a heap-allocated buffer.
static char *read_whole_file(const char *path)
{
	FILE *f = fopen(path, "r");
	if (f == NULL)
		return NULL;

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	rewind(f);

	char *str = xmalloc(fsize + 1);
	fread(str, 1, fsize, f);
	fclose(f);
	str[fsize] = 0;

	return str;
}

// Basic hash function sourced from http://www.cse.yorku.ca/~oz/hash.html
static size_t djb2(const char *str)
{
	size_t hash = 5381;
	while (*str)
		hash = ((hash << 5) + hash) ^ *str++;
	return hash;
}

// Initializes the uniform_map field of a shader object.
static void init_uniform_map(e0_shader *s)
{
	s->uniform_map.size = uniform_map_size;
	s->uniform_map.nodes = xmalloc(uniform_map_size *
	                               sizeof(e0_uniformMapNode*));

	for (size_t i = 0; i < uniform_map_size; i++)
		s->uniform_map.nodes[i] = NULL;
}

// Returns a new heap-allocated uniform map node for the given uniform name and location
static e0_uniformMapNode *new_map_node(const GLchar *name, GLint location)
{
	e0_uniformMapNode *node = xmalloc(sizeof(e0_uniformMapNode));
	node->name     = strdup_GLchar(name);
	node->location = location;
	node->next     = NULL;
	return node;
}

// Frees memory associated with a uniform map node
static void destroy_map_node(e0_uniformMapNode *node)
{
	free(node->name);
	free(node);
}

/* Checks the uniform map of the given shader object, looking for its location. Adds a new entry
   if the uniform has not been used before. */
static GLint uniform_location(e0_shader *s, const GLchar *name)
{
	// Hash the uniform's name into an index
	size_t i = djb2(name) % s->uniform_map.size;

	// Walk through the linked list at the given index, looking for a matching uniform name
	e0_uniformMapNode *prev = NULL,
	                  *curr = s->uniform_map.nodes[i];
	while (curr) {
		// If a match was found, return the location
		if (strcmp(name, curr->name) == 0)
			return curr->location;

		prev = curr;
		curr = curr->next;
	}

	// Since no match was found, create a new node
	e0_uniformMapNode *new_node =
		new_map_node(name,  glGetUniformLocation(s->program, name));

	// And add the new node to the list
	if (prev)
		prev->next = new_node;
	else
		s->uniform_map.nodes[i] = new_node;

	return new_node->location;
}

e0_shader *e0_createShaderFromPath(const char *vert_shader_path,
                                const char *frag_shader_path)
{
	// Read sources
	char *vert_shader_src, *frag_shader_src;
	if ((vert_shader_src = read_whole_file(vert_shader_path)) == NULL) {
		e0_setError("Reading vertex shader \"%s\": %s", vert_shader_path, strerror(errno));
		return NULL;
	}

	if ((frag_shader_src = read_whole_file(frag_shader_path)) == NULL) {
		e0_setError("Reading fragment shader \"%s\": %s", frag_shader_path, strerror(errno));
		return NULL;
	}

	// Create shader
	e0_shader *s = e0_createShaderFromSrc(vert_shader_src, frag_shader_src);

	// Cleanup and free
	free(vert_shader_src);
	free(frag_shader_src);

	// Don't append to GOA because that's done in e0_createShaderFromSrc

	return s;
}

e0_shader *e0_createShaderFromSrc(const GLchar *vert_shader_src,
                               const GLchar *frag_shader_src)
{
	// Create and compile shaders
	GLuint vert_shader, frag_shader;
	vert_shader = glCreateShader(GL_VERTEX_SHADER);
	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vert_shader, 1, &vert_shader_src, NULL);
	glShaderSource(frag_shader, 1, &frag_shader_src, NULL);
	glCompileShader(vert_shader);
	glCompileShader(frag_shader);

	// Check for shader compilation errors
	char info_log[512];
	GLint vert_success, frag_success;
	glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &vert_success);
	glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &frag_success);

	if (!vert_success) {
		glGetShaderInfoLog(vert_shader, 512, NULL, info_log);
		e0_setError("Vertex shader: %s", info_log);
		glDeleteShader(vert_shader);
		glDeleteShader(frag_shader);
		return NULL;
	}

	if (!frag_success) {
		glGetShaderInfoLog(frag_shader, 512, NULL, info_log);
		e0_setError("Fragment shader: %s", info_log);
		glDeleteShader(vert_shader);
		glDeleteShader(frag_shader);
		return NULL;
	}

	// Link the shaders into a program
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vert_shader);
	glAttachShader(shader_program, frag_shader);
	glLinkProgram(shader_program);

	// Check for linking errors
	int link_success;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &link_success);

	if (!link_success) {
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		e0_setError("Linking shaders: %s", info_log);
		glDeleteShader(vert_shader);
		glDeleteShader(frag_shader);
		glDeleteProgram(shader_program);
		return NULL;
	}

	// Cleanup shaders
	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	// Create a shader struct
	e0_shader *s = xmalloc(sizeof(e0_shader));
	s->program = shader_program;
	init_uniform_map(s);

	// Append shader to global object array
	append_object_GOA(s, SHADER);

	return s;
}

void e0_destroyShader(e0_shader *s)
{
	glDeleteProgram(s->program);

	// Free uniform map
	for (size_t i = 0; i < s->uniform_map.size; i++) {
		e0_uniformMapNode *next, *curr = s->uniform_map.nodes[i];
		while (curr) {
			next = curr->next;
			destroy_map_node(curr);
			curr = next;
		}
	}
	free(s->uniform_map.nodes);

	remove_object_GOA(s);

	free(s);
}

void e0_useShader(e0_shader *s)
{
	glUseProgram(s->program);
}

void e0_setu_bool(e0_shader *s, const GLchar *name, bool val)
{
	glUniform1i(uniform_location(s, name), (int)val);
}

void e0_setu_int(e0_shader *s, const GLchar *name, int val)
{
	glUniform1i(uniform_location(s, name), val);
}

void e0_setu_float(e0_shader *s, const GLchar *name, float val)
{
	glUniform1f(uniform_location(s, name), val);
}

void e0_setu_vec3(e0_shader *s, const GLchar *name, vec3 val)
{
	glUniform3fv(uniform_location(s, name), 1, val);
}

void e0_setu_mat4(e0_shader *s, const GLchar *name, mat4 val)
{
	glUniformMatrix4fv(uniform_location(s, name), 1, GL_FALSE, (float*)val);
}
