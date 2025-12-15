#include <parser/context.h>
#include <parser/ruleset.h>
#include <linux/limits.h>
#include <unistd.h>

int main()
{
    char cwd[PATH_MAX];
    getcwd(cwd, PATH_MAX);
    sprintf(cwd, "%s/%s.html", cwd, "cache/crawlertest");
    context_t ctx = context_init(cwd);

    set_mode(&ctx, HTML);

    configure_linking(&ctx);

    parse_func(&ctx);

    if(context_is_errors_thrown(&ctx)) {
        abort_and_dump(&ctx);
    }

    print_tokens(&ctx);

    context_clean(&ctx);

    return 0;
}
