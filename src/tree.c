#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>
#include "tree.h"
#include "vector.h"
#include "node.h"
#include "data.h"

#define NRM "\x1B[0m"
#define RED "\x1B[31m"
#define BLU "\x1B[34m"

Node *find_parent(Node *root_node, size_t key);

Node *find_max_left(Node *root_node);

int check_insert_balance(double alpha, size_t parent_size, size_t new_node_size,
                         size_t brother_size);

Node *find_goat(double alpha, Node *new_node);

size_t size_of_node(Node *node);

Node *build_balanced_tree(Vector *vector, ssize_t start, ssize_t end);

int restore_balance(Node *goat_node, Tree *tree);

int calc_kids(const Node *node);

int check_remove_balance(double alpha, size_t size, size_t max_size);

void remove_leave_node(Node *garbage_node, Tree *tree);

void remove_interim_node(Node *garbage_node, Tree *tree);

void remove_root_node(Node *garbage_node, Tree *tree);

void swap_nodes(Node *garbage_node, Node *swap_node, Tree *tree);

int remove_info(Node *node, size_t number);

size_t get_order(size_t number);

size_t calc_diff(size_t, size_t);

int find_min_diff(Node *root_node, Vector *vector, size_t *min_diff,
                  size_t key);

size_t max(size_t num1, size_t num2);

Tree *tree_new(double alpha) {
  Tree *tree = (Tree *)calloc(1, sizeof(Tree));

  if (!tree) {
    return NULL;
  }

  tree->alpha = alpha;
  return tree;
}

int is_empty(const Tree *tree) {
  if (!tree) {
    return NO_TREE;
  }

  if (tree->root == NULL) {
    return EMPTY;
  }

  return NOT_EMPTY;
}

int insert(Tree *tree, Node *new_node) {
  int status = is_empty(tree);

  if (status == NO_TREE) {
    return status;
  }

  if (status == EMPTY) {
    tree->root = new_node;
    (tree->size)++;
    tree->max_size = tree->size;
    return OK;
  }

  Node *parent_node = find_parent(tree->root, new_node->key);
  new_node->parent = parent_node;

  int res = node_cmp(parent_node, new_node);

  if (res > 0) {
    parent_node->left = new_node;
  } else if (res < 0) {
    parent_node->right = new_node;
  } else {
    if (new_node->data) {
      status = data_append(&(parent_node->data), new_node->data->values[0]);
    }

    if (status == ALLOC_ERR) {
      node_free(new_node);
      return ALLOC_ERR;
    }

    (parent_node->keys_amount)++;

    node_free(new_node);
    return OK;
  }

  (tree->size)++;
  tree->max_size = max(tree->max_size, tree->size);

  Node *goat_node = find_goat(tree->alpha, new_node);

  if (!goat_node) {
    return OK;
  }

  if (goat_node->key == new_node->key) {
    return OK;
  }

  status = restore_balance(goat_node, tree);
  return status;
}

Node *find_parent(Node *root_node, size_t key) {
  Node *current_node = root_node;
  Node *parent_node = NULL;

  while (current_node) {
    parent_node = current_node;

    if (current_node->key > key) {
      current_node = current_node->left;
      continue;
    }

    if (current_node->key == key) {
      return current_node;
    }

    current_node = current_node->right;
  }

  return parent_node;
}

Node *find_goat(double alpha, Node *node) {
  size_t node_size = 1, parent_size = 0, brother_size = 0;

  while (node->parent) {

    if (parent_size != 0) {
      node_size = parent_size;
    }

    parent_size = node_size + 1;

    if (node->parent->left == node) {
      if (node->parent->right) {
        brother_size = size_of_node(node->parent->right);
        parent_size += brother_size;
      }

    } else if (node->parent->left) {
      brother_size = size_of_node(node->parent->left);
      parent_size += brother_size;
    }

    int flag = check_insert_balance(alpha, parent_size, node_size, brother_size);

    if (flag == UNBALANCED) {
      break;
    }

    node = node->parent;
  }

  if (!node->parent) {
    int flag = check_insert_balance(alpha, parent_size, node_size, brother_size);

    if (flag == UNBALANCED) {
      return node;
    }

  }

  return node->parent;
}

int check_insert_balance(double alpha, size_t parent_size, size_t node_size, 
                         size_t brother_size) {
  if (brother_size > alpha * parent_size ||
      node_size > alpha * parent_size) {
    return UNBALANCED;
  }

  return BALANCED;
}

