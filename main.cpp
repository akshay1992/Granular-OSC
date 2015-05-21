#include <iostream>

#include "portaudio.h"

#include "osc.h"

#include "pa_wrapper.h"

//#define DEVICE_SELECTOR
//#define PRINT_DEVICE_INFO

using namespace std;


typedef struct
{
    int nchannels;	// Total number of channels
    Grain grain;
} paUserData;

// Port Audio Callback
int callback( const void *input,
                void *output,
                unsigned long frameCount,
                const PaStreamCallbackTimeInfo *timeInfo,
                PaStreamCallbackFlags statusFlags,
                void *userData )
{
  float *in = (float *) input;
  float *out = (float *) output;
  paUserData *ud = (paUserData*) userData;

  setGrain(ud->grain);
  for(int i=0; i<frameCount; i+=ud->nchannels)
  {
      out[i] = out[i+1] =  grain_process(ud->grain.data);
  }

  return paContinue;
}

int main()
{
    int nchannels = initPa();

    paUserData data;
    data.nchannels = nchannels;
    initGrain(data.grain);

    // --------------------------
    // START PA THREAD
    // --------------------------

    startPa(callback, &data);

    cout << "Started audio thread..." << endl;

    // -----------------------
    // SETUP OSC RECEIVER
    // -----------------------

    lo_server_thread st;
    char port[] = "7770";

    spatOSCinit(st, port);
    spatOSCstart(st);

    cout << "Started OSC thread..." << endl;

    // ---------------
    //      START
    // ---------------

    cout << "Running...   (Press 'q' and hit return to quit)" << endl;
    char c;
    do
    {
        c=getchar();
    } while (c!='q') ;

    spatOSCstop(st);

    return 0;
}

