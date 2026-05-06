LIB := $(LIB_PATH)libtree_tools.a

$(target) : $(LIB_OBJ) $(PREF_OBJ)main.o
	@echo $(EXCESS)
	ar rs  $(LIB) $(LIB_OBJ)
	$(CC) $(PREF_OBJ)main.o -L $(LIB_PATH) -ltree_tools -lm -lreadline -lgvc -lcgraph -o $(PNAME)
	rm -f $(LIB_OBJ) $(PREF_OBJ)main.o

$(PREF_OBJ)main.o : $(PREF_SRC)main.c
	$(CC) $(CFLAGS) $< -o $@

$(LIB_OBJ): $(LIB_PATH)%.o: $(PREF_SRC)%.c
	$(CC) $(CFLAGS) $< -o $@
