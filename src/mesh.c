#include <stdlib.h>
#include <stdbool.h>
#include <glad/glad.h>

#include "engine0/mesh.h"
#include "engine0/xmalloc.h"
#include "engine0/global_object_array.h"

static void create_vaps(e0_VAP *vaps, size_t n_vaps)
{
	for (size_t i = 0; i < n_vaps; i++) {
		glVertexAttribPointer(
			vaps[i].index,
			vaps[i].size,
			vaps[i].type,
			vaps[i].normalized,
			vaps[i].stride,
			vaps[i].offset
		);
		glEnableVertexAttribArray(vaps[i].index);
	}
}

e0_mesh *e0_createMeshEBO(GLfloat *vertices,
                          size_t n_vertices,
                          GLuint *indices,
                          size_t n_indices,
                          GLenum usage,
                          e0_VAP *vaps,
                          size_t n_vaps)
{
	e0_mesh *m  = xmalloc(sizeof(e0_mesh));

	// Create VAO
	glGenVertexArrays(1, &m->VAO);
	glBindVertexArray(m->VAO);

	// Create and fill VBO
	glGenBuffers(1, &m->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m->VBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		n_vertices * sizeof(GLfloat),
		vertices,
		usage
	);

	// Create and fill EBO
	glGenBuffers(1, &m->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		n_indices * sizeof(GLuint),
		indices,
		usage
	);

	create_vaps(vaps, n_vaps);

	// Append to global object array
	append_object_GOA(m, MESH);

	return m;
}

e0_mesh *e0_createMeshNoEBO(GLfloat *vertices,
                            size_t n_vertices,
                            GLenum usage,
                            e0_VAP *vaps,
                            size_t n_vaps)
{
	e0_mesh *m  = xmalloc(sizeof(e0_mesh));
	m->have_ebo = false;

	// Create VAO
	glGenVertexArrays(1, &m->VAO);
	glBindVertexArray(m->VAO);

	// Create and fill VBO
	glGenBuffers(1, &m->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m->VBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		n_vertices * sizeof(GLfloat),
		vertices,
		usage
	);

	create_vaps(vaps, n_vaps);

	// Append to global object array
	append_object_GOA(m, MESH);

	return m;
}

void e0_bindMesh(e0_mesh *m)
{
	glBindVertexArray(m->VAO);
}

void e0_destroyMesh(e0_mesh *m)
{
	glDeleteVertexArrays(1, &m->VAO);
	glDeleteBuffers(1, &m->VBO);
	if (m->have_ebo)
		glDeleteBuffers(1, &m->EBO);

	// Remove from global object array
	remove_object_GOA(m);

	free(m);
}
