#include <stdbool.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine0/engine0.h"
#include "engine0/util.h"
#include "engine0/global_object_array.h"

/**
 * Global state accessible by library users. Fields defined in engine0.h .
 */
struct e0_globals e0_globals;

static struct {
	GLFWwindow *window;
} internal;

/**
 * Function for updating OpenGL's viewport and window_width/height variables.
 * This function is passed to glfwSetFramebufferSizeCallback in window_init.
 */
static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	e0_globals.window_width  = width;
	e0_globals.window_height = height;
	glViewport(0, 0, width, height);
}

bool e0_init(unsigned int win_w, unsigned int win_h, const char *title,
             bool window_visible)
{
	// Initialize GLFW
	glfwInit();

	// Create global window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, window_visible ? GLFW_TRUE : GLFW_FALSE);

	e0_globals.window_width  = win_w;
	e0_globals.window_height = win_w;
	internal.window = glfwCreateWindow(win_w, win_h, title, NULL, NULL);

	if (!internal.window) {
		glfwTerminate();
		e0_setError("glfwCreateWindow failed");
		return false;
	}

	// Link OpenGL functions
	glfwMakeContextCurrent(internal.window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		e0_setError("gladLoadGLLoader failed");
		return false;
	}

	// Misc
	glViewport(0, 0, e0_globals.window_width, e0_globals.window_height);
	glfwSetFramebufferSizeCallback(internal.window, framebuffer_size_callback);

	// Vsync on by default
	glfwSwapInterval(1);

	// Initialize global object array
	initialize_GOA();

	return true;
}

void e0_terminate()
{
	destroy_GOA();
	glfwTerminate();
}

void e0_showWindow()
{
	glfwShowWindow(internal.window);
}

void e0_draw()
{
	glfwSwapBuffers(internal.window);
	glfwPollEvents();
}

bool e0_windowShouldClose()
{
	return glfwWindowShouldClose(internal.window);
}

bool e0_keyPressed(int keycode)
{
	return glfwGetKey(internal.window, keycode) == GLFW_PRESS;
}

void e0_setVsync(bool vsync_on)
{
	glfwSwapInterval(vsync_on ? 1 : 0);
}

double e0_getTime()
{
	return glfwGetTime();
}
