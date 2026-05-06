#ifndef STR_TOOLS_H
#define STR_TOOLS_H

char *fstr_read(FILE *fp, int *status);

size_t str_len(const char *str);

void mem_copy(void *dest, const void *src, size_t n);

#endif
