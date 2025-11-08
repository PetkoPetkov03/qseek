#ifndef __PARSER_ANALYZER__
#define __PARSER_ANALYZER__
#include <parser/token.h>

typedef struct __context__ context_t;

typedef struct __analyzer__ {
    context_t* ctx;
    token_t symbol;
} analyzer_t;

analyzer_t* init_analyzer();
int accept(analyzer_t*);
int expect(analyzer_t*);

void analyzer_clean(analyzer_t*);

#endif
