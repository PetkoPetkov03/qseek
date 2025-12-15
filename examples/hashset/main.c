#include <stl/hashset.h>
#include <stdio.h>
#include <stl/cast.h>

int main()
{
    Hash_Set int_set = {0};

    int a = 1234;
    int c = 133;

    hset_store(&int_set, "cnum", &a);
    hset_store(&int_set, "cfum", &c);

    int b = cast(hset_get, int, &int_set, "cnum");
    int d = cast(hset_get, int, &int_set, "cfum");

    printf("%d %d\n", b, d);

    return 0;
}
