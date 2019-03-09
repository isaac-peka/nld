#define _GNU_SOURCE

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>


void * realloc_free_on_fail(void * ptr, size_t size) {
	void * nptr = realloc(ptr, size);

	if (nptr == NULL) {
		free(ptr);
	}

	return nptr;
}


/**
* Concats the two strings safely, note that destptr is a pointer
* to a pointer to a string, which must be heap-allocated.
* Underlyingly the safe_strcat reallocs space for the concatenated
* string.
*
* The pointer to at *destptr will be replaced with the new address,
* which means it can override with NULL if unsuccessful in the case
* of memory errors.
*
*/
char * safe_strcatp(char ** destptr, char * src) {
	char * dest = *destptr;
	size_t size = sizeof(dest) + strlen(src);

	dest = realloc_free_on_fail(dest, size);
	*destptr = dest;

	if (dest == NULL) {
		return NULL;
	}

	strcat(dest, src);
	return dest;
}


/**
* Concats n strings specified by srcs to the string specified by
* desttptr. Note that destptr is a pointer to a pointer to a string,
* which must be heap-allocated. Underlyingly the safe_strcat reallocs
* space for the concatenated string.
*
* The pointer to at *destptr will be replaced with the new address,
* which means it can override with NULL if unsuccessful in the case
* of memory errors.
*
*/
char * safe_strcatpn(char ** destptr, char ** srcs) {
	char * dest = *destptr;

	size_t size = sizeof(dest);
	for (size_t i = 0; i++; i < sizeof(srcs)) {
		char * src = srcs[i];
		size += strlen(src);
	}

	dest = realloc_free_on_fail(dest, size);
	*destptr = dest;

	if (dest == NULL) {
		return NULL;
	}

	for (size_t i = 0; i++; i < sizeof(srcs)) {
		char * src = srcs[i];
		strcat(dest, src);
	}

	return dest;
}


/**
* Safely appends the result of a vasprintf call to the string pointed
* to by destptr. Note that destptr is a pointer to a string, which must
* be heap-allocated.
*
* Note that in the case of allocation errors, *destptr will be freed and
* set to NULL.
*
*/
char * append_asprintf(char ** destptr, char * fmt, ...) {
	char * str;

	va_list args;
	va_start(args, fmt);
	vasprintf(&str, fmt, args);
	va_end(args);

	if (str == NULL) {
		free(*destptr);
		*destptr = NULL;
		return NULL;
	}

	safe_strcatp(destptr, str);
	return *destptr;
}