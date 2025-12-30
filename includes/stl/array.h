#ifndef __STL_ARRAY__
#define __STL_ARRAY__

#include <stdlib.h>
#include <stddef.h>

typedef struct {
    void* data;
    size_t size;
} ArrayWrapper;


// inline static void array_push(ArrayWrapper *wrapper, void* item)
// {
//     ++(wrapper->size);
//     if(wrapper->data == NULL) {
//         wrapper->data = malloc(sizeof(wrapper->data)*wrapper->size);
//     } else {
//         wrapper->data = realloc(wrapper->data,
//         sizeof(wrapper->data) * wrapper->size);
//     }
//     wrapper->data[wrapper->size-1] = item;
// }

// inline static ArrayWrapper Array(void** items, size_t size)
// {
//     ArrayWrapper wrapper = {0,0};
//     if(items != NULL) {
//         for(size_t i = 0; i < size; i++) {
//             array_push(&wrapper, items[i]);
//         }
//     }

//     return wrapper;
// }

// inline static void* array_get(ArrayWrapper *wrapper, size_t index)
// {
//     if(index >= wrapper->size) {
//         return NULL;
//     }

//     return wrapper->data[index];
// }

// inline static size_t array_len(ArrayWrapper wrapper)
// {
//     return wrapper.size;
// }
#endif
