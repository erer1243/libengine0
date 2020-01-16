#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "engine0/util.h"
#include "engine0/xmalloc.h"

void die(const char *message)
{
	eputs(message);
	exit(EXIT_FAILURE);
}

void eprintf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
}

void eputs(const char *s)
{
	fprintf(stderr, "%s\n", s);
}

char *strdup(const char *s)
{
	size_t len = strlen(s) + 1;
	char *new = xmalloc(len * sizeof(char));
	memcpy(new, s, len);
	return new;
}

GLchar *strdup_GLchar(const GLchar *s)
{
	size_t len = strlen(s) + 1;
	GLchar *new = xmalloc(len * sizeof(GLchar));
	memcpy(new, s, len);
	return new;
}

char *strdupf(const char *fmt, ...)
{
	va_list args1, args2;
	va_start(args1, fmt);
	va_copy(args2, args1);

	int len = vsnprintf(NULL, 0, fmt, args1) + 1;
	char *new = xmalloc(len * sizeof(char));
	vsprintf(new, fmt, args2);

	va_end(args1);
	va_end(args2);
	return new;
}
