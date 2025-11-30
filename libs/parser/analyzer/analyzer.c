#include "parser/token.h"
#include <cast.h>
#include <stdio.h>
#include <stdlib.h>
#include <parser/context.h>
#include <parser/analyzer/analyzer.h>
#include <parser/tokenizer/tokenizer.h>

analyzer_t* init_analyzer()
{
    analyzer_t* analyzer = castp(malloc, analyzer_t, sizeof(analyzer_t));

    return analyzer;
}

int accept(context_t* ctx, token_t T)
{
    if(ctx->cToken.token_type == T) {
        tokenize(ctx);
        return 1;
    }
    return 0;
}

int expect(context_t* ctx, token_t T)
{
    if(accept(ctx, T)) return 1;

    //print_tokens(ctx);
    context_error_report(ctx, "unexpected symbol");

    return 0;
}

void analyzer_clean(analyzer_t *analyzer)
{
    free(analyzer);
}
