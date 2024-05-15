#include "bumpalloc_wrapper.h"
#include "bumpalloc.h"
#include <unistd.h>
#include <string.h>

#define WRITE(string)\
    write(1, string, sizeof(string))

void write_size_t(size_t val) {
    const base = 10;
    char out_all[21] = {'0'};   //max possible value = 18_446_744_073_709_551_615. therefore, max possible length is 21 (including null-terminator)
    char* out = &out_all[21];   // last non-nt character
    size_t i = 0;
    
    // zero case
    if (val == 0) {
        *--out = '0';
        i++;
    }

    // positive case
    size_t num = val;
    while (num != 0)
    {
        size_t remaindier = num % base;
        num /= base;
        *--out = remaindier + '0';  // decrement pointer and write the ascii digit
        i++;

        // never write past start of array
        if (out <= out_all) {
            break;
        }
    }

    // write from the last digit that was calculated
    write(1, out, i);
}

#define SIZE ((long long) 3000000000)
int main (int arc, char*argv[]) {
    char* ptr = (char*) malloc(SIZE);
    WRITE("test2 - allocated\n");
    for (long long i = 0; i < SIZE; i ++) {
        ptr[i] = NULL;
    }
    
    WRITE("test2 - written\n");

    if (*ptr != 0) {
        WRITE("test2 - write failed before free\n");
    }

    free((void*)ptr);  // the bump allocator should ignore this
    WRITE("test2 - freed\n");

    WRITE("test2 - checking for premature free\n");
    if (*ptr != NULL) {
        WRITE("test2- failed reading ptr \n");
        return 1;
    }

    for (long long i = 0;  i < SIZE; i++) {
        if (ptr[i] != NULL) {
            WRITE("test2 - failed reading ptr \n");
            
            write_size_t(i);
            WRITE("\n");
            return 1;
        }   
    }

    WRITE("test2 - OK\n");   
}