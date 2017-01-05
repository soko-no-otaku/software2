#include <stdio.h>

typedef struct node
{
  int value;
  struct node *left;
  struct node *right;
} Node;

void traverse(const Node *n)
{
  if (n == NULL) return;

  printf("value = %d\n", n->value);

  /*
    ???
  */
}

int main()
{
  Node n1  = {  1, NULL, NULL };
  Node n2  = {  2, NULL, NULL };
  Node n3  = {  3, NULL, NULL };
  Node n4  = {  4, NULL, NULL };
  Node n5  = {  5, NULL, NULL };
  Node n6  = {  6, NULL, NULL };
  Node n7  = {  7, NULL, NULL };
  Node n8  = {  8, NULL, NULL };
  Node n9  = {  9, NULL, NULL };
  Node n10 = { 10, NULL, NULL };

  /*
    ???
  */

  traverse(&n1);
}
