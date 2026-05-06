#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include "check_input.h"
#include <unistd.h>
#include "tree.h"
#include "vector.h"
#include "node.h"
#include "data.h"
#include "str_tools.h"


typedef enum {
  INP = 0,
  IMPORT_DATA,
  INS,
  DEL,
  FIND,
  PRINT,
  GRAPH_PRINT,
  TRAVERSE,
  SPEC_FIND,
} options;

typedef int (*dialog_func_ptr)(Tree **tree);


int check_option(int option);

int check_file_option(int option);

int dialog(Tree *tree);

int t_input_d(Tree **tree);

int t_import(Tree **tree);

int t_insert_d(Tree **tree);

int t_remove_d(Tree **tree);

int find_d(Tree **tree);

int t_print(Tree **tree);

int t_graph_print(Tree **tree);

int traversal_d(Tree **tree);

int spec_find_d(Tree **tree);

int t_input(Tree *tree, FILE *fp);

int read_key_and_value(FILE *fp, char **key_and_value);

int check_data(Node *node, char *key, char *info);

void talk_to_user();

void arr_free(char **arr, int size);

FILE *get_file(int *status, char *open_type);

void print_menu();

void print_errs(int status, size_t line_index, char *filename);


int main() {
  Tree *tree = NULL;

  dialog(tree);

  return 0;
}


int check_option(int option) {
  int flag = option >= INP && option <= SPEC_FIND;

  if (!flag) {
    printf("Please, enter an integer in range [%d, %d]\n", INP, SPEC_FIND);
  }

  return flag;
}


int dialog(Tree *tree) {
  int option = 0, status = OK, func_status = OK;

  dialog_func_ptr d_funcs[] = {
    t_input_d,
    t_import,
    t_insert_d,
    t_remove_d,
    find_d,
    t_print,
    t_graph_print,
    traversal_d,
    spec_find_d
  };

  print_menu();
  status = get_int(&option, NULL, check_option);

  while (status != EOF && status != ALLOC_ERR) {
    dialog_func_ptr d_func = d_funcs[option];
    func_status = (*d_func)(&tree);

    if (func_status != OK) {
      print_errs(func_status, 0, NULL);

      if (func_status == EOF || func_status == ALLOC_ERR) {
        status = func_status;
        continue;
      }
    }

    print_menu();
    status = get_int(&option, NULL, check_option);
  }

  if (tree) {
    tree_free(&tree, tree->root);
  }

  return status;
}


void print_menu() {
  char *message = "\n\t\t\t\t0. Create new tree\n"
    "\t\t\t\t1. Import data\n"
    "\t\t\t\t2. Insert item in tree\n"
    "\t\t\t\t3. Delete item with current version of key\n"
    "\t\t\t\t4. Find item with current key\n"
    "\t\t\t\t5. Print tree\n"
    "\t\t\t\t6. Print tree like graph\n"
    "\t\t\t\t7. Traverse tree\n"
    "\t\t\t\t8. Find item with closest key\n\n";

  printf("%s", message);
}


int t_input_d(Tree **tree) {
  FILE *fp = stdin;
  int file_option = 0, status = 0;
  char *prompt =
    "\t\t\t\t1. Enter data manually\n\t\t\t\t2. Import data from file\n";

  status = get_int(&file_option, prompt, check_file_option);

  if (status == EOF) {
    return status;
  }

  if (file_option > 1) {
    fp = get_file(&status, "r");

    if (status != OK) {
      return status;
    }
  }

  if (*tree) {
    tree_free(tree, (*tree)->root);
  }

  char *str_alpha = readline("Input alpha value\n");

  if (!str_alpha) {
    return EOF;
  }

  double alpha = atof(str_alpha);

  if (alpha < 0.5 || alpha >= 1) {
    free(str_alpha);
    return NOT_ALPHA;
  }

  *tree = tree_new(alpha);

  if (!*tree) {
    if (fp != stdin) {
      fclose(fp);
    }

    free(str_alpha);
    return ALLOC_ERR;
  }

  status = t_input(*tree, fp);

  if (fp != stdin) {
    fclose(fp);
  }

  free(str_alpha);
  return status;
}


int check_file_option(int option) {
  int flag = option >= 1 && option <= 2;

  if (!flag) {
    printf("Please, enter an integer in range [%d, %d]\n", 1, 2);
  }

  return flag;
}


