#include <httpc/dns.h>
#include <stdio.h>

int main()
{
    char host[100];
    printf("enter host: ");
    scanf("%s", host);
    char* ip = htoip(host);

    printf("%s\n", ip);
    return 0;
}
