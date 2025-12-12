#include <stl/hashset.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t hash(void* value, size_t vsize)
{
    unsigned char* bytes = (unsigned char*)value;
    int msize = 0;

    for(size_t i = 0; i < vsize; i++) {
        msize += (bytes[i] + i);
    }

    return (size_t)((msize + vsize) % (MAXBUFFERSIZE));
}

void hset_store_impl(Hash_Set* set, void* key, void* value, size_t offset)
{
    size_t hashv = (hash(key, sizeof(key)/sizeof(key[0])) + offset);
    if(hashv >= MAXBUFFERSIZE) {
        fprintf(stderr, "Hash out of bounds %d\n", hash);
        exit(EXIT_FAILURE);
    };

    Hash_Pair value_index = set->buffer[hashv];

    if(value_index.key != NULL) {
        hset_store_impl(set, key, value, (offset+1));
        return;
    }

    Hash_Pair hpair = {0};
    hpair.key = key;
    hpair.value = value;

    set->buffer[hashv] = hpair;
}

void* hset_get(Hash_Set* set, void* key)
{
    size_t hashv = (hash(key, sizeof(key)/sizeof(key[0])));

    if(hashv >= MAXBUFFERSIZE) {
        return NULL;
    }

    Hash_Pair value_index = set->buffer[hashv];

    while(strcmp((char*)key, (char*)value_index.key) != 0) {
        value_index = set->buffer[hashv+1];
    }

    return value_index.value;
}
