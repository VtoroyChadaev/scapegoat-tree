#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "../src/check_input.h"
#include "../src/tree.h"
#include "../src/node.h"
#include "../src/vector.h"
#include "../src/data.h"

typedef Vector *(*general_func)(Tree *tree, Node *node);

typedef enum {
  CHECK_INS = 0,
  CHECK_FIND,
  CHECK_REMOVE,
  CHECK_SPEC_FIND,
} check_indexs;

Tree **make_rand_trees(size_t size_of_tree, size_t key_len,
                       size_t amount, double alpha);

Vector *make_rand_nodes(size_t key_len, size_t number_of_nodes);

size_t make_rand_key(size_t key_len);

void help_user();

Vector *special_find_general(Tree *tree, Node *node);

Vector *insert_general(Tree *tree, Node *node);

Vector *find_general(Tree *tree, Node *node);

Vector *remove_general(Tree *tree, Node *node);

size_t spow(int base, size_t power);

int main(int argc, char **argv) {
  srand(time(NULL));
  size_t size_of_tree = 100, key_len = 20, trees_amount = 10;
  size_t number_of_nodes = 50;
  int is_converted = 1;
  int index = 0;
  char options[] = "s:n:k:a:A:firSh";
  double alpha = 0.5;
  int res = getopt(argc, argv, options);

  while (res != EOF) {

    switch (res) {
    case 's':
      is_converted *= str_to_sizet(&size_of_tree, optarg);
      break;
    case 'n':
      is_converted *= str_to_sizet(&number_of_nodes, optarg);
      break;
    case 'f':
      index = CHECK_FIND;
      break;
    case 'i':
      index = CHECK_INS;
      break;
    case 'a':
      trees_amount = 0;
      is_converted *= str_to_sizet(&trees_amount, optarg);
      break;
    case 'k':
      is_converted *= str_to_sizet(&key_len, optarg);
      break;
    case 'A':
      alpha = atof(optarg);
      break;
    case 'r':
      index = CHECK_REMOVE;
      break;
    case 'S':
      index = CHECK_SPEC_FIND;
      break;
    case 'h':
      help_user();
      return 1;
    case '?':
      printf("Incorrect argument");
      break;
    }

    res = getopt(argc, argv, options);
  }

  if (!is_converted) {
    printf("Invalid arguments' value(-s)\n");
    return 1;
  }
  
  if (alpha < 0.5 || alpha >= 1) {
    printf("Invalid alpha\n");
    return 1;
  }

  Tree **trees_vector =
      make_rand_trees(size_of_tree, key_len, trees_amount, alpha);

  if (!trees_vector) {
    fprintf(stderr, "ALLOC ERROR\n");
    return 1;
  }

  general_func funcs[] = {insert_general, find_general, remove_general,
                          special_find_general};
  general_func func = funcs[index];

  Vector *nodes_vector = make_rand_nodes(key_len, number_of_nodes);

  if (!nodes_vector) {
    fprintf(stderr, "ALLOC ERROR\n");
    return 1;
  }

  Vector **vectors_to_free = calloc(nodes_vector->len, sizeof(Vector *));

  if (!vectors_to_free) {
    fprintf(stderr, "ALLOC ERROR\n");
    return 1;
  }

  double general_time = 0;

  for (size_t i = 0; i < trees_amount; i++) {
    double time = 0;
    clock_t start, end;
    Tree *tree = trees_vector[i];

    start = clock();
    for (size_t j = 0; j < nodes_vector->len; j++) {
      Node *dup_node = node_dup(nodes_vector->nodes[j]);
      vectors_to_free[j] = (*func)(tree, dup_node);
    }
    end = clock();

    time = (double) 1000.0 * (end - start) / CLOCKS_PER_SEC;
    general_time += time;

    for (size_t k = 0; k < nodes_vector->len; k++) {
      if (vectors_to_free[k]) {
        vector_free(vectors_to_free[k]);
      }
    }
  }

  printf("%.15lf\n", general_time / trees_amount);

  for (size_t i = 0; i < trees_amount; ++i) {
    tree_free(&(trees_vector[i]), (trees_vector[i])->root);
  }

  free(trees_vector);

  for (size_t i = 0; i < number_of_nodes; ++i) {
    node_free(nodes_vector->nodes[i]);
  }

  vector_free(nodes_vector);

  free(vectors_to_free);
  return OK;
}

