#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAXLEN 1000

struct node
{
  char *str;
  struct node *next;
};

typedef struct node Node;

Node *push_front(Node *begin, const char *str)
{
  // Create a new element
  Node *p = malloc(sizeof(Node));
  char *s = malloc(strlen(str) + 1);
  strcpy(s, str);
  p->str = s;
  p->next = begin;

  return p;  // Now the new element is the first element in the list
}

Node *pop_front(Node *begin)
{
  assert(begin != NULL); // Don't call pop_front() when the list is empty
  Node *p = begin->next;

  free(begin->str);
  free(begin);

  return p;
}

Node *push_back(Node *begin, const char *str)
{
  if (begin == NULL) {   // If the list is empty
    return push_front(begin, str);
  }

  // Find the last element
  Node *p = begin;
  while (p->next != NULL) {
    p = p->next;
  }

  // Create a new element
  Node *q = malloc(sizeof(Node));
  char *s = malloc(strlen(str) + 1);
  strcpy(s, str);
  q->str = s;
  q->next = NULL;

  // The new element should be linked from the previous last element
  p->next = q;

  return begin;
}

Node *pop_back(Node *begin) {
  assert(begin != NULL);

  if (begin->next == NULL) {
    return pop_front(begin);
  }

  Node *current = begin;
  Node *prev = current;
  while (current->next != NULL) {
    prev = current;
    current = current->next;
  }

  free(current->str);
  free(current);

  prev->next = NULL;

  return begin;
}

Node *remove_all(Node *begin)
{
  while ((begin = pop_front(begin)))
    ; // Repeat pop_front() until the list becomes empty
  return begin;  // Now, begin is NULL
}

int main()
{
  Node *begin = NULL; // pointer to the first element in the list

  // Read all lines from stdin and store them in the list
  char buf[MAXLEN];
  while (fgets(buf, MAXLEN, stdin)) {
    //begin = push_front(begin, buf);
    begin = push_back(begin, buf); // Try this instead of push_front()
  }

  begin = pop_front(begin);  // What will happen if you do this?
  begin = pop_back(begin);   // What will happen if you do this?
  //begin = remove_all(begin); // What will happen if you do this?

  // Print all the strings stored in the list
  const Node *p;
  for (p = begin; p != NULL; p = p->next) {
    printf("%s", p->str);
  }

  return 0;
}
