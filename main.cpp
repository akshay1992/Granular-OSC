#include <iostream>
#include "osc.h"

int main()
{
    lo_server_thread st;
    char port[] = "7770";

    spatOSCinit(st, port);

    cout << "Press 'q' to quit" << endl;
    cout << "Running..." << endl;

    spatOSCstart(st);

    char c;
    do
    {
        c=getchar();
    } while (c!='q') ;

    spatOSCstart(st);

    return 0;
}
