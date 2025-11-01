#include <cast.h>
#include <btree.h>
#include <stdio.h>

int main(void)
{
  btree* tree = NULL;

  add_tree(&tree, 9);
  add_tree(&tree, 6);
  add_tree(&tree, 4);
  add_tree(&tree, 7);
  add_tree(&tree, 16);
  add_tree(&tree, 10);
  add_tree(&tree, 19);

  inorder(tree);
  printf("done\n");
  preorder(tree);
  printf("done\n");
  postorder(tree);
  printf("done\n");
  
  return 0;
}
