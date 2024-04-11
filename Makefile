CFLAGS = -Wall

LIBS = -lcunit

TARGET = tests

SRCS = rb_tree_test.c rb_tree.c
OBJS = rb_tree_test rb_tree

$(TARGET): $(OBJS)
	gcc $(CFLAGS) -o $@ $^ $(LIBS)

rb_tree_test: rb_tree_test.c
	gcc $(CFLAGS) -c $< -o $@

rb_tree: rb_tree.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)	