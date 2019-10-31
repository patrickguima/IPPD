#include <time.h>


struct cronometer {
    clock_t start;
    clock_t end;
}; 

struct cronometer init_cronometer();

struct cronometer start(struct cronometer c);

struct cronometer end(struct cronometer c);

struct cronometer reset(struct cronometer c);

double diff(struct cronometer c);

