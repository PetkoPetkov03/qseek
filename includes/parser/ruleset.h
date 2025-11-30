#ifndef __RULESET__

typedef struct __context__ context_t;

void parse_func(context_t *ctx);

void file(context_t* ctx);
void record(context_t* ctx);
void field(context_t* ctx);

#endif
