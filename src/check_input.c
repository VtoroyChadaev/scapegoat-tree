#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include "check_input.h"

int get_int(int *input, char *prompt, check_int_func func) {
	if (prompt) {
		printf("%s", prompt);
	}
	
	int flag = 1;

	int returned = scanf("%d", input);

	if (func) {
		flag = (*func)(*input);
	}
	
	while (returned < 1 || !flag) {
		
		if (returned < 0) {
			scanf("%*c");
			return EOF;
		}
		
		scanf("%*[^\n]");
		scanf("%*c");
		printf("Please, enter an integer\n");
		returned = scanf("%d", input);

		if (func) {
			flag = (*func)(*input);
		}
	}
	
	scanf("%*c");
	return 0;
}

int get_sizet(size_t *input, char *prompt, check_sizet_func func) {
	char *str = readline(prompt);

	if (!str) {
		return EOF;
	}

	*input = 0;
	int flag = str_to_sizet(input, str);
	int extra_flag = 1;

	if (func) {
		extra_flag = (*func)(*input);
	}
	
	while (!flag || !extra_flag) {
		free(str);
		char *help_prompt = "Please, enter a whole number >= 0\n";
		str = readline(help_prompt);

		if (!str) {
			return EOF;
		}

		flag = str_to_sizet(input, str);

		if (func) {
			extra_flag = (*func)(*input);
		}
	}

	if (str) {
		free(str);
	}
	
	return 0;
}

int str_to_sizet(size_t *result_number, char *str) {
	if (!str) {
		return 0;
	}

  *result_number = 0;
	int flag = 0;
	size_t depth = 0; //number of digits
	char *string_number = NULL;
	
	while (*str && *str >= '0' && *str <= '9') {
		flag = 1;
		depth++;
		string_number = realloc(string_number, depth);
		string_number[depth - 1] = *str;
		str++;
	}

	if (flag) {
		size_t weight = 1;

		for (size_t i = 0; i < depth; i++) {
			int digit = string_number[depth - 1 - i] - '0';
			(*result_number) += digit * weight;
			weight *= 10;
		}
	}

	if (string_number) {
		free(string_number);
	}
	return flag;
}

int str_to_ssizet(ssize_t *sz, char *str) {
	if (!str) {
		return 0;
	}

	int flag = 0;
	size_t depth = 0; //number of digits
	char *number = NULL;
	
	while (*str && *str >= '0' && *str <= '9') {
		flag = 1;
		depth++;
		number = realloc(number, depth);
		number[depth - 1] = *str;
		str++;
	}

	if (flag) {
		size_t weight = 1;

		for (size_t i = 0; i < depth; i++) {
			int digit = number[depth - 1 - i] - '0';
			(*sz) += digit * weight;
			weight *= 10;
		}
	}

	if (number) {
		free(number);
	}
	return flag;
}

