
#ifndef __DEB__
#define __DEB__

#include <stdint.h>

typedef struct {
    uint16_t state;
    uint16_t c0;
    uint16_t c1;
    uint16_t toggle;
} debounce_t;

uint16_t debouncer(uint16_t sample, debounce_t* p);

#endif

