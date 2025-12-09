#include <queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <cast.h>

int main()
{
  queue_t* queue = init_queue();

  for(char i = 'a'; i <= 'z'; i++) {
    char* buff = castp(malloc, char, sizeof(char));

    *buff = i;

    enqueue(queue, buff);
  }


  while(!queue_is_empty(queue)) {
    char buff = cast_safe(dequeue, char, queue);

    printf("%c\n", buff);
  }

  queue_clear(queue);
}
