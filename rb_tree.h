#ifndef RB_TREE
#define RB_TREE

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
void insert(RBtree* tree, int value);
RBnode* search(RBtree* tree, int target);
void delete_node(RBtree* tree, int target);

#endif