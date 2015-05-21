#ifndef GRAIN_WRAPPER
#define GRAIN_WRAPPER

#include "grain.h"
#include <atomic>

typedef struct
{
    void * data = grain_construct(44100, 5);
    std::atomic<float> amp;
    std::atomic<float> int_freq;
    std::atomic<int> rate;
    std::atomic<int> max_grains;
    std::atomic<int> grain_size;

    // For Panning
    std::atomic<float> x;
    std::atomic<float> y;
} Grain;

void initGrain(Grain &g)
{
    g.amp = 0.2;
    g.int_freq = 7;
    g.rate= 220;
    g.max_grains = 4;
    g.grain_size = 15;
}
void setGrain(Grain &g)
{
    grain_set_param(g.data, GRAIN_AMP, g.amp.load());
    grain_set_param(g.data, GRAIN_GRAIN_INT_FREQ, g.int_freq.load());
    grain_set_param(g.data, GRAIN_GRAIN_RATE, g.rate.load());
    grain_set_param(g.data, GRAIN_MAX_GRAINS, g.max_grains.load());
    grain_set_param(g.data, GRAIN_GRAIN_SIZE, g.grain_size.load());
}

#endif
