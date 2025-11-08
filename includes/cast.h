#ifndef __CAST_MACROS__

#define __CAST_MACROS__
#include <stdlib.h>

#define cast(func, cast_t, ...) (*(cast_t*)(func(__VA_ARGS__)))
#define castp(func, cast_t, ...) ((cast_t*)(func(__VA_ARGS__)))

#define cast_safe(func, cast_t, ...) \
  ({ void* _tmp = (func)(__VA_ARGS__); \
     cast_t _val = _tmp ? *(cast_t*)_tmp : (cast_t){0};\
      free(_tmp);\
      _val; \
    })

#endif
