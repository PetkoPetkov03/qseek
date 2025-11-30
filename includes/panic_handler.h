#ifndef __PANIC_HANDLER__
#define __PANIC_HANDLER__

#include <stdio.h>
#include <stdlib.h>

#define CALLPANIC(msg) do { \
fprintf(stderr, "PANIC: %s:%d %s\n", __FILE__, __LINE__, msg); \
abort(); \
} while(0);

#endif
