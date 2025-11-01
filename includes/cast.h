#ifndef __CAST_MACROS__

#define __CAST_MACROS__

#define cast(func, cast_t, ...) (*(cast_t*)(func(__VA_ARGS__)))
#define castp(func, cast_t, ...) ((cast_t*)(func(__VA_ARGS__)))

#define cast_safe(func, cast_t, ...) \
  ({ void* _tmp = (func)(__VA_ARGS__); _tmp ? *(cast_t*)_tmp : (cast_t){0}; })

#endif
