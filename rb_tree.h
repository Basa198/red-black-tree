#ifndef RB_TREE
#define RB_TREE

#include <stdbool.h>

typedef enum rb_color { BLACK, RED } rb_color;

typedef struct RBnode {
    struct RBnode* _parent;
    struct RBnode* _child[2];
    rb_color _color;
    int _key;
} RBnode;

#define LEFT 0
#define RIGHT 1
#define _left _child[LEFT]
#define _right _child[RIGHT]

typedef struct RBtree {
    RBnode* _root;
} RBtree;

RBnode* create_rb_node(int key, rb_color color);
RBtree* create_rb_tree();
void free_rb_tree(RBtree* tree);
bool rb_insert(RBtree* tree, int value);
RBnode* rb_search(RBtree* tree, int target);
bool rb_delete_node(RBtree* tree, int target);

#endif