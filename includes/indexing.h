#ifndef __WEB_INDEXING__
#define __WEB_INDEXING__

#include <stl/hashset.h>
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

#define DEFAULT_STACK_SIZE 1024

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef char* str;

typedef struct {
    str name;
    str words[DEFAULT_STACK_SIZE];
    size_t words_size;
    float instances[DEFAULT_STACK_SIZE];
    size_t instances_size;
} doc;

static inline doc* doc_init(str d_name) {
    doc *result = (doc*)malloc(sizeof(doc));
    (result->name) = (char*)malloc(strlen(d_name)+1);
    strcpy((result->name), d_name);

    return result;
}

typedef struct {
    uint16 index;
    str name;
    str* words;
    size_t wordSize;
    uint16* instances;
    size_t instancesSize;
} IndexingValue;

typedef struct {
    IndexingValue** documents;
    size_t documentSize;
} IndexingQueue;

IndexingQueue* init_indexingq();

void add_documents(IndexingQueue* queue, doc** docs, size_t docCount);

void indexing_print(IndexingQueue* queue);

void free_indexingq(IndexingQueue* queue);

#endif
