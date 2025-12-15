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
    char* key_stringified = (char*)key;
    size_t hashv = (hash(key,
    sizeof(key_stringified[0])/sizeof(key_stringified)) + offset);

    if(hashv >= MAXBUFFERSIZE) {
        fprintf(stderr, "Hash out of bounds %ld\n", hashv);
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

int hashpair_isnull(Hash_Pair hp)
{
    Hash_Pair npair = {0};
    if(hp.key == npair.key && hp.value == npair.value) return 0;
    return 1;
}

void* hset_get(Hash_Set* set, void* key)
{
    char* key_stringified = (char*)key;
    size_t hashv = (hash(key,
    sizeof(key_stringified[0])/sizeof(key_stringified)));

    if(hashv >= MAXBUFFERSIZE) {
        return NULL;
    }

    Hash_Pair value_index = set->buffer[hashv];

    int loop_index = 0;
    while(strcmp((char*)key, (char*)value_index.key) != 0) {
        loop_index++;
        if(loop_index >= MAXBUFFERSIZE) {
            fprintf(stderr, "Element not found: index[%d]\n", loop_index);
            return NULL;
        }

        value_index = set->buffer[hashv+1];
    }

    return value_index.value;
}
