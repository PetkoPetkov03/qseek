#ifndef __HASHSET__
#define __HASHSET__

#include <stddef.h>

#define MAXBUFFERSIZE 1000

typedef struct __hash_pair__ {
    void* key;
    void* value;
} Hash_Pair;

typedef struct __hash_set__ {
    Hash_Pair buffer[MAXBUFFERSIZE];
} Hash_Set;

#define hset_store(set, key, value) do {\
hset_store_impl(set, key, value, 0);\
} while(0);

void hset_store_impl(Hash_Set* set,
const void* key, void* value, size_t offset);

void* hset_get(Hash_Set* set, const void* key);

#endif