size_t size_of_node(Node *node) {
  if (!node) {
    return 0;
  }

  size_t size = 1;

  size += size_of_node(node->left);
  size += size_of_node(node->right);
  return size;
}

int restore_balance(Node *goat_node, Tree *tree) {
  Vector *vector = vector_new();

  if (!vector) {
    return ALLOC_ERR;
  }

  Node *goat_parent = goat_node->parent;

  int status = in_order_traversal(goat_node, vector);

  if (status == ALLOC_ERR) {
    return status;
  }

  Node *balanced_subtree_root = build_balanced_tree(vector, 0, vector->len - 1);
  balanced_subtree_root->parent = goat_parent;

  vector_free(vector);

  if (!goat_parent) {
    tree->root = balanced_subtree_root;
    return OK;
  }

  if (goat_parent->left == goat_node) {
    goat_parent->left = balanced_subtree_root;
  } else {
    goat_parent->right = balanced_subtree_root;
  }

  return OK;
}

Node *build_balanced_tree(Vector *vector, ssize_t start, ssize_t end) {
  if (start > end) {
    return NULL;
  }

  size_t middle = (start + end) / 2;
  Node *root_node = vector->nodes[middle];

  root_node->left = build_balanced_tree(vector, start, middle - 1);

  if (root_node->left) {
    root_node->left->parent = root_node;
  }

  root_node->right = build_balanced_tree(vector, middle + 1, end);

  if (root_node->right) {
    root_node->right->parent = root_node;
  }

  return root_node;
}

int t_remove(Tree *tree, size_t key, size_t number) {
  int status = is_empty(tree);

  if (status != NOT_EMPTY) {
    return status;
  }

  Node *garbage_node = tree->root;

  while (garbage_node) {
    if (garbage_node->key == key) {
      break;
    }

    if (garbage_node->key > key) {
      garbage_node = garbage_node->left;
      continue;
    }

    garbage_node = garbage_node->right;
  }

  if (!garbage_node) {
    return NO_KEY;
  }

  if (garbage_node->keys_amount > 1) {
    status = remove_info(garbage_node, number);
    return status;
  }

  if (number > 1) {
    return NO_KEY;
  }

  int kids_amount = calc_kids(garbage_node);

  switch (kids_amount) {
    case 0:
      remove_leave_node(garbage_node, tree);
      break;
    case 1:
      remove_interim_node(garbage_node, tree);
      break;
    case 2:
      remove_root_node(garbage_node, tree);
  }

  (tree->size)--;
  
  int flag = check_remove_balance(tree->alpha, tree->size, tree->max_size);

  if (flag == UNBALANCED) {
    restore_balance(tree->root, tree);
    tree->max_size = tree->size;
  }

  return OK;
}

int calc_kids(const Node *node) {
  if (node->left && node->right) {
    return 2;
  }

  if (!node->left && !node->right) {
    return 0;
  }

  return 1;
}

int check_remove_balance(double alpha, size_t size, size_t max_size) {
  if (size < alpha * max_size) {
    return UNBALANCED;
  }

  return BALANCED;
}

int remove_info(Node *garbage_node, size_t number) {
  if (number > garbage_node->keys_amount || number == 0) {
    return NO_KEY;
  }

  if (!garbage_node->data) {
    (garbage_node->keys_amount)--;
    return OK;
  }

  size_t empty_amount = calc_diff(garbage_node->keys_amount, garbage_node->data->len);

  if (empty_amount > 0) {
    size_t exist_amount = garbage_node->data->len;

    if (empty_amount >= exist_amount) {

      if (number <= empty_amount) {
        (garbage_node->keys_amount)--;
        return OK;
      } else {
        number -= empty_amount;
      }
    } else {

      if (number <= exist_amount) {
        (garbage_node->keys_amount)--;
        return OK;
      } else {
        number -= empty_amount;
      }
    }
  }

  (number)--;
  Data *vector = garbage_node->data;

  for (size_t i = number; i < vector->len - 1; ++i) {
    vector->values[i] = vector->values[i + 1];
  }

  (vector->len)--;
  (garbage_node->keys_amount)--;

  if (garbage_node->data->len > 0) {
    vector->values = realloc(vector->values, (vector->len) * sizeof(size_t));
  }

  return OK;
}

void remove_leave_node(Node *garbage_node, Tree *tree) {
  Node *parent_node = garbage_node->parent;

  if (!parent_node) {
    tree->root = NULL;
    node_free(garbage_node);
    return;
  }

  if (parent_node->left == garbage_node) {
    parent_node->left = NULL;
  } else {
    parent_node->right = NULL;
  }

  node_free(garbage_node);
}

