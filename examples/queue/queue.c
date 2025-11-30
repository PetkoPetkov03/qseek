#include <queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <cast.h>

int main()
{
    queue_t* queue = init_queue();

    char i;
    for(char a = 'a'; a <= 'z'; a++) {
        char* c = castp(malloc, char, sizeof(char));
        *c = a;
        enqueue(queue, c);
    }


    while(!queue_is_empty(queue)) {
        i = cast_safe(dequeue, char, queue);

        printf("%c\n", i);
    }
}
