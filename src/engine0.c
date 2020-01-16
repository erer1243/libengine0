#include <stdbool.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine0/util.h"
#include "engine0/error.h"

unsigned int window_width = 0, window_height = 0;
static GLFWwindow *window;

/**
 * Function for updating OpenGL's viewport and window_width/height variables.
 * This function is passed to glfwSetFramebufferSizeCallback in window_init.
 */
static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	window_width  = width;
	window_height = height;
	glViewport(0, 0, width, height);
}

bool e0_init(unsigned int win_w, unsigned int win_h, const char *title,
             bool window_visible)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, window_visible ? GLFW_TRUE : GLFW_FALSE);

	window_width  = win_w;
	window_height = win_w;
	window        = glfwCreateWindow(win_w, win_h, title, NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		e0_setError("glfwCreateWindow failed");
		return false;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		e0_setError("gladLoadGLLoader failed");
		return false;
	}

	glViewport(0, 0, window_width, window_height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSwapInterval(1);

	return true;
}

void e0_terminate()
{
	glfwTerminate();
}

void e0_showWindow()
{
	glfwShowWindow(window);
}

void e0_loopWindow()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool e0_windowShouldClose()
{
	return glfwWindowShouldClose(window);
}

bool e0_keyPressed(int keycode)
{
	return glfwGetKey(window, keycode) == GLFW_PRESS;
}

void e0_setVsync(bool vsync_on)
{
	glfwSwapInterval(vsync_on ? 1 : 0);
}

double e0_getTime()
{
	return glfwGetTime();
}
