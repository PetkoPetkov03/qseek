#include <stl/hashset.h>
#include <stdio.h>

int main()
{
    Hash_Set int_set = {0};

    int a = 1234;
    int c = 133;

    hset_store(&int_set, &a, &a);
    hset_store(&int_set, "cfum", &c);

    int b = *(int*)hset_get(&int_set, &a);
    int d = *(int*)hset_get(&int_set, "cfum");

    printf("%d %d\n", b, d);

    return 0;
}
