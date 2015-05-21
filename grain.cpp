#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "grain.h"

struct grain_s {

    double *grain;
    double grain_phs;
    double grain_inc;

    double *env;
    double env_phs;
    double env_inc;

    struct grain_s *next;
};

void *grain_construct(int sr, int alloc_grains)
{
    int i;
    GRAIN_DATA *data = (GRAIN_DATA *) malloc(sizeof(GRAIN_DATA));

    data->sr = sr;
    data->alloc_grains = alloc_grains;
    data->amp = 1.0;
    data->max_grains = alloc_grains;
    grain_set_param(data, GRAIN_GRAIN_RATE, 1000.0);
    data->grain_int_freq = 2.5;
    data->grain_dur_ms = 30.0;
    data->counter = 0;
    data->num_active_grains = 0;

    data->trigger_accum = 0;
    for (i = 0; i < GRAIN_BUF_SIZE; i++) {
        data->grain[i] = sin(2 * M_PI * i / (float) GRAIN_BUF_SIZE);
    }
    for (i = 0; i < GRAIN_ENV_SIZE; i++) {
        data->env[i] = sin(M_PI * i / (float) (GRAIN_ENV_SIZE - 1));
    }

    data->active_head = NULL;
    data->inactive_head = NULL;

    for (i = 0; i < alloc_grains; i++) {
        grain_t *new_grain = (grain_t *) malloc(sizeof(grain_t));
        new_grain->next = data->inactive_head;
        data->inactive_head = new_grain;
        new_grain->env_phs = 0;
        new_grain->env_inc = data->grain_int_freq / (data->sr * data->grain_dur_ms);
        new_grain->grain = data->grain;
        new_grain->env = data->env;
    }
    return (void *) data;
}

double grain_run_grain(grain_t *g)
{
    double out;
//    assert(g->env_phs < GRAIN_ENV_SIZE);
    double int_part;
    double frac_grain_phs = modf(g->grain_phs, &int_part);
    double frac_env_phs = modf(g->env_phs, &int_part);

    int next_grain_index = (int) (g->grain_phs + 1);
    int next_env_index = (int) (g->env_phs + 1);
    if(next_grain_index >= GRAIN_BUF_SIZE) {
        next_grain_index = 0;
    }
    if(next_env_index >= GRAIN_ENV_SIZE) {
        next_env_index = 0;
    }

    out = ( (frac_grain_phs * g->grain[next_grain_index])
            + ((1 - frac_grain_phs) * g->grain[(int) g->grain_phs]) )
            *
            ( (frac_env_phs * g->env[next_env_index])
                        + ((1 - frac_env_phs) * g->env[(int) g->env_phs]) );

    g->env_phs += g->env_inc;
    g->grain_phs += g->grain_inc;
    if (g->grain_phs >= GRAIN_BUF_SIZE) {
        g->grain_phs -= GRAIN_BUF_SIZE;
    }

    return out;
}

int grain_is_done(grain_t *g)
{
    if (g->env_phs >= GRAIN_ENV_SIZE) {
        return 1;
    } else {
        return 0;
    }
}

double grain_process(void *data)
{
    GRAIN_DATA *d = (GRAIN_DATA *) data;

    // check if new grain
    if (d->trigger_accum >= d->trigger_target) { // time for a new grain

        if(d->inactive_head && d->max_grains > d->num_active_grains) {
            grain_t * new_grain = d->inactive_head;
            d->inactive_head = d->inactive_head->next;
            new_grain->next = d->active_head;
            d->active_head = new_grain;
            new_grain->grain_phs = 0;
            new_grain->env_phs = 0;
            new_grain->grain_inc = d->grain_int_freq * GRAIN_BUF_SIZE / ((d->grain_dur_ms /1000.0) * d->sr);
            new_grain->env_inc = GRAIN_ENV_SIZE / ((d->grain_dur_ms /1000.0) * d->sr);
            d->num_active_grains++;
        }
        d->trigger_accum = 0;
    }
    d->trigger_accum++;

    //get the output from all grains
    double out = 0.0;
    grain_t *g = d->active_head;

    while (g) {
        out += grain_run_grain(g);
        g = g->next;
    }

    g = d->active_head;
    grain_t *previous = NULL;
    while(g) {
        if (grain_is_done(g)) { // check for finished grains
            if(previous == NULL) {
                d->active_head = g->next;

                g->next = d->inactive_head;
                d->inactive_head = g;
                g = d->active_head;
            } else {
                previous->next = g->next;

                g->next = d->inactive_head;
                d->inactive_head = g;
                g = previous->next;
            }
            d->num_active_grains--;
        } else {
            previous = g;
            g = g->next;
        }
    }
    *d->out = out * d->amp;

    return *d->out;
}


void grain_destroy(void *data)
{
    GRAIN_DATA *graindata = (GRAIN_DATA *) data;

    grain_t *gr = graindata->inactive_head;

    while (gr) {
        grain_t *next = gr->next;
        free(gr);
        gr = next;
    }

    gr = graindata->active_head;

    while (gr) {
        grain_t *next = gr->next;
        free(gr);
        gr = next;
    }

    free(graindata);
}


void grain_set_param(void *data, int index, double value)
{
    GRAIN_DATA *d = (GRAIN_DATA *) data;
    switch (index) {
    case GRAIN_MAX_GRAINS:
        d->max_grains = value;
        break;
    case GRAIN_AMP:
        d->amp = value;
        break;
    case GRAIN_GRAIN_SIZE:
        d->grain_dur_ms = value;
        break;
    case GRAIN_GRAIN_RATE:
        if(value >= d->sr) {
            value = d->sr - 1;
        } else if (value < 0.01) {
            value = 0.01;
        }
        d->grain_rate = value;
        d->trigger_target = (int)(d->sr / d->grain_rate);
        break;
    case GRAIN_GRAIN_INT_FREQ:
        d->grain_int_freq = value;
        break;
    }
    return;
}


double *grain_get_input(void *data, int index)
{
    GRAIN_DATA *d = (GRAIN_DATA *) data;
//    switch (index) {
//    case GRAIN_IN1:
//        return d->in1;
//    case GRAIN_IN2:
//        return d->in2;
//    }
    return NULL;
}


void grain_set_output(void *data, int index, double *out_buffer)
{
    GRAIN_DATA *d = (GRAIN_DATA *) data;
    switch (index) {
    case 0:
        d->out = out_buffer;
        break;
    }
}
