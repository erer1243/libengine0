#ifndef e0_mesh_h
#define e0_mesh_h

#include <stdbool.h>

typedef struct {
	GLuint VAO, VBO, EBO;
	bool have_ebo;
} e0_mesh;

typedef struct {
	GLuint index;
	GLint size;
	GLenum type;
	GLint normalized; // Using GLboolean produces an error. GLint works fine.
	GLsizei stride;
	const void *offset;
} e0_VAP;

e0_mesh *e0_createMeshEBO(GLfloat *vertices,
                          size_t n_vertices,
                          GLuint *indices,
                          size_t n_indices,
                          GLenum usage,
                          e0_VAP *VAPs,
                          size_t n_VAPs);
e0_mesh *e0_createMeshNoEBO(GLfloat *vertices,
                            size_t n_vertices,
                            GLenum usage,
                            e0_VAP *VAPs,
                            size_t n_VAPs);
void e0_bindMesh(e0_mesh *m);
void e0_destroyMesh(e0_mesh *m);

#endif
