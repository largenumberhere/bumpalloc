# bumpalloc
A simple bump allocator written in C for linux with x86_64 CPU.
Bump allocators are extremely fast because they have near-0 overhead, however they are also very niche, since they cannot free a single allocation.

### Compile
Requires the make build system and a PC with 4GB of ram Run the following command. It will take a while, because several tests will run, if any fail every time, your platform may not be supported.
```
make
```

The generated `.a` library files and `.h` headers will be generated and put into a `lib` folder. 
To use the bump allocator, there is 2 distinct approaches.

1. With the malloc wrapper. Put the files into your project folder and link them.
Then `malloc()` and `free()` can be used like normal, except that free does nothing.
    ```
    # linking example
    gcc main.c -Lbumpalloc_wrapper.h bumpalloc_wrapper.a -Lbumpalloc.h bumpalloc.a -Latexit.h 
    ```

2. Raw - Put the files into your project folder and link them.
You are then required to initalize `bumpalloc_create` and take a pointer to it for all other functions. `bumpalloc_alloc` is used to allocate, `bumpalloc_free` does nothing and `bumpalloc_destroy` deallocates everything. Consult `bumpalloc.h` for the function documentation.
    ```
    #linking example
    gcc main.c -Lbumpalloc.h bumpalloc.a 
    ```

 