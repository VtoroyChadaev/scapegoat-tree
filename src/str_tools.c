#include <stdio.h>
#include <stdlib.h>
#include "str_tools.h"
#include "tree.h"
#define CTRL_P 16

char *fstr_read(FILE *fp, int *status) {
  char buf[50] = {'\0'};
  size_t len = 0, b_len = 0;
  size_t index = 0;
  char *str = (char *)calloc(len + 1, sizeof(char));

  if (!str) {
    *status = ALLOC_ERR;
    return NULL;
  }

  int res = fscanf(fp, "%50[^\n]s", buf);

  while (res > 0) {

    if (*buf == CTRL_P) {
      free(str);
      *status = EOF;
      fscanf(fp, "%*c");
      return NULL;
    }

    b_len = str_len(buf);
    len += b_len;
    str = (char *)realloc(str, sizeof(char) * (len + 1));
    str[len] = '\0';
    mem_copy(str + index, buf, b_len);
    index += b_len;
    res = fscanf(fp, "%50[^\n]s", buf);
  }

  if (res == EOF) {
    free(str);
    *status = EOF;
    fscanf(fp, "%*c");
    return NULL;
  }

  fscanf(fp, "%*c");
  *status = OK;
  return str;
}

void mem_copy(void *dest, const void *src, size_t n) {
  char *destination = (char *)dest;
  const char *source = (char *)src;

  for (size_t i = 0; i < n; i++) {
    *(destination + i) = *(source + i);
  }
}

size_t str_len(const char *str) {
  size_t len = 0;

  while (*str) {
    ++len;
    ++str;
  }

  return len;
}
