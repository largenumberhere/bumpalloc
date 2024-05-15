
#ifndef BUMPALLOC_WRAPPER_H
    #include "bumpalloc.h"
    #include "atexit.h"
    #define BUMPALLOC_WRAPPER_H 1

    // set the bump allocator to be called by `malloc()` and `free()` and initalizes it where appropriate. 
    // Implementation details are abstracted away for convieniennce

    // The malloc function, replaces the built-in one
    extern void *b_malloc(long long size);   

    // The free function, replaces the built-in one
    extern void b_free(void *ptr);

    #define free(ptr)\
        b_free(ptr)

    #define malloc(size)\
        b_malloc(size)

#endif