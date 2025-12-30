#ifndef __WEB_INDEXING__
#define __WEB_INDEXING__
#include <stl/btree.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <stdint.h>
#include <stl/array.h>

typedef uint8_t uint8;
typedef char* str;

typedef struct {
    str name;
    str words[1024];
    size_t words_size;
    float instances[1024];
    size_t instances_size;
} doc;

static inline doc doc_init(str d_name) {
    doc result = { .name = d_name, .words = {0}, .words_size = 0,
    .instances = {0}, .instances_size = 0 };

    return result;
}

#endif
