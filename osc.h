#ifndef SPATOSC
#define SPATOSC

#include "lo/lo.h"
#include <stdio.h>

#include "pa_wrapper.h"
using namespace std;

extern paUserData data;

int done = 0;

void error(int num, const char *msg, const char *path)
{
    printf("liblo server error %d in path %s: %s\n", num, path, msg);
    fflush(stdout);
}

int quit_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{
    done = 1;
    printf("quiting\n\n");
    fflush(stdout);

    return 0;
}

void handle(int i, float x, float y)
{
//    cout << "Handling" << i << ": " << "(" << x << ", " << y << ")"<< endl;
    switch(i)
    {
    case 1:
        data.grain.rate = (1-y) *150 ;
        break;
    case 2:
        data.grain.y = y;
        data.grain.x = x;
        break;
    case 3:
        data.grain.grain_size = (1-y) * 30 ;
    case 4:
        data.grain.int_freq = (1-y) * 14 + 2;
    case 5:
        data.grain.max_grains = rand()/RAND_MAX * 20 + 5;
    default:;
    }

}


int multi1_handler(const char *path, const char *types, lo_arg ** argv,
                int argc, void *data, void *user_data)
{
    handle(1, argv[0]->f, argv[1]->f);
    return 0;
}

int multi2_handler(const char *path, const char *types, lo_arg ** argv,
                int argc, void *data, void *user_data)
{
    handle(2, argv[0]->f, argv[1]->f);
    return 0;
}
int multi3_handler(const char *path, const char *types, lo_arg ** argv,
                int argc, void *data, void *user_data)
{
    handle(3, argv[0]->f, argv[1]->f);
    return 0;
}
int multi4_handler(const char *path, const char *types, lo_arg ** argv,
                int argc, void *data, void *user_data)
{
    handle(4, argv[0]->f, argv[1]->f);
    return 0;
}
int multi5_handler(const char *path, const char *types, lo_arg ** argv,
                int argc, void *data, void *user_data)
{
    handle(5, argv[0]->f, argv[1]->f);
    return 0;
}

void spatOSCinit(lo_server_thread &st, char * port)
{

    st = lo_server_thread_new(port, error);

    /* add method that will match the path /quit with no args */
    lo_server_thread_add_method(st, "/quit", "", quit_handler, NULL);

    lo_server_thread_add_method(st, "/multi/1", "ff", multi1_handler, NULL);
    lo_server_thread_add_method(st, "/multi/2", "ff", multi2_handler, NULL);
    lo_server_thread_add_method(st, "/multi/3", "ff", multi3_handler, NULL);
    lo_server_thread_add_method(st, "/multi/4", "ff", multi4_handler, NULL);
    lo_server_thread_add_method(st, "/multi/5", "ff", multi5_handler, NULL);
}

void spatOSCstart(lo_server_thread &st)
{
    lo_server_thread_start(st);
}

void spatOSCstop(lo_server_thread &st)
{
    lo_server_thread_free(st);
}


#endif
