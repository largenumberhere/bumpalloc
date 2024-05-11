// expose only atexit from stdlib.h

#include <stdlib.h>
extern int atexit(void (*func)(void));

