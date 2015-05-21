#include <inttypes.h>

//#include "ugen.h"

#define GRAIN_BUF_SIZE 2048
#define GRAIN_ENV_SIZE 1024

typedef enum {
    GRAIN_MAX_GRAINS,
    GRAIN_AMP,
    GRAIN_GRAIN_SIZE, // in ms
    GRAIN_GRAIN_RATE,
    GRAIN_GRAIN_INT_FREQ, // Internal frequency
    GRAIN_NUM_PARAMS
} GRAIN_PARAMS;

typedef struct grain_s grain_t;

typedef struct {
    double *out;

    double amp;
    double max_grains;
    double grain_dur_ms;
    double grain_rate;
    double grain_int_freq;

    int counter;
    int sr;
    int alloc_grains;

    int trigger_accum;
    int trigger_target;
    int num_active_grains;
    grain_t *active_head;
    grain_t *inactive_head;
    double grain[GRAIN_BUF_SIZE];
    double env[GRAIN_ENV_SIZE];

} GRAIN_DATA;

void *grain_construct(int sr, int alloc_grains);

double grain_process(void *data);

void grain_destroy(void *data);

void grain_set_param(void *data, int index, double value);

double *grain_get_input(void *data, int index);

void grain_set_output(void *data, int index, double *out_buffer);
