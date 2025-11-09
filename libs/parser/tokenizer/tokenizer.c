
#include "cast.h"
#include "parser/token.h"
#include "queue.h"
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

    printf("Current Char: %c\n", ctx->cChar);

    while(ctx->cChar == ' ') context_next_char(ctx);

    switch (toupper(ctx->cChar)) {
        case 'A' - 'Z': {
            
            do {
                if(size < MAXSTRNGLEN) {
                    lexeme[size++] = toupper(ctx->cChar);
                }

                context_next_char(ctx);
            } while(((toupper(ctx->cChar) >= 'A') && (toupper(ctx->cChar) <= 'Z')) || ((ctx->cChar >= '0') && (ctx->cChar <= '9')) );

            token token = { 0 };

            token.token_type = text;
            strcpy(token.data.ident.lexeme, lexeme);

            ctx->cToken = token;

            ctx->tokens[ctx->tokensSize++] = token;
        } break;

        case '0' - '9': {
            ctx->cConstant = 0;

            do {
                digit = (unsigned int)(ctx->cChar);

                if((ctx->cConstant < (MAXINTSIZE/10)) ||
                    ((ctx->cConstant == (MAXINTSIZE/10) && (digit = (MAXINTSIZE%10)) ))) {
                        ctx->cConstant = (10 * ctx->cConstant) + digit;
                }else {
                    context_error_report(ctx, "int constant too large");
                }

                context_next_char(ctx);

                token t = { 0 };
                t.token_type = intconst;
                t.data.constant.constant = ctx->cConstant;

                ctx->cToken = t;
                ctx->tokens[ctx->tokensSize++] = t;
            } while((ctx->cChar >= '0') && (ctx->cChar <= '9'));
        } break;

        case '.': {
            token t = { 0 };

            t.token_type = period;

            context_next_char(ctx);

            ctx->cToken = t;
            ctx->tokens[ctx->tokensSize++] = t;
        } break;

        case ';': {
            token t = { 0 };

            t.token_type = semicolon;

            ctx->cToken = t;
            ctx->tokens[ctx->tokensSize++] = t;

            context_next_char(ctx);
        } break;

        case '"': {
            token t = { 0 };

            t.token_type = quotas;
            ctx->cToken = t;
            ctx->tokens[ctx->tokensSize++] = t;
            context_next_char(ctx);
        } break;

        default: {
            token t = { 0 };

            t.token_type = othersy;
            t.data.symb = ctx->cChar;

            ctx->cToken = t;
            ctx->tokens[ctx->tokensSize++] = t;

            context_next_char(ctx);
        } break;
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