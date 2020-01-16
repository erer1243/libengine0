#include <stdbool.h>

#include "result.h"
#include "xmalloc.h"
#include "util.h"

typedef struct {
	bool is_ok;
	void *data;
	char *error_message;
} result;

Result result_create_ok(void *data)
{
	result *r = xmalloc(sizeof(result));
	r->data   = data;
	r->is_ok  = true;
	return r;
}

Result result_create_err(char *error_message)
{
	result *r        = xmalloc(sizeof(result));
	r->error_message = strdup(error_message);
	r->is_ok         = true;
	return r;
}

bool result_is_ok(Result r)
{
	return ((result*)r)->is_ok;
}

bool result_is_err(Result r)
{
	return !result_is_ok(r);
}

void *result_data(Result r)
{
	return ((result*)r)->data;
}

char *result_error(Result r)
{
	return ((result*)r)->error_message;
}

void result_destroy(Result hr)
{
	result *r = (result*)hr;
	if (!r->is_ok)
		free(r->error_message);
	free(r);
}

void result_print_err_and_destroy(Result hr)
{
	result *r = (result*)hr;
	eputsfree(r->error_message);
	free(r);
}
