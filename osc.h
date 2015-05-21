#ifndef SPATOSC
#define SPATOSC

#include "lo/lo.h"
#include <stdio.h>
using namespace std;

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

void handle(int i)
{
    cout << "Handling" << i << endl;

}


int multi1_handler(const char *path, const char *types, lo_arg ** argv,
                int argc, void *data, void *user_data)
{
    printf("Multi 1: (%f, %f) \n", argv[0]->f, argv[1]->f);
    handle(1);
    fflush(stdout);
    return 0;
}

int multi2_handler(const char *path, const char *types, lo_arg ** argv,
                int argc, void *data, void *user_data)
{
    printf("Multi 2: (%f, %f) \n", argv[0]->f, argv[1]->f);
    fflush(stdout);
    handle(2);
    return 0;
}
int multi3_handler(const char *path, const char *types, lo_arg ** argv,
                int argc, void *data, void *user_data)
{
    printf("Multi 3: (%f, %f) \n", argv[0]->f, argv[1]->f);
    fflush(stdout);
    handle(3);
    return 0;
}
int multi4_handler(const char *path, const char *types, lo_arg ** argv,
                int argc, void *data, void *user_data)
{
    printf("Multi 4: (%f, %f) \n", argv[0]->f, argv[1]->f);
    fflush(stdout);
    handle(4);
    return 0;
}
int multi5_handler(const char *path, const char *types, lo_arg ** argv,
                int argc, void *data, void *user_data)
{
    printf("Multi 5: (%f, %f) \n", argv[0]->f, argv[1]->f);
    fflush(stdout);
    handle(5);
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
