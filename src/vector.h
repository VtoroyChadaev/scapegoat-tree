#ifndef VECTOR_H
#define VECTOR_H

struct Node;

typedef struct Vector {
  struct Node **nodes;
  size_t len;
} Vector;

Vector *vector_new();

int vector_append(Vector *vector, struct Node *node);

int vector_remove(Vector *vector, size_t index);

void vector_print(Vector *vector);

void vector_free(Vector *vector);

#endif

