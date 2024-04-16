#include <CUnit/Basic.h>
#include <stdlib.h>
#include <stdbool.h>
#include "rb_tree.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

int find_longest_path(RBnode* node) {
    if (!node) return 0;
    return 1 + max(find_longest_path(node->_left), find_longest_path(node->_right));
}

int find_shortest_path(RBnode* node) {
    if (!node) return 0;
    return 1 + min(find_shortest_path(node->_left), find_shortest_path(node->_right));
}

bool _black_depth(RBnode* node, int* height) {
    if (!node) {
        return true;
    }
    int left = 0;
    int right = 0;
    if (!_black_depth(node->_left, &left) || !_black_depth(node->_right, &right)) return false;
    if (left != right) return false;
    *height = left;
    if (node->_color == BLACK) *height += 1;
    return true;
}

bool _red_property(RBnode* node) {
    if (!node) return true;
    // red node must not have a red child
    // if a node has one child, then it must be a red child, otherwise black depth property would be violated
    if (!node->_left && node->_right) {
        if (node->_right->_color == BLACK) return false;
        if (node->_color == RED) return false;
        return _red_property(node->_right);
    } else if (node->_left && !node->_right) {
        if (node->_left->_color == BLACK) return false;
        if (node->_color == RED) return false;
        return _red_property(node->_left);
    } else {
        rb_color left_color = node->_left ? node->_left->_color : BLACK;
        rb_color right_color = node->_right ? node->_right->_color : BLACK;
        if (node->_color == RED && (left_color == RED || right_color == RED)) return false;
        return _red_property(node->_left) && _red_property(node->_right);
    }
}

bool is_red_black(RBtree* tree) {
    if (!tree) return true;
    if (!_red_property(tree->_root)) return false;
    int height = 0;
    printf("A\n");
    if (!_black_depth(tree->_root, &height)) return false;
    // int shortest_path = find_shortest_path(tree->_root);
    // int longest_path = find_longest_path(tree->_root);
    // if (shortest_path * 2 < longest_path) return false;
    return true;
}

int init_suite() {
    return 0;
}

int clean_suite() {
    return 0;
}

void test_create_rb_node() {
    RBnode* node = create_rb_node(10, BLACK);
    CU_ASSERT_EQUAL(node->_key, 10);
    CU_ASSERT_EQUAL(node->_color, BLACK);
    CU_ASSERT_PTR_NULL(node->_parent);
    CU_ASSERT_PTR_NULL(node->_left);
    CU_ASSERT_PTR_NULL(node->_right);

    free(node);
}

void test_create_rb_tree() {
    RBtree* tree = create_rb_tree();

    CU_ASSERT_PTR_NULL(tree->_root);

    free(tree);
}

void test_free_rb_tree() {
    // free an empty tree
    RBtree* tree1 = create_rb_tree();
    free_rb_tree(tree1);

    // free a tree with one node
    RBtree* tree2 = create_rb_tree();
    insert(tree2, 10);
    free_rb_tree(tree2);

    //free a tree with multiple nodes
    RBtree* tree3 = create_rb_tree();
    insert(tree3, 1);
    insert(tree3, 2);
    insert(tree3, 3);
    insert(tree3, 4);
    free_rb_tree(tree3);
}

void test_insert() {
    RBtree* tree = create_rb_tree();
    insert(tree, 1);
    CU_ASSERT_EQUAL(tree->_root->_key, 1);
    CU_ASSERT_TRUE(is_red_black(tree));
    insert(tree, 2);
    CU_ASSERT_EQUAL(search(tree, 2)->_key, 2);
    CU_ASSERT_EQUAL(search(tree, 1)->_key, 1);
    CU_ASSERT_TRUE(is_red_black(tree));

    insert(tree, 3);
    insert(tree, 4);
    insert(tree, 5);
    CU_ASSERT_TRUE(is_red_black(tree));

    free_rb_tree(tree);
}

void test_search() {
    RBtree* tree = create_rb_tree();
    CU_ASSERT_PTR_NULL(search(tree, 1));
    insert(tree, 1);
    CU_ASSERT_EQUAL(search(tree, 1), tree->_root);
    CU_ASSERT_PTR_NULL(search(tree, 2));
    insert(tree, 2);
    CU_ASSERT_EQUAL(search(tree, 2)->_key, 2);
    free_rb_tree(tree);
}

void test_delete_node() {
    RBtree* tree = create_rb_tree();
    // should not crash
    delete_node(tree, 1);
    insert(tree, 1);
    delete_node(tree, 1);
    CU_ASSERT_PTR_NULL(tree->_root);;
    CU_ASSERT_PTR_NULL(search(tree, 1));

    insert(tree, 1);
    insert(tree, 2);
    insert(tree, 3);
    insert(tree, 4);
    insert(tree, 5);
    insert(tree, 6);
    insert(tree, 0);
    insert(tree, -5);
    delete_node(tree, 3);
    CU_ASSERT_PTR_NULL(search(tree, 3));
    CU_ASSERT_TRUE(is_red_black(tree));
    delete_node(tree, 2);
    CU_ASSERT_TRUE(is_red_black(tree));
    CU_ASSERT_PTR_NULL(search(tree, 2));
    delete_node(tree, 1);
    CU_ASSERT_PTR_NULL(search(tree, 1));
    CU_ASSERT_TRUE(is_red_black(tree));
    delete_node(tree, 0);
    CU_ASSERT_PTR_NULL(search(tree, 0));
    CU_ASSERT_TRUE(is_red_black(tree));
    delete_node(tree, -5);
    CU_ASSERT_PTR_NULL(search(tree, -5));
    CU_ASSERT_TRUE(is_red_black(tree));
    delete_node(tree, 5);
    CU_ASSERT_PTR_NULL(search(tree, 5));
    CU_ASSERT_TRUE(is_red_black(tree));
    delete_node(tree, 6);
    CU_ASSERT_PTR_NULL(search(tree, 6));
    CU_ASSERT_TRUE(is_red_black(tree));

    free_rb_tree(tree);
}

int main() {
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("Red-Black Tree Tests", init_suite, clean_suite);
    CU_add_test(suite, "test_create_rb_node", test_create_rb_node);
    CU_add_test(suite, "test_create_rb_tree", test_create_rb_tree);
    CU_add_test(suite, "test_free_rb_tree", test_free_rb_tree);
    CU_add_test(suite, "test_free_rb_tree", test_free_rb_tree);
    CU_add_test(suite, "test_insert", test_insert);
    CU_add_test(suite, "test_search", test_search);
    CU_add_test(suite, "test_delete_node", test_delete_node);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}