FILE *get_file(int *status, char *open_type) {
  char *filename = readline("Enter name of file to open\n");

  if (!filename) {
    *status = EOF;
    return NULL;
  }

  char *token = strtok(filename, " \t");
  FILE *fp = NULL;

  fp = fopen(token, open_type);

  if (!fp) {
    *status = OPEN_ERR;
    free(filename);
    return NULL;
  }

  *status = OK;
  free(filename);
  return fp;
}


void talk_to_user() {
  char message[] =
    "\n\t\t\t\tInput item' key and info separated in 2 lines\n"
    "\t\t\t\tEXAMPLE `56\n\t\t\t\t        3498`\n"
    "\t\t\t\tTO END INPUT, press <C-p> <C-j>\n\n"
    "\t\t\t\tN.B: item's INFO and KEY must be a WHOLE_NUMBER\n\n";
  fprintf(stdout, "%s", message);
}


int t_import(Tree **tree) {
  FILE *fp = stdin;
  int file_option = 0, status = 0;

  if (!*tree) {
    return NO_TREE;
  }

  char *prompt =
    "\t\t\t\t1. Enter data manually\n\t\t\t\t2. Import data from file\n";

  status = get_int(&file_option, prompt, check_file_option);

  if (status == EOF) {
    return status;
  }

  if (file_option > 1) {
    fp = get_file(&status, "r");

    if (status != OK) {
      return status;
    }
  }

  status = t_input(*tree, fp);
  return status;
}


int t_insert_d(Tree **tree) {
  int status = is_empty(*tree);

  if (status == NO_TREE) {
    return status;
  }

  char *key = readline("Input KEY of Item\n");

  if (!key) {
    return EOF;
  }

  if (strlen(key) == 0) {
    free(key);
    return EMPTY_LINE;
  }

  char *info = readline("Input INFO of Item\n");

  if (!info) {
    free(key);
    return EOF;
  }

  Node *node = node_new();

  if (!node) {
    free(key);
    free(info);
    return ALLOC_ERR;
  }

  size_t number_info = 0;
  int is_converted = str_to_sizet(&(node->key), key);

  if (strlen(info) > 0) {
    is_converted *= str_to_sizet(&number_info, info);
  }

  if (!is_converted) {
    node_free(node);
    free(key);
    free(info);
    return NOT_NUMBER;
  }

  if (number_info) {
    status = data_append(&(node->data), number_info);

    if (status == ALLOC_ERR) {
      return status;
    }
  }

  status = insert(*tree, node);

  free(key);
  free(info);
  return status;
}


int t_remove_d(Tree **tree) {
  int status = is_empty(*tree);

  if (status != NOT_EMPTY) {
    return status;
  }

  printf("\t\t\n");
  char *key = readline("Input key of element to delete\n");

  if (!key) {
    return EOF;
  }

  size_t number_key = 0;
  int is_converted = str_to_sizet(&number_key, key);

  if (!is_converted) {
    free(key);
    return NOT_NUMBER;
  }

  char *string_number = readline("Input version number\n");

  size_t version = 1;

  is_converted = str_to_sizet(&version, string_number);

  if (!is_converted) {
    free(key);
    free(string_number);
    return NOT_NUMBER;
  }

  status = t_remove(*tree, number_key, version);

  free(key);
  free(string_number);

  return status;
}


int find_d(Tree **tree) {
  int status = is_empty(*tree);

  if (status != NOT_EMPTY) {
    return status;
  }

  printf("\t\t\n");
  char *key = readline("Input key of element to find\n");

  if (!key) {
    return EOF;
  }

  size_t number_key = 0;
  int is_converted = str_to_sizet(&number_key, key);

  if (!is_converted) {
    free(key);
    return NOT_NUMBER;
  }

  Vector *vector = find(*tree, number_key);

  if (!vector) {
    free(key);
    return NO_KEY;
  }

  vector_print(vector);
  vector_free(vector);
  free(key);
  return OK;
}


int t_print(Tree **tree) {
  int status = is_empty(*tree);

  if (status == NO_TREE) {
    printf("NO TREE");
    return OK;
  }

  if (status == EMPTY) {
    printf("EMPTY TREE");
    return OK;
  }

  printf("\n\n");
  format_print(*tree, (*tree)->root);
  printf("\n\n");
  return OK;
}


int t_graph_print(Tree **tree) {
  int status = is_empty(*tree);

  if (status == NO_TREE) {
    printf("NO TREE");
    return OK;
  }

  if (status == EMPTY) {
    printf("EMPTY TREE");
    return OK;
  }

  graph_print(*tree);

  return OK;
}


