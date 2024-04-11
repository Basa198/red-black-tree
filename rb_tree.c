#include <stdlib.h>
#include <stdio.h>

#include "rb_tree.h"

void _free_node(RBnode* node) {
    if (!node) return;

    _free_node(node->_left);
    node->_left = NULL;
    _free_node(node->_right);
    node->_right = NULL;
    node->_parent = NULL;

    free(node);
}

RBnode* _find_insertion_point(RBnode* root, int value) {
    if (root->_key == value) return NULL; // don't allow duplicate values
    if (value < root->_key && root->_left) return _find_insertion_point(root->_left, value);
    else if (value > root->_key && root->_right) return _find_insertion_point(root->_right, value);
    return root;
}

RBnode* create_rb_node(int key, rb_color color) {
    RBnode* node = (RBnode*)malloc(sizeof(RBnode));
    if (!node) {
        fprintf(stderr, "Failed Memory Allocation (RBnode)\n");
        return NULL;
    }
    
    node->_color = color;
    node->_key = key;
    node->_parent = NULL;
    node->_left = NULL;
    node->_right = NULL;

    return node;
}

RBnode* rotate_dir_root(RBtree* tree, RBnode* parent_node, int dir) {
    RBnode* grandparent_node = parent_node->_parent;
    RBnode* child_node = parent_node->_child[1 - dir];
    RBnode* adopted_child = child_node->_child[dir];
    parent_node->_child[1 - dir] = adopted_child;
    if (adopted_child) adopted_child->_parent = parent_node;
    child_node->_child[dir] = parent_node;
    parent_node->_parent = child_node;
    child_node->_parent = grandparent_node;
    if (grandparent_node) {
        grandparent_node->_child[grandparent_node->_left == parent_node ? LEFT : RIGHT] = child_node;
    } else {
        tree->_root = child_node; // if grandparent is NULL, then child is the new root
    }
    return child_node;
}

RBtree* create_rb_tree() {
    RBtree* tree = (RBtree*)malloc(sizeof(RBtree));
    if (!tree) {
        fprintf(stderr, "Memory Allocation Failed (RBtree)\n");
        return NULL;
    }

    tree->_root = NULL;

    return tree;
}

RBnode* search(RBtree* tree, int target) {
    if (!tree) return NULL;

    RBnode* node = tree->_root;

    while (node && node->_key != target) {
        if (node->_key > target) node = node->_left;
        else node = node->_right;
    }

    return node;
}

void free_rb_tree(RBtree* tree) {
    if (!tree) return;

    RBnode* node = tree->_root;
    _free_node(node);
    tree->_root = NULL;

    free(tree);
}

void insert(RBtree* tree, int value) {
    if (!tree) {
        fprintf(stderr, "Invalid insert: tree is NULL\n");
        return;
    }
    RBnode* new_node = create_rb_node(value, RED);
    if (!new_node) exit(1);
    if (!tree->_root) { // if tree is empty
        tree->_root = new_node;
        return;
    }
    RBnode* parent_node = _find_insertion_point(tree->_root, value);
    if (!parent_node) { // Will only occur when value is a duplicate
        free(new_node);
        return;
    }; 
    int dir = parent_node->_key > new_node->_key ? LEFT : RIGHT;
    new_node->_parent = parent_node;
    parent_node->_child[dir] = new_node; 
    while (parent_node) {
        if (parent_node->_color == BLACK) return; // no red property violation
        RBnode* grandparent_node = parent_node->_parent;
        if (!grandparent_node) { // parent is root and red
            parent_node->_color = BLACK;
            return;
        }
        dir = grandparent_node->_left == parent_node ? LEFT : RIGHT;
        RBnode* uncle = grandparent_node->_child[1 - dir];
        if (!uncle || uncle->_color == BLACK) { // parent is red and uncle is black
            if (new_node == parent_node->_child[1 - dir]) { // if new node is an inner grandchild
                rotate_dir_root(tree, parent_node, dir);
                new_node = parent_node;
                parent_node = grandparent_node->_child[dir];
            }
            rotate_dir_root(tree, grandparent_node, 1 - dir);
            parent_node->_color = BLACK;
            grandparent_node->_color = RED;
            return;
        }
        // both parent and uncle are red
        // make parent and uncle black and grandparent red, then grandparent becomes the new_node
        parent_node->_color = BLACK;
        uncle->_color = BLACK;
        grandparent_node->_color = RED;
        new_node = grandparent_node; 
        parent_node = grandparent_node->_parent;
    } 
}

void delete_node(RBtree*, int target) {}