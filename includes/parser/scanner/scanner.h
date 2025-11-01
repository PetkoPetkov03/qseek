#ifndef __PARSER_SCANNER__
#define __PARSER_SCANNER__

#include <queue.h>

typedef struct __scanner__ {
  queue_t* scanner_queue;
  int fd;
} scanner_t;

scanner_t* init_scanner(char* file[]);

void scanner_load(scanner_t*);

char scanner_get_next_char(scanner_t*);

#endif
