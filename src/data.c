#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "data.h"

Data *data_new() {
  Data *data = (Data *)calloc(1, sizeof(Data));
  return data;
}

Data *data_dup(const Data *data) {
  if (!data) {
    return NULL;
  }

  Data *new_data = data_new();

  if (!new_data) {
    return NULL;
  }

  new_data->len = data->len;

  if (!new_data->values) {
    return new_data;
  }

  new_data->values = calloc(new_data->len, sizeof(size_t));

  for (size_t i = 0; i < new_data->len; ++i) {
    new_data->values[i] = data->values[i];
  }

  return new_data;
}

int data_append(Data **data, size_t info) {
  if (!(*data)) {
    *data = data_new();

    if (!(*data)) {
      return ALLOC_ERR;
    }
  }

  ((*data)->len)++;
  (*data)->values = realloc((*data)->values, sizeof(size_t) * ((*data)->len));

  if (!(*data)->values) {
    return ALLOC_ERR;
  }

  (*data)->values[(*data)->len - 1] = info;
  return OK;
}

void data_print(const Data *data) {
  if (!data) {
    printf(")");
    return;
  }

  if (data->len > 0) {
    printf("; ");
  }

  for (size_t i = 0; i < data->len; ++i) {
    printf("%zu", data->values[i]);

    if (i != data->len - 1) {
      printf(", ");
    }
  }

  printf(")");
}

void data_free(Data *data) {
  if (!data) {
    return;
  }

  if (data->values) {
    free(data->values);
  }

  free(data);
}


