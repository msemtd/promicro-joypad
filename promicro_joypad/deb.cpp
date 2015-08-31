
 
#include "deb.h"

uint16_t debouncer(uint16_t sample, debounce_t* p)
{
    if(!p)
        return 0;
    uint16_t delta;
    delta = sample ^ p->state;
    p->c1 = (p->c1 ^ p->c0) & delta;
    p->c0 = ~(p->c0) & delta;

    p->toggle = delta & ~(p->c0 | p->c1);
    p->state ^= p->toggle;

    return p->state;
}

