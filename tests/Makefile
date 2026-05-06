PNAME = program
target = $(PNAME)
CC = gcc 
CFLAGS = -Wall -Wextra -Werror -g3 -c
PREF_SRC = ../src/
PREF_OBJ = ./build/
LIB_PATH := $(PREF_OBJ)

filename = make_$(lib).d

LIB_SRC := $(PREF_SRC)check_input.c $(PREF_SRC)tree.c $(PREF_SRC)node.c $(PREF_SRC)vector.c $(PREF_SRC)data.c

OBJ := $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.o, $(SRC))

LIB_OBJ := $(patsubst $(PREF_SRC)%.c, $(LIB_PATH)%.o, $(LIB_SRC))

-include $(filename)

.PHONY : run valrun clean print
run :
	./$(PNAME)

valrun :
	valgrind --leak-check=full ./$(PNAME)

clean :
	rm -f $(PNAME) ./build/*

print :
	ls 
	ls ./build/

