#ifndef __PARSER_CONTEXT__
#define __PARSER_CONTEXT__
#include "parser/analyzer/analyzer.h"
#include <parser/token.h>
#include <parser/tokenizer/tokenizer.h>
#include <parser/scanner/scanner.h>
#include <stddef.h>

typedef struct __scanner__ scanner_t;
typedef struct __tokenizer__ tokenizer_t;

#define MAXTOKENS 100000

typedef enum {
    DEFAULT,
    URL,
    HTML
} parser_t;

typedef struct __context__ {
    scanner_t* scanner;
    tokenizer_t* tokenizer;
    analyzer_t* analyzer;

    token tokens[MAXTOKENS];
    size_t tokensSize;
    size_t tokensIndex;

    token cToken;

    char cChar;
    int cConstant;

    const char* file_path;

    int error_count;
    char* errors[100];

    parser_t parser_type;
} context_t;

context_t context_init(const char* file_path);

void set_mode(context_t* ctx, parser_t ptype);

void context_next_char(context_t* ctx);

void context_next_symbol(context_t* ctx);

token context_next_token(context_t* context);

void link_analyzer_instance(context_t* ctx, analyzer_t* analyzer);

void link_scanner_instance(context_t* ctx, scanner_t* scanner);

void link_tokenizer_instance(context_t* ctx, tokenizer_t* tokenizer);

void context_error_report(context_t* ctx, const char* message);

int context_is_errors_thrown(context_t* ctx);

void abort_and_dump(context_t* context);

void context_clean(context_t *context);

inline static void configure_linking(context_t *ctx)
{
    tokenizer_t* tokenizer = init_tokenizer();
    scanner_t* scanner = init_scanner();
    analyzer_t* analyzer = init_analyzer();

    link_scanner_instance(ctx, scanner);
    link_tokenizer_instance(ctx, tokenizer);
    link_analyzer_instance(ctx, analyzer);

    scanner_load(scanner);
}

#endif
