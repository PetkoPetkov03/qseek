#include <stl/queue.h>
#include <stdlib.h>
#include <stl/cast.h>
#include <stddef.h>

queue_t* init_queue()
{
  queue_t* q = castp(calloc, queue_t, 1, sizeof(queue_t));

  return q;
}

void enqueue(queue_t* que, void* value)
{
  node* new_node = castp(malloc, node, sizeof(node));

  new_node->value = value;
  new_node->next = NULL;

  if(que->rear == NULL) {
    que->front = que->rear = new_node;
  }else {
    que->rear->next = new_node;
    que->rear = new_node;
  }
}

int queue_is_empty(queue_t* que) 
{
  return que->front == NULL && que->rear == NULL ? 1 : 0;
}

void* dequeue(queue_t* que) 
{
  if(que->front == NULL) {
    return NULL;
  }

  node* temp = que->front;
  void* val = temp->value;

  que->front = que->front->next;

  if(que->front == NULL) {
    que->rear = NULL;
  }

  free(temp);

  return val;
}

void queue_clear(queue_t *que) 
{
  void* temp;
  while((temp = dequeue(que)) != NULL) {
    free(temp);
  }

  free(que);
}
