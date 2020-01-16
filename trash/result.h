#ifndef result_h
#define result_h

#include <stdbool.h>

typedef void *Result;

/**
 * Creates an OK result. Stores data pointer, does not copy any data into itself.
 * @param  data Pointer to data to store in result
 * @return      An OK result with the given data
 */
Result result_create_ok(void *data);

/**
 * Creates an error result. Copies the given string, which is freed by
 * result_destroy().
 * @param  error_message Error message to store in result
 * @return               An error result with the given message
 */
Result result_create_err(char *error_message);
bool result_is_ok(Result r);
bool result_is_err(Result r);
void *result_data(Result r);
char *result_error(Result r);
void result_destroy(Result r);
void result_print_err_and_destroy(Result r)

#endif
