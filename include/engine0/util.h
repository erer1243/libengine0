#ifndef e0_util_h
#define e0_util_h

#include <stdbool.h>
#include <glad/glad.h>

/**
 * Prints the given error message to stderr and exits with EXIT_FAILURE.
 * @param message Error message to print
 */
void die(const char *message);

/**
 * Prints like printf to stderr. Equivalent to fprintf(stderr, fmt, ...).
 * @param fmt Format string
 * @param ... Format arguments
 */
// void eprintf(const char *fmt, ...);

/**
 * Prints like puts to stderr. Equivalent to fprintf(stderr, "%s\n", s).
 * @param s String to be printed
 */
// void eputs(const char *s);

/**
 * Duplicates a string onto the heap.
 * @param  s String to be duped
 * @return   The new string
 */
// char *strdup(const char *s);

/**
 * Identical to strdup but with GLchar.
 * @param  s String to be duped
 * @return   The new string
 */
GLchar *strdup_GLchar(const GLchar *s);

/**
 * Duplicate a string onto the heap with printf formatting.
 * @param  fmt Format string
 * @param  ... Format arguments
 * @return     The new string or NULL if allocation failed
 */
// char *strdupf(const char *fmt, ...);

#endif
