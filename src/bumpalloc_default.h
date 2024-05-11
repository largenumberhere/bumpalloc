// set the bump allocator to be called by `malloc()` and `free()` and initalizes it where appropriate. 
// Implementation details are abstracted away for convieniennce

// The malloc function, replaces the built-in one
extern void *malloc(long long size);   

// The free function, replaces the built-in one
extern void free(void *ptr);