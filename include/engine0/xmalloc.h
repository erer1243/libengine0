#ifndef e0_xmalloc_h
#define e0_xmalloc_h

#include <stdlib.h>

/**
 * OOM-checking malloc. Should be used over stdlib malloc. On OOM the program
 * calls die("Out of memory").
 * message and calls die("Out of memory");
 * @param  size Number of bytes to allocate
 * @return      Non-NULL pointer to allocated memory
 */
void *xmalloc(size_t size);

/**
 * OOM-checking realloc. Should be used over stdlib realloc. Acts identically to
 * xmalloc on OOM.
 * @param  ptr  Old data
 * @param  size New size
 * @return      New pointer to data
 */
void *xrealloc(void *ptr, size_t size);

#endif
