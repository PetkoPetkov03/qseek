#include <queue.h>
#include <stdio.h>
#include <cast.h>

int main()
{
  queue_t* queue = init_queue();

  char i;
  char a = 47;
  char b = 53;
  char c = 54;
  enqueue(queue, &a);
  enqueue(queue, &b);
  enqueue(queue, &c);

  i = cast_safe(dequeue, char, queue);

  printf("%c\n", i);
  
  i = cast_safe(dequeue, char, queue);

  printf("%c\n", i);
  
  i = cast_safe(dequeue, char, queue);

  printf("%c\n", i);

  
  i = cast_safe(dequeue, char, queue);

  printf("%c\n", i);
}
