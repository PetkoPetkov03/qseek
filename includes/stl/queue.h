#ifndef __QUEUE__
#define __QUEUE__

#include <stddef.h>

typedef struct __node__ {
  void* value;
  struct __node__* next;
} node;

typedef struct __q_struct__ {
  struct __node__* front;
  struct __node__* rear;
} queue_t;

queue_t* init_queue(void);

void enqueue(queue_t* que, void* value);

void* dequeue(queue_t* que);

int queue_is_empty(queue_t* que);

void queue_clear(queue_t* que);

#endif
