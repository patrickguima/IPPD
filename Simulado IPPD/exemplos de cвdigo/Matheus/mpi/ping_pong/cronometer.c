#include "cronometer.h"
#include <time.h>
#include <stdlib.h>

struct cronometer init_cronometer () {
    struct cronometer c;
    c.start = -1;
    c.end = -1;
    return c;
}

struct cronometer start (struct cronometer c) {
    c.start = clock();
    return c;
}

struct cronometer end (struct cronometer c) {
    c.end = clock();
    return c;
}

struct cronometer reset (struct cronometer c) {
    c.start = 0;
    c.end = 0;
    return c;
}

double diff(struct cronometer c) {
    double cpu_time = ((double) (c.end - c.start)) / CLOCKS_PER_SEC;    
    return cpu_time;
}
