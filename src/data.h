#ifndef DATA_H
#define DATA_H

typedef struct Data {
  size_t *values;
  size_t len;
} Data;

Data *data_new();

Data *data_dup(const Data *data);

int data_append(Data **data, size_t info);

void data_print(const Data *data);

void data_free(Data *data);


#endif

