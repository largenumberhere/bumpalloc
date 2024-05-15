#ifndef BUMPALLOC_H
#define BUMPALLOC_H 1

/// a small modular bump allocator
#include <stddef.h>

// please forgive me lord, for I have written the code in the header file

typedef struct {
    void* start;
    void* end;
    void* next;
} bumpalloc_t;

// Create the bump allocator with the given size in the next part of the heap. 
// Only one bump allocator is supported at a time 
extern bumpalloc_t bumpalloc_create(size_t heap_size);

// show the state of the allocator
extern void bumpalloc_debug(bumpalloc_t* bump_alloc);
// release all resources, this also frees all allocations

extern void bumpalloc_destroy(bumpalloc_t* bump_alloc);

// slice up the next region of memory and hand you a pointer or NULL if there's no room left.
extern void* bumpalloc_alloc(bumpalloc_t* bump_alloc, size_t size);

// The memory shall be leaked, as god intended (bump allocators don't support freeing one thing at a time)
// This is just to make this allocator more api-friendly
extern void bumpalloc_free(bumpalloc_t bumpalloc, void* ptr);

#endif