#ifndef e0_error_h
#define e0_error_h

/**
 * Set the global error string. Formatting like printf is allowed.
 * @param fmt Format string
 * @param ... Format parameters
 */
void e0_setError(const char *fmt, ...);

/**
 * Get the global error string.
 * @return The global error string
 */
const char *e0_getError();

#endif
