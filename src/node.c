#include <stdlib.h>
#include <stdio.h>
#include "node.h"
#include "data.h"

Node *node_dup(const Node *node) {
  Node *new_node = node_new();

  if (!new_node) {
    return NULL;
  }

  new_node->key = node->key;

  new_node->data = data_dup(node->data);

  if (node->data && !new_node->data) {
    free(new_node);
    return NULL;
  }

  return new_node;
}

Node *node_new() {
  Node *node = (Node *)calloc(1, sizeof(Node));

  if (!node) {
    return NULL;
  }

  node->keys_amount = 1;
  return node;
}

int node_cmp(const Node *node1, const Node *node2) {
  if (node1->key < node2->key) {
    return -1;
  }

  if (node1->key == node2->key) {
    return 0;
  }

  return 1;
}

void node_print(const Node *node) {
  if (!node) {
    printf("null");
    return;
  }

  printf("(%zu", node->key);

  if (node->keys_amount > 1) {
    printf("[%zu keys]", node->keys_amount);
  }

  data_print(node->data);
}

void node_free(Node *node) {
  if (!node) {
    return;
  }

  if (node->data) {
    data_free(node->data);
  }

  free(node);
}


