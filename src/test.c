#include<stdio.h>
#include "bumpalloc.h"

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

void bulk_memset() {

    bumpalloc_t alloc = bumpalloc_create(4000000000);
    bumpalloc_debug(&alloc);
     
    for (int i = 0; i < 500000000; i++) {
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

    bumpalloc_destroy(&alloc);

    printf("bulk_memset test done\n");
}

void read_write_small() {
    bumpalloc_t alloc = bumpalloc_create(128);
    bumpalloc_debug(&alloc);
    
    sample_t*  first = (sample_t*) bumpalloc_alloc(&alloc, sizeof(sample_t));
    bumpalloc_debug(&alloc);
    first->a = 1;
    first->b = 'a';
    first->c = 1;
    print_sample(first);

    sample_t*  seccond = (sample_t*) bumpalloc_alloc(&alloc, sizeof(sample_t));
    bumpalloc_debug(&alloc);
    seccond->a = 2;
    seccond->b = 'b';
    seccond->c = 2;
    print_sample(seccond);
    bumpalloc_destroy(&alloc);

    printf("read_write_small test done \n");
}


int main(int arc, char *argv[]) {
    read_write_small();
    bulk_memset();
    
    return 0;
}