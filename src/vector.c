#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "vector.h"
#include "node.h"

int vector_append(Vector *vector, Node *current_node) {
  ++(vector->len);
  vector->nodes = realloc(vector->nodes, sizeof(Node *) * vector->len);

  if (!vector->nodes) {
    vector_free(vector);
    return ALLOC_ERR;
  }

  vector->nodes[vector->len - 1] = current_node;
  return OK;
}

Vector *vector_new() {
  Vector *vector = calloc(1, sizeof(Vector));

  if (!vector) {
    return NULL;
  }

  return vector;
}

int vector_remove(Vector *vector, size_t number) {
  for (size_t i = number; i < vector->len - 1; ++i) {
    vector->nodes[i] = vector->nodes[i + 1];
  }

  (vector->len)--;

  if (vector->len > 0) {
    vector->nodes = realloc(vector->nodes, (vector->len) * sizeof(size_t));
  }

  return OK;
}

void vector_free(Vector *vector) {
  if (!vector) {
    return;
  }

  if (!vector->nodes) {
    free(vector);
    return;
  }

  free(vector->nodes);
  free(vector);
}

void vector_print(Vector *vector) {
  if (!vector) {
    return;
  }

  if (!vector->nodes) {
    return;
  }

  for (size_t i = 0; i < vector->len; ++i) {
    node_print(vector->nodes[i]);

    if (i != vector->len - 1) {
      printf(" ");
    }
  }

  printf("\n");
}


