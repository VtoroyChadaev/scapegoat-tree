#ifndef CHECK_INPUT_H
#define CHECK_INPUT_H

typedef int (*check_int_func) (int);

typedef size_t (*check_sizet_func) (size_t);

int get_int (int *, char *prompt, check_int_func);

int get_sizet(size_t *, char *prompt, check_sizet_func);

int str_to_sizet(size_t *, char *str);

int str_to_ssizet(ssize_t *, char *str);

#endif

