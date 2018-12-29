/*
* Copyright (c) 2018-2019 Ruben N
* This library is free software; you can redistribute it and/or modify it under
* the terms of The MIT License (opensource.org/licenses/MIT).
*/
#ifndef _RVEC_H
#define _RVEC_H

#include <stdlib.h>

#ifndef RVEC_MALLOC
#   define RVEC_MALLOC malloc
#endif 

#ifndef RVEC_FREE
#   define RVEC_FREE free
#endif

#ifndef RVEC_REALLOC
#   define RVEC_REALLOC realloc
#endif

static inline void _rvec_cleanup(void *v) { if (*(void**)v) RVEC_FREE(*(void**)v); }

#ifndef RVEC_NO_RAII
#   define _RVEC_RAII __attribute__((cleanup(_rvec_cleanup)))
#else
#   define _RVEC_RAII
#endif // RVEC_NO_RAII


#define rvec_t(type) _RVEC_RAII struct __attribute__((__packed__)) \
    { size_t capacity, size; type data[]; } *
 
#define rvec_init(v) ( ( (v) = RVEC_MALLOC(sizeof(*(v))) ) \
    ? ((v)->capacity = (v)->size = 0) : -1 )

#define rvec_size(v) ((v)->size)

#define rvec_capacity(v) ((v)->capacity)

#define rvec_pop(v) ((v)->data[--(v)->size])

#define rvec_i(v,i) ((v)->data[(i)])

#define rvec_begin(v) (&rvec_i((v),0))

#define rvec_end(v) (&rvec_i((v),rvec_size(v)))

#define rvec_set(v,i,x) (rvec_i((v), (i)) = (x))

#define rvec_get(v,i) (rvec_i((v), (i)))

static inline int _rv_adjust_cap(void **v, size_t item_size, size_t new_capacity) {
    size_t capacity = *(size_t *)*v;
    if(!new_capacity) new_capacity = capacity ? (capacity << 1) : 8;
    void *ptr = RVEC_REALLOC(*v, (sizeof(size_t) * 2) + (new_capacity * item_size) );
    if(!ptr) return -1;
    *v = ptr;
    (**(size_t**)v) = new_capacity;
    size_t *vec_size_ptr = (size_t*)(*v + sizeof(size_t));
    if(*vec_size_ptr > new_capacity) *vec_size_ptr = new_capacity;
    return 0;
}

#define _rv_assign(v,x) ((v)->data[(v)->size++] = (x), 0 )

#define rvec_push(v,x) ( ((v)->capacity != (v)->size) \
    ? _rv_assign((v), (x)) \
    : (!_rv_adjust_cap((void**)&(v), sizeof((v)->data[0]), 0 )) \
    ? _rv_assign((v), (x)) \
    : -1 )

#define rvec_resize(v,sz) (_rv_adjust_cap((void**)&(v), sizeof((v)->data[0]), (sz)))

#define rvec_copy(dest,src) do { \
    int r = 0; \
    size_t item_size = sizeof((dest)->data[0]); \
    if((dest)->capacity < (src)->size) \
        r = _rv_adjust_cap( (void**)&(dest), item_size, (src)->size ); \
    if(r != 1) { \
        memcpy((dest)->data, (src)->data, item_size * (src)->size); \
        (dest)->size = (src)->size; \
    } \
} while (0)

#define rvec_destroy(v) (RVEC_FREE((v)))

#endif // _RVEC_H