int traversal_d(Tree **tree) {
  int status = is_empty(*tree);

  if (status != NOT_EMPTY) {
    return status;
  }

  printf("\t\t\n");
  char *string_number = readline("Input number."
      "To traverse WHOLE TREE enter EMPTY LINE\n");

  if (!string_number) {
    return EOF;
  }

  ssize_t number = 0;

  if (strlen(string_number) == 0) {
    number = -1;
  } else {
    int is_converted = str_to_ssizet(&number, string_number);

    if (!is_converted) {
      free(string_number);
      return NOT_NUMBER;
    }
  }

  traversal((*tree)->root, number);
  printf("\n");

  free(string_number);
  return OK;
}


int spec_find_d(Tree **tree) {
  int status = is_empty(*tree);

  if (status != NOT_EMPTY) {
    return status;
  }

  char *string_number = readline("Input number to compare with\n");

  if (!string_number) {
    return EOF;
  }

  size_t number_key = 0;
  int is_converted = str_to_sizet(&number_key, string_number);

  if (!is_converted) {
    free(string_number);
    return NOT_NUMBER;
  }

  Vector *vector = special_find(*tree, number_key);

  if (!vector) {
    free(string_number);
    return NO_KEY;
  }

  free(string_number);
  vector_print(vector);
  vector_free(vector);
  return OK;
}


int t_input(Tree *tree, FILE *fp) {
  if (!tree) {
    return NO_TREE;
  }

  if (fp == stdin) {
    talk_to_user();
  }

  size_t line_index = 0;
  char *key_and_value[2];

  int status = read_key_and_value(fp, key_and_value);

  while (status == OK) {
    line_index++;
    Node *node = node_new();
    char *key = key_and_value[0];
    char *value = key_and_value[1];

    if (!node) {
      free(key);
      free(value);
      return ALLOC_ERR;
    }

    if (strlen(key) == 0) {
      free(key);
      free(value);
      node_free(node);
      status = read_key_and_value(fp, key_and_value);
      continue;
    }

    size_t number_value = 0;
    int is_converted = str_to_sizet(&(node->key), key);

    if (strlen(value) > 0) {
      is_converted *= str_to_sizet(&number_value, value);
    }

    if (!is_converted) {
      print_errs(NOT_NUMBER, line_index, NULL);
      free(key);
      free(value);
      node_free(node);
      status = read_key_and_value(fp, key_and_value);
      continue;
    }

    int insert_status = 0;

    if (number_value) {
      insert_status = data_append(&(node->data), number_value);

      if (insert_status == ALLOC_ERR) {
        return insert_status;
      }
    }

    insert_status = insert(tree, node);

    if (insert_status == ALLOC_ERR) {
      free(key);
      free(value);
      node_free(node);
      return ALLOC_ERR;
    }

    free(key);
    free(value);

    status = read_key_and_value(fp, key_and_value);
  }

  /*
  if (key_and_value[0]) {
    free(key_and_value[0]);
  }

  if (key_and_value[1]) {
    free(key_and_value[1]);
  }
  */

  if (status == ALLOC_ERR) {
    return status;
  }

  return OK;
}

int read_key_and_value(FILE *fp, char **key_and_value) {
  int status = OK;

  key_and_value[0] = fstr_read(fp, &status);

  if (status != OK) {
    return status;
  }

  key_and_value[1] = fstr_read(fp, &status);

  return status;
}

int check_data(Node *node, char *key, char *info) {
  node->data = data_new();

  if (!node->data) {
    return ALLOC_ERR;
  }

  size_t key_len = strlen(key);

  if (!key_len) {
    free(key);
    free(info);
    node_free(node);
    return EMPTY_LINE;
  }

  return OK;
}


void print_errs(int status, size_t line_index, char *filename) {
  if (status == EOF) {
    status++;
  }

  if (status > NOT_ALPHA) {
    fprintf(stderr, "%s\n", "Congradulations! You have broken Error system");
    return;
  }

  char *messages[] = {
    "^D",
    "NO TREE",
    "EMPTY TREE",
    "Can't find current KEY in tree",
    "ERR: can't allocate memory",
    "ERR: can't open file",
    "Empty line",
    "Input is not a whole number",
    "ALPHA must be in RANGE [0.5, 1)"
  };

  fprintf(stderr, "%s", messages[status]);

  if (filename) {
    fprintf(stderr, " %s", filename);
  }

  if (line_index) {
    fprintf(stderr, " at line %zu", line_index);
  }

  fprintf(stderr, "\n");
}
