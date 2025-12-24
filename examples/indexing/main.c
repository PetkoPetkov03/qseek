#include <indexing.h>
#include <linux/limits.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <parser/context.h>
#include <parser/ruleset.h>
#include <stl/hashset.h>

int main(int argc, char** argv)
{
    char cwd[PATH_MAX] = {0};
    getcwd(cwd, PATH_MAX);

    if(argc < 2 || argc > 3) {
        fprintf(stderr, "Argument size not correct %d\n", argc);
        exit(EXIT_FAILURE);
    }

    char* filePath;

    for(int i = 1; i < argc; i++) {
        if(strcmp("--htmlf", argv[i]) == 0) {
            i += 1;
            filePath = (char*)malloc(sizeof(argv[i])/sizeof(argv[i][0]));
            strcpy(filePath, argv[i]);
        }
    }

    char next[100] = {0};
    sprintf(next, "/cache/%s.html", filePath);
    strcat(cwd, next);

    printf("cwd: %s\n", cwd);

    context_t ctx = context_init(cwd);
    set_mode(&ctx, HTML);

    configure_linking(&ctx);

    parse_func(&ctx);

    if(context_is_errors_thrown(&ctx)) {
        abort_and_dump(&ctx);
    }

    char* log[1024] = {0};
    int log_in[1024] = {0};
    size_t size = 0;
    for(size_t i = 0; i < ctx.tokensSize; i++) {
        if(ctx.tokens[i].token_type == htmlstr) {
            if(size == 0) {
                log[size] = ctx.tokens[i].data.ident.lexeme;
                log_in[size] = (log_in[size])+1;
                size++;
                continue;
            }

            for(size_t j = 0; j < size; j++) {
                if(strcmp(log[j], ctx.tokens[i].data.ident.lexeme) == 0) {
                    log_in[j] = (log_in[j])+1;
                    continue;
                }
            }

            log[size] = ctx.tokens[i].data.ident.lexeme;
            log_in[size] = (log_in[size])+1;
            size++;
        }
    }

    for(size_t i = 0; i < size; i++) {
        printf("Word: %s - %d\n", log[i], log_in[i]);
    }

    context_clean(&ctx);
    return 0;
}
