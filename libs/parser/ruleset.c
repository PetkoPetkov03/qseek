#include "parser/analyzer/analyzer.h"
#include <parser/tokenizer/tokenizer.h>
#include "parser/token.h"
#include <parser/ruleset.h>
#include <parser/context.h>
#include <string.h>

void hostname(context_t *ctx)
{
    while(accept(ctx, dotseparator)) {
        expect(ctx, xalphas);
    }
}

void hostip(context_t *ctx)
{
  while(accept(ctx, dotseparator)) {
    expect(ctx, digits);
  }
}

void port(context_t *ctx)
{
    printf("port called\n");
    print_token(ctx->cToken);
    if(accept(ctx, column)) {
        printf("COLUMN ACCEPTED\n");
        //print_token(ctx->cToken);
        printf("PORT STARTS\n");
        expect(ctx, digits);
    }
}

void hostport(context_t *ctx)
{
    printf("hostport start\n");

    if(accept(ctx, xalphas)) {
      hostname(ctx);
    }else if(accept(ctx, digits)) {
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
    while(accept(ctx, separator)) {
        accept(ctx, xpalphas);
        accept(ctx, xalphas);
        accept(ctx, digits);
        accept(ctx, safe);
        accept(ctx, extra);
        accept(ctx, esc);
    }
}

void http(context_t *ctx)
{
    if(accept(ctx, column)) {
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
    if(accept(ctx, httpd)) {
        printf("HTTP START\n");
        http(ctx);
    }else if(accept(ctx, tcp)) {

    } else {
      context_error_report(ctx, "Unsuported protocol");
    }
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


void record(context_t *ctx)
{
    field(ctx);

    while(ctx->cToken.token_type == semicolon) {
        accept(ctx, semicolon);
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
        printf("URL START\n");
        tokenize(ctx);

        url(ctx);
    }
}
