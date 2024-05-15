/// a small modular bump allocator

#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "atexit.h"

typedef struct {
    void* start;
    void* end;
    void* next;
} bumpalloc_t;

// Create a bump allocator with the given size in the next part of the heap. 
// Only one bump allocator is supported at a time 
extern bumpalloc_t bumpalloc_create(size_t heap_size) {
    void* brkval1 = sbrk(heap_size);  // make room for heap and get start of heap, larger number
    void* brkval2 = sbrk((size_t) NULL);       // get end of heap, smaller number

    void* start   = brkval1;
    void* next    = brkval1;
    void* end     = brkval2;

    bumpalloc_t bumpalloc = {
        .start = start,
        .end = end,
        .next = next
    };

    return bumpalloc;
}

// show the state of the allocator
extern void bumpalloc_debug(bumpalloc_t* bump_alloc) {
    printf("bumpalloc_t {\n");
    printf("    start = %p\n", bump_alloc->start);
    printf("    next  = %p\n", bump_alloc->next);
    printf("    end   = %p\n", bump_alloc->end);
    printf("}\n");
}

// release all resources, this also frees all allocations
extern void bumpalloc_destroy(bumpalloc_t* bump_alloc){
    // move the break back to where it was before
    sbrk(bump_alloc->start - bump_alloc->end);  // Will cause more than one bump allocator at a time cause Undefined behaviour

    // good housekeeping
    bump_alloc->start = NULL;
    bump_alloc->next = NULL;
    bump_alloc->end = NULL;
}

// slice up the next region of memory and hand you a pointer or NULL if there's no room left.
extern void* bumpalloc_alloc(bumpalloc_t* bump_alloc, size_t size){
    void* mem = bump_alloc->next;
    void* new_next = bump_alloc->next + size;
    
    size_t max_addr = (size_t) new_next;
    if ((void*)max_addr > bump_alloc->end) {
        bumpalloc_debug(bump_alloc);
        printf("bumpalloc_alloc Heap exhausted at %p. max_addr=%p, size=%ld\n", (void*) bump_alloc->end, (void*) max_addr, size);
        return NULL;
    }

    bump_alloc->next = new_next;

    // zero the carved out memory so it's easier to work with
    memset(mem, '\0', size);

    return mem;
}

// The memory shall be leaked, as god intended (bump allocators don't support freeing one thing at a time)
// This is just to make this allocator more api-friendly
extern void bumpalloc_free(bumpalloc_t bumpalloc, void* ptr){}