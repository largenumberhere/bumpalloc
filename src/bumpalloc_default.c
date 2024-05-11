#include "bumpalloc.h"
#include <stdbool.h>
#include "atexit.h"

static size_t HEAP_MAX_SIZE = 4000000000;  //approx 4GB, exact size doesn't matter since linux uses optimistic memory allocation

typedef struct
{
    int is_initalized;                      // zerod by the compiler
    bumpalloc_t bumpalloc; 
}  globalalloc_t ;

// The allocator's state
globalalloc_t globalalloc;


// Not strictly neccesary since the kernel will clean up after exit anyway, 
//  but it is nice to have for completeness and looks more professional
void handle_exit() {
    bumpalloc_destroy(&globalalloc.bumpalloc);
    globalalloc.is_initalized = 0;
}


void *malloc(size_t size) {
    //init is required
    if (globalalloc.is_initalized == 0) {
        bumpalloc_t bumpalloc = bumpalloc_create(HEAP_MAX_SIZE);
        globalalloc.bumpalloc = bumpalloc;
        globalalloc.is_initalized = 1;

        atexit(handle_exit);    
    }

    void* mem = bumpalloc_alloc(&globalalloc.bumpalloc, size);
    return mem;
}

// The free function
void free(void* ptr) {
    if (globalalloc.is_initalized) {
        bumpalloc_free(globalalloc.bumpalloc, ptr);
    }
}



