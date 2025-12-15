#include "parser/tokenizer/tokenizer.h"
#include <parser/scanner/scanner.h>
#include "parser/analyzer/analyzer.h"
#include "parser/ruleset.h"
#include <parser/context.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main(void)
{
    system("pwd > ./pwd.txt");
    int fd = open("./pwd.txt", O_RDONLY);

    char temp;
    char buffer[100];
    int i = 0;

    while((read(fd, &temp, 1)) != 0) {
        buffer[i++] = temp;
    }

    buffer[--i] = '\0';

    printf("pwd: %s\n", buffer);

    char result[250];

    snprintf(result, sizeof(result), "%s/examples/parser/testurl.txt", buffer);
    printf("dir: %s\n", result);

    context_t ctx =
    context_init(result);

    set_mode(&ctx, URL);

    configure_linking(&ctx);

    parse_func(&ctx);

    if(context_is_errors_thrown(&ctx)) {
        abort_and_dump(&ctx);
    }

    print_tokens(&ctx);

    context_clean(&ctx);

    return 0;
}
