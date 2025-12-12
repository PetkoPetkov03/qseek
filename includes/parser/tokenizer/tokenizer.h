#ifndef __PARSER_TOKENIZER__
#define __PARSER_TOKENIZER__
#include <stl/queue.h>
#include <stl/cast.h>

typedef struct __context__ context_t;

typedef struct __tokenizer__ {
    context_t* ctx;
} tokenizer_t;

tokenizer_t* init_tokenizer();

void tokenize(context_t* ctx);

void print_tokens(context_t* ctx);

void tokenizer_clean(tokenizer_t* tokenizer);

#endif
