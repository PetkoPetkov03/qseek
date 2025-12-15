#include "parser/analyzer/analyzer.h"
#include <parser/tokenizer/tokenizer.h>
#include "parser/token.h"
#include <parser/ruleset.h>
#include <parser/context.h>
#include <string.h>

void hostname(context_t *ctx)
{
    while(accept_token(ctx, dotseparator)) {
        expect(ctx, xalphas);
    }
}

void hostip(context_t *ctx)
{
  while(accept_token(ctx, dotseparator)) {
    expect(ctx, digits);
  }
}

void port(context_t *ctx)
{
    print_token(ctx->cToken);
    if(accept_token(ctx, column)) {
        expect(ctx, digits);
    }
}

void hostport(context_t *ctx)
{
    if(accept_token(ctx, xalphas)) {
      hostname(ctx);
    }else if(accept_token(ctx, digits)) {
      hostip(ctx);
    }else {
      context_error_report(ctx, "host pattern unrecognized");
    }

    if(ctx->cToken.token_type == column) {
        port(ctx);
    }
}

void path(context_t *ctx)
{
    while(accept_token(ctx, separator)) {
        accept_token(ctx, xpalphas);
        accept_token(ctx, xalphas);
        accept_token(ctx, digits);
        accept_token(ctx, safe);
        accept_token(ctx, extra);
        accept_token(ctx, esc);
    }
}

void http(context_t *ctx)
{
    if(accept_token(ctx, column)) {
        expect(ctx, separator);
        expect(ctx, separator);

        hostport(ctx);

        path(ctx);
    }else {
        context_error_report(ctx, "HTTP format error");
    }
}

void url(context_t *ctx)
{
    if(accept_token(ctx, httpd)) {
        http(ctx);
    }else if(accept_token(ctx, tcp)) {

    } else {
      context_error_report(ctx, "Unsuported protocol");
    }
}

void field(context_t *ctx)
{
    if(accept_token(ctx, intconst)) {
        return;
    }else if(accept_token(ctx, quotas)) {
        expect(ctx, text);
        expect(ctx, quotas);
    }else {
        context_error_report(ctx, "expected intconst or string");
    }
}


void record(context_t *ctx)
{
    field(ctx);

    while(ctx->cToken.token_type == semicolon) {
        accept_token(ctx, semicolon);
        field(ctx);
    }

    expect(ctx, period);
}

void file(context_t *ctx)
{
    record(ctx);

    while ((ctx->cToken.token_type == intconst)
    || (ctx->cToken.token_type == quotas)) {
        record(ctx);
    }
}


void parse_func(context_t *ctx)
{
    if(ctx->parser_type == DEFAULT) {
        tokenize(ctx);
        file(ctx);
    }

    if(ctx->parser_type == URL) {
        tokenize(ctx);

        url(ctx);
    }
}
