#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "main.h"
#include "util.h"
#include "window.h"

int main(int argc, char **argv)
{
	char *err;
	if ((err = window_init())) {
		eputsfree(err);
		return 1;
	}

	// Load shaders
	// ------------
	unsigned int shader_program = create_shader("src/shader.vert",
	                                            "src/shader.frag");
	if (!shader_program)
		return 1;

	// Shape data
	// ----------
	// float vertices[] = {
	// 	// Vertices          Texture Coords
	// 	-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left
	// 	-0.5f,  0.5f, 0.0f,  0.0f, 1.0f, // top left
	// 	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // bottom right
	// 	 0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // top right
	// };
	//
	// unsigned int indices[] = {
	// 	0, 1, 2,
	// 	3, 1, 2,
	// };

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	};

	// 1. create objects (vertex array object, vertex buffer object, element buffer object)
	unsigned int VAO, VBO/*, EBO*/;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// glGenBuffers(1, &EBO);

	// 2. bind vertex array object (for following calls to be applied to)
	glBindVertexArray(VAO);

	// 3. copy vertices into vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 4. copy index array into element buffer object
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
	//              indices, GL_STATIC_DRAW);

	// 5. setup vertex attribute pointers
	#define VP(n) ((void*)(n))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
	                      5 * sizeof(float), VP(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
	                      5 * sizeof(float), VP(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Load textures
	stbi_set_flip_vertically_on_load(true);
	unsigned int tex = load_texture("res/face.jpg", GL_RGB),
	             tex2 = load_texture("res/tux.png", GL_RGBA);
	if (!tex || !tex2)
		return 1;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);

	// Shader must be used before setting uniforms
	glUseProgram(shader_program);

	// Map texture units to samplers via their index
	setu_int(shader_program, "tex1", 0);
	setu_int(shader_program, "tex2", 1);

	// Other OpenGL settings
	// ---------------------
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glBindVertexArray(VAO);

	// Render loop vars
	// -----------
	#define RAD(deg) ((deg) * GLM_PI / 180.0)
	#define VEC3(...) ((vec3){__VA_ARGS__})
	#define VEC3S(scalar) ((vec3){scalar, scalar, scalar})
	#define PRESSED(keycode) (window_key_pressed(keycode))

	vec3 cube_positions[] = {
		{ 0.0f,  0.0f,  0.0f  },
		{ 2.0f,  5.0f, -15.0f },
		{-1.5f, -2.2f, -2.5f  },
		{-3.8f, -2.0f, -12.3f },
		{ 2.4f, -0.4f, -3.5f  },
		{-1.7f,  3.0f, -7.5f  },
		{ 1.3f, -2.0f, -2.5f  },
		{ 1.5f,  2.0f, -2.5f  },
		{ 1.5f,  0.2f, -1.5f  },
		{-1.3f,  1.0f, -1.5f  }
	};

	vec3 camera_pos = {0.0f, 0.0f, 3.0f},
	     camera_front = {0.0f, 0.0f, -1.0f};

	float last_time = 0;

	// Render loop
	// -----------
	while (!window_should_close()) {
		// Clear buffers
		// -------------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Process input
		// -------------
		float current_time = glfwGetTime(),
		      camera_speed = 2.5f * (current_time - last_time);
		last_time = current_time;

		if (PRESSED(GLFW_KEY_W)) {
			vec3 buf;
			glm_vec3_mul(VEC3S(camera_speed), camera_front, buf);
			glm_vec3_add(camera_pos, buf, camera_pos);
		}
		if (PRESSED(GLFW_KEY_S)) {
			vec3 buf;
			glm_vec3_mul(VEC3S(camera_speed), camera_front, buf);
			glm_vec3_sub(camera_pos, buf, camera_pos);
		}
		if (PRESSED(GLFW_KEY_A)) {
			vec4 camera_right_axis;
			glm_vec3_cross(camera_front, GLM_YUP, camera_right_axis);
			float magnitude = glm_vec3_norm(camera_right_axis) * camera_speed;
			glm_vec3_sub(camera_pos, VEC3(magnitude, 0, 0), camera_pos);
		}
		if (PRESSED(GLFW_KEY_D)) {
			vec4 camera_right_axis;
			glm_vec3_cross(camera_front, GLM_YUP, camera_right_axis);
			float magnitude = glm_vec3_norm(camera_right_axis) * camera_speed;
			glm_vec3_add(camera_pos, VEC3(magnitude, 0, 0), camera_pos);
		}

		// Gen view and projection matrices
		// --------------------------------
		mat4 view;
		vec3 camera_center;
		glm_vec3_add(camera_pos, camera_front, camera_center);
		glm_lookat(camera_pos, camera_center, GLM_YUP, view);

		mat4 projection;
		glm_perspective(RAD(65.0f), (float)window_width/(float)window_height,
		                0.1f, 100.0f, projection);

		setu_mat4(shader_program, "view", view);
		setu_mat4(shader_program, "projection", projection);

		// Draw cubes
		// ----------
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		for (int i = 0; i < 10; i++) {
			mat4 model = GLM_MAT4_IDENTITY_INIT;
			glm_translate(model, cube_positions[i]);
			glm_rotate(model, glfwGetTime(), VEC3(1.0f, 0.5f, 0.25f));

			setu_mat4(shader_program, "model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		window_loop();
	}

	// Terminate
	// ---------
	glfwTerminate();
	return 0;
}
