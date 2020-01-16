#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "engine0/error.h"
#include "engine0/util.h"
#include "engine0/xmalloc.h"

static char *global_error = NULL;

void e0_setError(const char *fmt, ...)
{
	if (global_error)
		free(global_error);

	va_list args1, args2;
	va_start(args1, fmt);
	va_copy(args2, args1);

	int len = vsnprintf(NULL, 0, fmt, args1) + 1;
	char *new_error = xmalloc(len * sizeof(char));
	vsprintf(new_error, fmt, args2);

	va_end(args1);
	va_end(args2);

	global_error = new_error;
}

const char *e0_getError()
{
	return global_error;
}
