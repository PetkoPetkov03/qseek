#ifndef __PARSER_CONTEXT__
#define __PARSER_CONTEXT__
#include "parser/analyzer/analyzer.h"
#include <parser/token.h>

typedef struct __scanner__ scanner_t;
typedef struct __tokenizer__ tokenizer_t;

typedef struct __context__ {
    scanner_t* scanner;
    tokenizer_t* tokenizer;
    analyzer_t* analyzer;
    token current_token;

    const char* file_path;

    int error_count;
    char* errors[100];
} context_t;

context_t context_init(const char* file_path);

token context_next_token(context_t* context);

void link_analyzer_instance(context_t* ctx, analyzer_t* analyzer);

void link_scanner_instance(context_t* ctx, scanner_t* scanner);

void link_tokenizer_instance(context_t* ctx, tokenizer_t* tokenizer);

void context_error_report(context_t* ctx, const char* message);

int context_is_error_trown(context_t* ctx);

void abort_and_dump(context_t* context);

void context_clean(context_t *context);

#endif
