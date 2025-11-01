#include <queue.h>
#include <stdlib.h>
#include <cast.h>
#include <stddef.h>

queue_t* init_queue()
{
  queue_t* q = castp(malloc, queue_t, sizeof(queue_t));

  q->front = q->rear = NULL;

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

void* dequeue(queue_t* que) {
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
