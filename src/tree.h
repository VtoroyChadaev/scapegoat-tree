#ifndef TREE_H
#define TREE_H

typedef enum {
  OK = 0,
  NO_TREE,
  EMPTY,
  NO_KEY,
  ALLOC_ERR,
  OPEN_ERR,
  EMPTY_LINE,
  NOT_NUMBER,
  NOT_ALPHA,
  NOT_EMPTY,
} err_messages;

typedef enum {
  BALANCED = 98,
  UNBALANCED,
} tree_conditions;

struct Vector;
struct Node; 

typedef struct Tree {
  struct Node *root;
  size_t size;
  size_t max_size;
  double alpha;
} Tree;

Tree *tree_new(double alpha);

int is_empty(const Tree *tree);

int insert(Tree *tree, struct Node *node);

int t_remove(Tree *tree, size_t key, size_t number);

void traversal(const struct Node *root_node, ssize_t compare_number);

int in_order_traversal(struct Node *root_node, struct Vector *vector);

struct Vector *find(const Tree *tree, const size_t key);

struct Vector *special_find(Tree *tree, size_t key); 

void format_print(const Tree *tree, const struct Node *root_node);

void graph_print(const Tree *tree);

void tree_free(Tree **tree, struct Node *root_node);

#endif

