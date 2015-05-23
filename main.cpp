#include <iostream>

#include "portaudio.h"

#include "osc.h"
#include "pa_wrapper.h"

#define OSC_TEST

using namespace std;

paUserData data;

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
//      cout << ud->grain.x << endl;
      float samp = grain_process(ud->grain.data);
      out[i] = samp ;
      out[i+1] = samp ;
  }

  return paContinue;
}

int main()
{
    int nchannels = initPa();

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

