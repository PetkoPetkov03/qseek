#define USE_CUSTOM_PARSER 1
#include "parser/tokenizer/tokenizer.h"
#include <parser/scanner/scanner.h>
#include "parser/analyzer/analyzer.h"
#include "parser/ruleset.h"
#include <parser/context.h>


int main(void)
{
    context_t ctx =
    context_init("/home/petkosuse/Dev/qseek/examples/parser/testurl.txt");

    set_mode(&ctx, URL);

    tokenizer_t* tokenizer = init_tokenizer();
    scanner_t* scanner = init_scanner();
    analyzer_t* analyzer = init_analyzer();

    link_scanner_instance(&ctx, scanner);
    link_tokenizer_instance(&ctx, tokenizer);
    link_analyzer_instance(&ctx, analyzer);

    scanner_load(scanner);

    parse_func(&ctx);

    if(context_is_error_trown(&ctx)) {
        abort_and_dump(&ctx);
    }

    print_tokens(&ctx);

    context_clean(&ctx);

    return 0;
}
