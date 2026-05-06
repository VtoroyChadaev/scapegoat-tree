PNAME = program
target = $(PNAME)
CC = gcc 
CFLAGS = -Wall -Wextra -Werror -g3 -c
PREF_SRC = ../src/
PREF_OBJ = ../build/
LIB_PATH := ../build/

filename = make_$(lib).d

MAIN := $(PREF_SRC)main.c

LIB_SRC := $(wildcard $(PREF_SRC)*.c)

LIB_SRC := $(filter-out $(MAIN), $(LIB_SRC))

OBJ := $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.o, $(MAIN))

LIB_OBJ := $(patsubst $(PREF_SRC)%.c, $(LIB_PATH)%.o, $(LIB_SRC))

-include $(filename)

.PHONY : run valrun clean print
run :
	./$(PNAME)

valrun :
	valgrind --leak-check=full ./$(PNAME)

clean :
	rm -f $(PNAME) ../build/*

print :
	ls 
	ls ../build/


