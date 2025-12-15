#include <httpc/httpclient.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>

int main()
{
    client_fetch_input();

    client_parse_url();

    client_fetch_content();

    return 0;
}
