
#include "cast.h"
#include "parser/token.h"
#include <ctype.h>
#include <parser/tokenizer/tokenizer.h>
#include <parser/scanner/scanner.h>
#include <parser/context.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

tokenizer_t* init_tokenizer()
{
    tokenizer_t* tokenizer = castp(malloc, tokenizer_t, sizeof(tokenizer_t));

    return tokenizer;
}

void tokenize(context_t* ctx) 
{
    char lexeme[MAXSTRNGLEN] = { 0 };
    int digit = 0;
    size_t size = 0;

    if(ctx->cChar == '\0') {
        context_next_char(ctx);
    }

    if(isalpha(ctx->cChar)) {
        int strSizeFlag = 0;
        do {
            if(size < MAXSTRNGLEN) {
                lexeme[size++] = toupper(ctx->cChar);
            }

            if(size >= MAXSTRNGLEN && strSizeFlag == 0) {
                context_error_report(ctx, "String overflow: array too large max 8 bytes");
                strSizeFlag = 1;
            }

            context_next_char(ctx);
        } while(isalnum(ctx->cChar));

        token t = {0};
        t.token_type = text;
        strcpy(t.data.ident.lexeme, lexeme);
        ctx->cToken = t;
        ctx->tokens[ctx->tokensSize++] = t;
    } 
    else if(isdigit(ctx->cChar)) {
        int digitTooLargeFlag = 0;
        ctx->cConstant = 0;
        do {
            digit = ctx->cChar - '0';
            if((ctx->cConstant < (MAXINTSIZE/10)) ||
               ((ctx->cConstant == (MAXINTSIZE/10)) && (digit <= (MAXINTSIZE%10)))) {
                ctx->cConstant = (10 * ctx->cConstant) + digit;
            } else {
                if(digitTooLargeFlag == 0) {
                    context_error_report(ctx, "int constant too large");
                    digitTooLargeFlag = 1;
                }
            }
            context_next_char(ctx);
        } while(isdigit(ctx->cChar));

        token t = {0};
        t.token_type = intconst;
        t.data.constant.constant = ctx->cConstant;
        ctx->cToken = t;
        ctx->tokens[ctx->tokensSize++] = t;
    } 
    else if(ctx->cChar == '.') {
        token t = {0};
        t.token_type = period;
        ctx->cToken = t;
        ctx->tokens[ctx->tokensSize++] = t;
        context_next_char(ctx);
    } 
    else if(ctx->cChar == ';') {
        token t = {0};
        t.token_type = semicolon;
        ctx->cToken = t;
        ctx->tokens[ctx->tokensSize++] = t;
        context_next_char(ctx);
    } 
    else if(ctx->cChar == '"') {
        token t = {0};
        t.token_type = quotas;
        ctx->cToken = t;
        ctx->tokens[ctx->tokensSize++] = t;
        context_next_char(ctx);
    } 
    else {
        token t = {0};
        t.token_type = othersy;
        t.data.symb = ctx->cChar;
        ctx->cToken = t;
        ctx->tokens[ctx->tokensSize++] = t;
        context_next_char(ctx);
    }
}

void print_tokens(context_t* ctx)
{
    while(ctx->tokensSize > ctx->tokensIndex+1) {
        token t = ctx->tokens[ctx->tokensIndex++];

        print_token(t);
    }

    ctx->tokensIndex = 0;
}

void tokenizer_clean(tokenizer_t *tokenizer)
{
    free(tokenizer);
}