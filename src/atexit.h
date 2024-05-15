#ifndef ATEXIT_H
    #define ATEXIT_H 1

    // expose only atexit from stdlib.h

    extern int atexit(void (*func)(void));

#endif