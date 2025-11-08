#include "parser/token.h"
#include <cast.h>
#include <stdlib.h>
#include <parser/context.h>
#include <parser/analyzer/analyzer.h>
#include <parser/tokenizer/tokenizer.h>

analyzer_t* init_analyzer() 
{
    analyzer_t* analyzer = castp(malloc, analyzer_t, sizeof(analyzer_t));

    return analyzer;
}

int accept(analyzer_t* analyzer)
{
    if(analyzer->ctx->current_token.token_type == analyzer->symbol) {
        tokenize(analyzer->ctx->tokenizer);
        return 1;
    }
    return 0;
}

int expect(analyzer_t* analyzer)
{
    if(accept(analyzer)) return 1;

    context_error_report(analyzer->ctx, "unexpected symbol");
    return 0;
}

void analyzer_clean(analyzer_t *analyzer)
{
    free(analyzer);
}