Tree **make_rand_trees(size_t size_of_tree, size_t key_len,
                       size_t trees_amount, double alpha) {
  Tree **trees_vector = calloc(trees_amount, sizeof(Tree *));

  if (!trees_vector) {
    return NULL;
  }

  for (size_t i = 0; i < trees_amount; i++) {
    trees_vector[i] = tree_new(alpha);

    if (!trees_vector[i]) {
      return NULL;
    }
  }

  // fills trees_vector
  for (size_t i = 0; i < trees_amount; ++i) {
    Tree *tree = trees_vector[i];

    // fills tree
    for (size_t j = 0; j < size_of_tree; ++j) {
      size_t key_base = spow(10, key_len);
      size_t key = rand() % key_base + 1;
      // size_t key = make_rand_key(key_len);

      if (!key) {
        return NULL;
      }

      Node *node = node_new();

      if (!node) {
        return NULL;
      }

      node->key = key;
      insert(tree, node);
    }
  }

  return trees_vector;
}

Vector *make_rand_nodes(size_t key_len, size_t number_of_nodes) {
  Vector *vector = vector_new();

  if (!vector) {
    return NULL;
  }

  vector->nodes = calloc(number_of_nodes, sizeof(Node *));
  vector->len = number_of_nodes;

  if (!vector->nodes) {
    return NULL;
  }

  for (size_t i = 0; i < number_of_nodes; ++i) {
    size_t key_base = spow(10, key_len);
    size_t key = rand() % key_base + 1;
    // size_t key = make_rand_key(key_len);

    if (!key) {
      return NULL;
    }

    vector->nodes[i] = node_new();

    if (!vector->nodes[i]) {
      return NULL;
    }

    vector->nodes[i]->key = key;
  }

  return vector;
}

size_t make_rand_key(size_t key_len) {
  size_t key = 0;

  for (size_t i = 0; i < key_len; ++i) {
    key += (rand() % 10 + 1) * spow(10, i);
  }
  
  return key;
}


size_t spow(int base, size_t power) {
	size_t result = 1;
	for (size_t i = 0; i < power; i++) {
		result = result * base;
	}
	return result;
}


Vector *special_find_general(Tree *tree, Node *node) {
  Vector *vector = special_find(tree, node->key);
  
  if (node) {
    node_free(node);
  }

  return vector;
}

Vector *insert_general(Tree *tree, Node *node) {
  Vector *vector = NULL;
  insert(tree, node);
  return vector;
}

Vector *find_general(Tree *tree, Node *node) {
  Vector *vector = find(tree, node->key);

  if (node) {
    node_free(node);
  }

  return vector;
}

Vector *remove_general(Tree *tree, Node *node) {
  Vector *vector = NULL;
  size_t number = rand() % node->keys_amount + 1;

  t_remove(tree, node->key, number);

  if (node) {
    node_free(node);
  }

  return vector;
}

void help_user() {
  printf(
      "Arguments:\n"
      "\t[-f] to check find.\n"
      "\t[-S] to check special_find\n"
      "\t[-i] to check insert\n"
      "\t[-r] to check remove\n"
      "\t[-a trees_amount] set amount of trees to generate. Default value: 1\n"
      "\t[-s tree_size] to set size of tree. Default value: 100\n"
      "\t[-n number_of_nodes] to set number of nodes to be tested. Default "
      "value: 50\n"
      "\t[-k key_len] to set max len of key. Default value: 20\n");
}
