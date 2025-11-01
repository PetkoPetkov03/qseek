#include <stdio.h>
#include <stdlib.h>
#include <btree.h>
#include <cast.h>

void add_tree_impl(btree** tree, ntype item, btree* parent)
{
  btree* p;

  if(*tree == NULL) {
    p = (btree*)malloc(sizeof(btree));
    p->left=p->right=NULL;
    p->value = item;

    p->parent = parent;

    *tree = p;
    return;
  }

  if(item < (*tree)->value) {
    add_tree_impl(&((*tree)->left), item, *tree);
  } else {
    add_tree_impl(&((*tree)->right), item, *tree);
  }
}

const btree* search_tree(const btree* tree, ntype i)
{
  if(tree == NULL) return NULL;

  if(tree->value == i) return tree;

  if(tree->value < i) {
    return search_tree(tree->right, i);
  }else {
    return search_tree(tree->left, i);
  }
}

const btree* find_min(const btree* tree)
{
  if(tree == NULL) return NULL;

  if(tree->left == NULL) return tree;

  return find_min(tree->left);
}

const btree* find_max(const btree* tree)
{
  if(tree == NULL) return NULL;

  if(tree->right == NULL) return tree;

  return find_max(tree->right);
}

void delete_node(btree* tree, ntype item)
{
  if(tree == NULL) return;

  btree* element = castp(search_tree, btree, tree, item);

  if(element == NULL) return;

  int hasParent = element->parent != NULL;
  int isLeft = hasParent && element->value < element->parent->value;

  if(element->left == NULL && element->right == NULL) {
    if(hasParent) {
      if(isLeft) {
        element->parent->left = NULL;
      }else {
        element->parent->right = NULL;
      }
    }

    free(element);
  }else if(element->left != NULL && element->right == NULL) {
    element->left->parent = element->parent;
    
    if(hasParent) {
      if(isLeft) {
        btree* temp = element->parent;

        temp->left = element->left;
      }else {
        btree* temp = element->parent;

        temp->right = element->left;
      }
    }else {
      tree = element->left;
    }

    free(element);
  }else if(element->left == NULL && element->right != NULL) {
    element->right->parent = element->parent;

    if(hasParent){
      if(isLeft) {
        btree* temp = element->parent;

        temp->left = element->right;
      }else {
        btree* temp = element->parent;
        temp->right = element->right;
      }
    }

    free(element);
  } else {
    btree* right_min = castp(find_min, btree, element->right);

    element->value = right_min->value;

    delete_node(right_min, right_min->value);
  }
}

void inorder(btree* tree)
{
  if(tree == NULL) {
    return;
  }

  inorder(tree->left);
  printf("%i\n", tree->value);
  inorder(tree->right);
}

void preorder(btree* tree)
{
  if(tree == NULL) {
    return;
  }

  printf("%i\n", tree->value);
  preorder(tree->left);
  preorder(tree->right);
}

void postorder(btree* tree)
{
  if(tree == NULL) {
    return;
  }

  postorder(tree->left);
  postorder(tree->right);
  printf("%i\n", tree->value);
}
