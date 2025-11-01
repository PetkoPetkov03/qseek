#ifndef __BTREE__
#include <stddef.h>

typedef int ntype;

typedef struct __btree__ {
  ntype value;
  struct __btree__* parent;
  struct __btree__* left;
  struct __btree__* right;
} btree;

void add_tree_impl(btree** tree, ntype item, btree* parent);

#define add_tree(tree, item) add_tree_impl(tree, item, NULL)

const btree* search_tree(const btree* tree, ntype i);

const btree* find_min(const btree* tree);
const btree* find_max(const btree* tree);

void delete_node(btree* tree, ntype item);

void inorder(btree* tree);

void preorder(btree* tree);

void postorder(btree* tree);

#endif
