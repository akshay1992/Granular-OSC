#include <iostream>

#include "portaudio.h"

#include "osc.h"
#include "pa_wrapper.h"

using namespace std;

extern paUserData data;


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
  grain_set_param(ud->grain.data, GRAIN_AMP, ud->grain.amp);

  setGrain(ud->grain);
  for(int i=0; i<frameCount; i+=ud->nchannels)
  {
      float samp = grain_process(ud->grain.data);
      out[i] = ud->grain.x * samp;
      out[i+1] = (1-ud->grain.x) * samp;
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

