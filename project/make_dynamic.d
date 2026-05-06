LIB := $(LIB_PATH)libtree_tools.so

$(target) : $(PREF_OBJ)main.o $(LIB)
	$(CC) $(PREF_OBJ)main.o -L $(LIB_PATH) -ltree_tools -lm -lreadline -lgvc -lcgraph -o $(PNAME)
	rm -f $(LIB_OBJ) $(PREF_OBJ)main.o

$(PREF_OBJ)main.o : $(PREF_SRC)main.c
	$(CC) $(CFLAGS) $< -o $@

$(LIB): $(LIB_SRC)
	$(CC) -shared -fPIC $(LIB_SRC) -o $@ -lreadline -lgvc -lcgraph

