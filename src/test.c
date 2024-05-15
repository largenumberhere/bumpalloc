#include <stdio.h>
#include "bumpalloc.h"
#include <stdbool.h>
#include <stdlib.h>


void exit_error() {
    exit(1);
}



// valid states: {1,'a',1}, {2, 'b', 2}
typedef struct {
    int a;      //1-4
    char b;     //5-5
                // padding to align for size_t
    size_t c;   //8-16
} sample_t;


void print_sample(sample_t* sample) {
    printf("sample_t {\n");
    printf("    a = %i\n",sample->a);
    printf("    b = %c\n", sample->b);
    printf("    c = %ld\n", sample->c);
    printf("}\n");

}

#define HEAP_SIZE ((size_t)4000000000)
#define MAX_MEOWS ((int) (HEAP_SIZE/8))

void bulk_memset() {
    bumpalloc_t alloc = bumpalloc_create(HEAP_SIZE);
    if (alloc.start != alloc.next) {
        bumpalloc_debug(&alloc);
        printf("test1 -bumpalloc did not start at the beginning\n");
        exit_error();
    }
    if (alloc.end - alloc.start != HEAP_SIZE) {
        bumpalloc_debug(&alloc);
        printf("test1 -bumpalloc did no reserve the correct size\n");
        exit_error();
    }
    
    // write
    char* mem0 = alloc.start;
    
    for (int i = 0; i < MAX_MEOWS; i++) {
        char* mem = bumpalloc_alloc(&alloc, 8);
        
        *(mem+0) = 'm';
        *(mem+1) = 'e';
        *(mem+2) = 'o';
        *(mem+3) = 'w';
        
        *(mem+4) = 'M';
        *(mem+5) = 'E';
        *(mem+6) = 'O';
        *(mem+7) = 'W';
    }

   if (alloc.start > alloc.end) {
        bumpalloc_debug(&alloc);
        printf("test1 -Allocator was greater than last pointer\n");
        exit_error();
    }

    if (alloc.next != alloc.end) {
        bumpalloc_debug(&alloc);
        printf("test1 -Allocator did not get to end\n");
        exit_error();
    }

    bumpalloc_destroy(&alloc);

    printf("test1 -bulk_memset test done\n");
}

void read_write_small() {
    #define WWS_HEAP ((size_t) 128)

    bumpalloc_t alloc = bumpalloc_create(WWS_HEAP);
    //bumpalloc_debug(&alloc);
    
    if (alloc.next != alloc.start) {
        bumpalloc_debug(&alloc);
        printf("test1 -Allocator was not being at the start or end. This CPU's architechture may be unsupported\n");
        exit_error();
    }
    
    if (alloc.start > alloc.end) {
        bumpalloc_debug(&alloc);
        printf("test1 -Allocator was greater than last pointer\n");
        exit_error();
    }

    if (alloc.start + WWS_HEAP != alloc.end) {
        bumpalloc_debug(&alloc);
        printf("test1 -Allocator did not reserve the correct size\n");
        exit_error();
    }

    sample_t*  first = (sample_t*) bumpalloc_alloc(&alloc, sizeof(sample_t));
    
    if (alloc.start > alloc.end) {
        bumpalloc_debug(&alloc);
        printf("test1 -Allocator was greater than last pointer\n");
        exit_error();
    }

    if (alloc.next + WWS_HEAP - sizeof(sample_t)  != alloc.end) {
        bumpalloc_debug(&alloc);
        printf("test1 -Allocator did not reserve the correct size\n");
        exit_error();
    }

    first->a = 1;
    first->b = 'a';
    first->c = 1;
    if 
    (   first->a!= 1 |
        first->b!='a' |
        first->c!=1 
    ) {
        print_sample(first);
        printf("test1 -The sample was corrupted. read_write_small test failed!\n");
        exit_error();
    }

    sample_t*  seccond = (sample_t*) bumpalloc_alloc(&alloc, sizeof(sample_t));
    
    if (alloc.start > alloc.end) {
        bumpalloc_debug(&alloc);
        printf("test1 -Allocator was greater than last pointer\n");
        exit_error();
    }

    if (alloc.next + WWS_HEAP - (sizeof(sample_t)*2)  != alloc.end) {
        bumpalloc_debug(&alloc);
        printf("test1 -Allocator did not reserve the correct size\n");
        exit_error();
    }

    seccond->a = 2;
    seccond->b = 'b';
    seccond->c = 2;
    if 
    (   seccond->a!= 2 |
        seccond->b!= 'b' |
        seccond->c!= 2 
    ) {
        print_sample(seccond);
        printf("test1 -The sample was corrupted. read_write_small test failed!\n");
        exit_error();
    }

    bumpalloc_destroy(&alloc);

    printf("test1 -read_write_small test done\n");
}


int main(int arc, char *argv[]) {
    read_write_small();
    bulk_memset();
    
    printf("test1 - OK\n");
    return 0;
}