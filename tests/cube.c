#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <engine0/engine0.h>
#include <engine0/util.h>

float vertices[] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
};

const char *vertex_shader =
"#version 330 core                          \n"
"layout (location = 0) in vec3 aPos;        \n"
"out vec3 posColor;                         \n"
"uniform mat4 projection;                   \n"
"uniform mat4 view;                         \n"
"uniform mat4 model;                        \n"
"void main() { gl_Position =                  "
"              projection * view              "
"                * model * vec4(aPos, 1.0); \n"
"              posColor = aPos + 0.5; }     \n"
;

const char *fragment_shader =
"#version 330 core                                \n"
"in vec3 posColor;                                \n"
"out vec4 FragColor;                              \n"
"void main() { FragColor = vec4(posColor, 1.0); } \n"
;

#define REQUIRE(x) do { if (!x) die(e0_getError()); } while (0)

int main()
{
	REQUIRE(e0_init(
		800, 800,
		"cube",
		true
	));

	e0_mesh *m = e0_createMeshNoEBO(
		// Vertices
		vertices, sizeof(vertices)/sizeof(float),
		// Usage
		GL_STATIC_DRAW,
		// Vertex Attribute Pointer specifications
		&(e0_VAP){ 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0 }, 1
	);
	e0_bindMesh(m);

	e0_shader *s = e0_createShaderFromSrc(vertex_shader, fragment_shader);
	REQUIRE(s);
	e0_useShader(s);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.1f, 0.2f, 1.0f);

	float camera_x = 0.0f, camera_y = 0.0f;

	while (!e0_windowShouldClose()) {
		// Projection matrix
		mat4 projection;
		glm_perspective(
			65.0f * GLM_PI / 180.0f,
			(float)e0_globals.window_width / (float)e0_globals.window_height,
			0.1f,
			100.0f,
			projection
		);
		e0_setu_mat4(s, "projection", projection);

		float seconds = e0_getTime();
		camera_x = sinf(seconds) / 1.5;
		camera_y = cosf(seconds) / 1.5;

		// View matrix
		mat4 view;
		glm_lookat(
			// Camera position
			(vec3){camera_x, camera_y, 3.0f},
			// Camera target
			(vec3){camera_x, camera_y, 0.0f},
			// Up direction
			GLM_YUP,
			// Destination mat4
			view
		);
		e0_setu_mat4(s, "view", view);

		// Model matrix
		mat4 model = GLM_MAT4_IDENTITY_INIT;
		glm_rotate(
			// Destination mat4
			model,
			// Radians
			seconds,
			// Axis multipliers
			(vec3){
				// x
				1.0,
				// y
				0.5,
				// z
				0.25
			}
		);
		e0_setu_mat4(s, "model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		e0_draw();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// e0_destroyMesh(m);
	// e0_destroyShader(s);
	e0_terminate();
}
