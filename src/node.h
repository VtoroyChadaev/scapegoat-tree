#ifndef NODE_H
#define NODE_H

struct Data;

typedef struct Node {
  size_t key;
  size_t keys_amount;
  struct Node *parent;
  struct Node *left;
  struct Node *right;
  struct Data *data;
} Node;

Node *node_new();

Node *node_dup(const Node *node);

int node_cmp(const Node *node1, const Node *node2);

void node_print(const Node *node);

void node_free(Node *node);

#endif
