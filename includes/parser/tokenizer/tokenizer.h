#ifndef __PARSER_TOKENIZER__
#define __PARSER_TOKENIZER__
#include <queue.h>
#include <cast.h>

typedef struct __context__ context_t;

typedef struct __tokenizer__ {
    queue_t* tokens;
    context_t* ctx;
} tokenizer_t;

tokenizer_t* init_tokenizer();

void tokenize(tokenizer_t* tokenizer);

void print_tokens(tokenizer_t* tokenizer);

void tokenizer_clean(tokenizer_t* tokenizer);

#endif
