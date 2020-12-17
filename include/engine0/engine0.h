#ifndef e0_engine0_h
#define e0_engine0_h

#include <stdbool.h>
#include <glad/glad.h>
#include <cglm/cglm.h>

#include "engine0/error.h"
#include "engine0/mesh.h"
#include "engine0/shader.h"
#include "engine0/texture.h"

/* Excluded: util.h xmalloc.h global_object_array.h */

/**
 * Global variables meant to be read only. Updated by internal processes,
 * never read from by e0.
 */
extern struct e0_globals {
	unsigned int window_width, window_height;
} e0_globals;

/**
 * Initializes a window and OpenGL context for it with given parameters.
 * @param  window_width   Width of window
 * @param  window_height  Height of window
 * @param  window_title   Title of window
 * @param  window_visible If window is visible immediately (if false, use
 *                        e0_show_window() to make it visible)
 * @return                true on success, false on error
 */
bool e0_init(unsigned int window_width, unsigned int window_height,
             const char *window_title, bool window_visible);

/**
 * Close window and cleanup libraries. Does not free any objects.
 */
void e0_terminate();

/**
 * Show window if it was previously hidden.
 */
void e0_showWindow();

/**
 * Must be run every single frame. Swaps frame buffer and polls for window
 * events
 */
void e0_draw();

/**
 * Checks if the window should close (i.e. it has been X-ed out).
 * @return true if the window should close, false if not
 */
bool e0_windowShouldClose();

/**
 * Check if a key is being pressed for the current frame
 * @param  keycode key to check
 * @return         true if pressed, false if not
 */
bool e0_keyPressed(int keycode);

/**
 * Sets whether vsync should be enabled or not. Default is true.
 * @param vsync_on true to enable, false to disable
 */
void e0_setVsync(bool vsync_on);

/**
 * Get the number of seconds since library was initialized.
 * @return Seconds since library was initialzed
 */
double e0_getTime();


#endif
