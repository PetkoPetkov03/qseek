#include <asm-generic/errno-base.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <parser/context.h>
#include "parser/analyzer/analyzer.h"
#include "parser/token.h"
#include "queue.h"
#include <parser/scanner/scanner.h>
#include <parser/tokenizer/tokenizer.h>
#include <stdlib.h>
#include <cast.h>
#include <string.h>

context_t context_init(const char* file_path) 
{
    context_t ctx;

    ctx.file_path = file_path;
    ctx.error_count = 0;

    ctx.cChar = '\0';
    memset(ctx.tokens, 0, MAXTOKENS*sizeof(token));
    ctx.tokensIndex = 0;
    ctx.tokensSize = 0;

    ctx.scanner = NULL;
    ctx.tokenizer = NULL;

    return ctx;
}

void context_next_char(context_t *ctx)
{
    scanner_t* scanner = ctx->scanner;

    char temp = scanner_get_next_char(scanner);

    ctx->cChar = temp;
}

void link_analyzer_instance(context_t *ctx, analyzer_t *analyzer) 
{
    ctx->analyzer = analyzer;
    analyzer->ctx = ctx;
}

void link_scanner_instance(context_t *ctx, scanner_t *scanner) 
{
    ctx->scanner = scanner;
    scanner->ctx = ctx;
}

void link_tokenizer_instance(context_t *ctx, tokenizer_t *tokenizer) 
{
    ctx->tokenizer = tokenizer;
    tokenizer->ctx = ctx;
}

void context_error_report(context_t *ctx, const char *message)
{
    ctx->errors[ctx->error_count++] = (char*)message;
}

int context_is_error_trown(context_t *ctx) 
{
    if(ctx->error_count > 0) return 1;
    return 0;
}

void context_clean(context_t *context)
{

    if(context->scanner) {
        scanner_clean(context->scanner);
    }

    if(context->tokenizer) {
        tokenizer_clean(context->tokenizer);    
    }

    if(context->analyzer) {
        analyzer_clean(context->analyzer);
    }

}

void abort_and_dump(context_t *context)
{

    printf("Error breakdown: %i errors thrown\n", context->error_count);
    for(int i = 0; i < context->error_count; i++) {
        fprintf(stderr, "%s: %s\n", context->errors[i], strerror(EPERM));
    }

    context_clean(context);

    exit(EXIT_FAILURE);
}