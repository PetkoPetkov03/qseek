#include "parser/analyzer/analyzer.h"
#include <parser/tokenizer/tokenizer.h>
#include "parser/token.h"
#include <parser/ruleset.h>
#include <parser/context.h>

void file(context_t *ctx)
{
    record(ctx);

    while ((ctx->cToken.token_type == intconst) || (ctx->cToken.token_type == quotas)) {
        record(ctx);
    }
}

void record(context_t *ctx)
{
    field(ctx);

    while(ctx->cToken.token_type == semicolon) {
        accept(ctx, semicolon);
        field(ctx);
    }

    expect(ctx, period);
}

void field(context_t *ctx)
{
    if(accept(ctx, intconst)) {
        return;
    }else if(accept(ctx, quotas)) {
        expect(ctx, text);
        expect(ctx, quotas);
    }else {
        context_error_report(ctx, "expected intconst or string");
    }
}