void remove_interim_node(Node *garbage_node, Tree *tree) {
  Node *next_node = NULL;

  if (!garbage_node->left) {
    next_node = garbage_node->right;
  } else {
    next_node = garbage_node->left;
  }

  if (!garbage_node->parent) {
    tree->root = next_node;
    node_free(garbage_node);
    return;
  }

  if (garbage_node->parent->left == garbage_node) {
    garbage_node->parent->left = next_node;
  } else {
    garbage_node->parent->right = next_node;
  }

  next_node->parent = garbage_node->parent;
  node_free(garbage_node);
}

void remove_root_node(Node *garbage_node, Tree *tree) {
  Node *swap_node = find_max_left(garbage_node);

  if (!swap_node) {
    return;
  }

  swap_nodes(garbage_node, swap_node, tree);
  node_free(garbage_node);
}

Node *find_max_left(Node *root_node) {
  Node *max_node = root_node->left;

  if (!max_node) {
    return NULL;
  }

  while (max_node->right) {
    max_node = max_node->right;
  }

  return max_node;
}

void swap_nodes(Node *garbage_node, Node *swap_node, Tree *tree) {
  if (swap_node == garbage_node->left) {
    swap_node->right = garbage_node->right;
    swap_node->right->parent = swap_node;

    swap_node->parent = garbage_node->parent;

    if (!garbage_node->parent) {
      tree->root = swap_node;
      return;
    }

    if (garbage_node->parent->left == garbage_node) {
      garbage_node->parent->left = swap_node;
    } else {
      garbage_node->parent->right = swap_node;
    }

    return;
  }

  // case node(-s) between garbage_node and swap_node

  if (swap_node->left) {
    swap_node->left->parent = swap_node->parent;
  }

  swap_node->parent->right = swap_node->left;

  swap_node->right = garbage_node->right;
  swap_node->right->parent = swap_node;

  swap_node->left = garbage_node->left;
  swap_node->left->parent = swap_node;

  swap_node->parent = garbage_node->parent;

  if (!garbage_node->parent) {
    tree->root = swap_node;
    return;
  }

  if (garbage_node->parent->left == garbage_node) {
    garbage_node->parent->left = swap_node;
  } else {
    garbage_node->parent->right = swap_node;
  }
}

int in_order_traversal(Node *root_node, Vector *vector) {
  if (!root_node) {
    return OK;
  }

  int status = in_order_traversal(root_node->left, vector);

  if (status == ALLOC_ERR) {
    return status;
  }

  status = vector_append(vector, root_node);

  if (status == ALLOC_ERR) {
    return status;
  }

  status = in_order_traversal(root_node->right, vector);

  if (status == ALLOC_ERR) {
    return status;
  }

  return OK;
}

void traversal(const Node *root_node, const ssize_t compare_number) {
  if (!root_node) {
    return;
  }

  if (root_node->key - compare_number > 0) {
    node_print(root_node);
    printf(" ");
  }

  traversal(root_node->left, compare_number);
  traversal(root_node->right, compare_number);
}

Vector *find(const Tree *tree, const size_t key) {
  int status = is_empty(tree);

  if (status != NOT_EMPTY) {
    return NULL;
  }

  Node *current_node = tree->root;
  Vector *vector = NULL;

  while (current_node) {
    if (key < current_node->key) {
      current_node = current_node->left;
      continue;
    }

    if (key == current_node->key) {
      if (!vector) {
        vector = vector_new();

        if (!vector) {
          return NULL;
        }
      }

      int status = vector_append(vector, current_node);

      if (status == ALLOC_ERR) {
        return NULL;
      }
    }

    current_node = current_node->right;
  }

  return vector;
}

Vector *special_find(Tree *tree, size_t key) {
  int status = is_empty(tree);

  if (status != NOT_EMPTY) {
    return NULL;
  }

  Vector *vector = vector_new();

  if (!vector) {
    return NULL;
  }

  size_t min_diff = SIZE_MAX;

  status = find_min_diff(tree->root, vector, &min_diff, key);

  if (status == ALLOC_ERR) {
    return NULL;
  }

  if (!vector->len) {
    vector_free(vector);
    return NULL;
  }

  for (size_t i = 0; i < vector->len; ++i) {
    size_t diff = calc_diff(vector->nodes[i]->key, key);

    if (diff != min_diff) {
      vector_remove(vector, i);
      i--;
    }
  }

  return vector;
}

