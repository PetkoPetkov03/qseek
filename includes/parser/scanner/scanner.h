#ifndef __PARSER_SCANNER__
#define __PARSER_SCANNER__
#include <queue.h>

typedef struct __context__ context_t;

typedef struct __scanner__ {
  queue_t* scanner_queue;
  context_t* ctx;
} scanner_t;

scanner_t* init_scanner();

void scanner_load(scanner_t*);

char scanner_get_next_char(scanner_t*);

void scanner_clean(scanner_t *scanner);

#endif
