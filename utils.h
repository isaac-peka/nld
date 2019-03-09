char * safe_strcatp(char ** destptr, char * src);
char * safe_strcatpn(char ** destptr, char ** srcs);
char * append_asprintf(char ** destptr, char * fmt, ...);
void * realloc_free_on_fail(void * ptr, size_t size);