int find_min_diff(Node *root_node, Vector *vector, size_t *min_diff,
                  size_t key) {
  if (!root_node) {
    return OK;
  }

  int status = find_min_diff(root_node->left, vector, min_diff, key);

  if (status == ALLOC_ERR) {
    return status;
  }

  status = vector_append(vector, root_node);
  size_t diff = calc_diff(root_node->key, key);

  if (diff < *min_diff && diff != 0) {
    *min_diff = diff;
  }

  if (status == ALLOC_ERR) {
    return status;
  }

  status = find_min_diff(root_node->right, vector, min_diff, key);

  if (status == ALLOC_ERR) {
    return status;
  }

  return OK;
}

size_t calc_diff(size_t num1, size_t num2) {
  size_t diff = 0;

  if (num1 >= num2) {
    diff = num1 - num2;
  } else {
    diff = num2 - num1;
  }

  return diff;
}

void format_print(const Tree *tree, const Node *node_to_print) {

  if (!node_to_print) {
    return;
  }

  if (!node_to_print->parent) {
    printf(BLU);
    printf(".\n└──");
    node_print(node_to_print);
    printf(NRM);
    printf("\n   ");
    format_print(tree, node_to_print->right);
    format_print(tree, node_to_print->left);
    return;
  }

  Node *parent_node = tree->root;

  while (parent_node != node_to_print->parent) {
    int res = node_cmp(parent_node, node_to_print);

    if (res > 0) {
      parent_node = parent_node->left;
      printf("   ");
    } else {
      if (parent_node->left) {
        printf("│  ");
      } else {
        printf("   ");
      }
      parent_node = parent_node->right;
    }
  }

  int flag = node_to_print->parent->left == node_to_print;

  if (flag || !node_to_print->parent->left) {
    printf("└──");
  } else {
    printf("├──");
  }

  if (node_to_print->parent->right == node_to_print) {
    printf(RED);
  }

  node_print(node_to_print);
  printf(NRM);
  printf("\n   ");

  format_print(tree, node_to_print->right);
  format_print(tree, node_to_print->left);
}

void add_node(Agraph_t *graph, Node *current_node,
              Agnode_t *parent_graph_node) {

  if (!current_node) {
    return;
  }

  size_t number_order = get_order(current_node->key);
  char *label = calloc(number_order + 1, sizeof(char));

  if (!label) {
    return;
  }

  snprintf(label, number_order + 1, "%zu", current_node->key);

  Agnode_t *graph_node = agnode(graph, label, 1);

  agsafeset(graph_node, "label", label, "");
  agsafeset(graph_node, "color", "black", "");
  agsafeset(graph_node, "fontcolor", "black", "");

  free(label);

  if (parent_graph_node) {
    Agedge_t *edge1 = agedge(graph, parent_graph_node, graph_node, NULL, 1);
    Agedge_t *edge2 = agedge(graph, graph_node, parent_graph_node, NULL, 1);
    agsafeset(edge1, "color", "lightgray", "");
    agsafeset(edge2, "color", "lightgray", "");
  }

  add_node(graph, current_node->left, graph_node);
  add_node(graph, current_node->right, graph_node);
}

void graph_print(const Tree *tree) {
  Agraph_t *graph = agopen("g", Agdirected, NULL);

  if (!graph) {
    return;
  }

  GVC_t *graph_context = gvContext();

  if (!graph_context) {
    agclose(graph);
    return;
  }

  add_node(graph, tree->root, NULL);
  gvLayout(graph_context, graph, "dot");
  gvRenderFilename(graph_context, graph, "png", "tree.png");
  gvFreeLayout(graph_context, graph);
  gvFreeContext(graph_context);
  agclose(graph);
}

size_t get_order(size_t number) {
  size_t order = 0;

  if (number == 0) {
    return 1;
  }

  while (number) {
    order += 1;
    number /= 10;
  }

  return order;
}

void tree_free(Tree **tree, Node *root_node) {
  if (*tree) {
    free(*tree);
    *tree = NULL;
  }

  if (!root_node) {
    return;
  }

  Node *left_node = root_node->left;
  Node *right_node = root_node->right;

  node_free(root_node);

  tree_free(tree, left_node);
  tree_free(tree, right_node);
}

size_t max(size_t num1, size_t num2) { return (num1 > num2) ? num1 : num2; }
