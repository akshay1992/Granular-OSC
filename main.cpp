#include <iostream>
#include <math.h>

#include "portaudio.h"

#include "osc.h"
#include "pa_wrapper.h"

#define RMS_BUFFER_LENGTH_ms 120

using namespace std;

paUserData data;

lo_address sender_thread = lo_address_new(NULL, "9876");

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

  static int RMS_frameCounter = RMS_BUFFER_LENGTH_ms * data.sampleRate;

  static double RMS_accum_L, RMS_accum_R;

  setGrain(ud->grain);
  for(int i=0; i<frameCount; i+=ud->nchannels)
  {
//      cout << ud->grain.x << endl;
      float samp = grain_process(ud->grain.data);
      RMS_accum_L += out[i] = samp ;
      RMS_accum_R += out[i+1] = samp ;
      RMS_frameCounter --;
  }

  if (RMS_frameCounter ==0)
  {
    RMS_frameCounter = RMS_BUFFER_LENGTH_ms * data.sampleRate;
    float RMS_L = sqrt(RMS_accum_L/RMS_frameCounter);
    float RMS_R = sqrt(RMS_accum_R/RMS_frameCounter);

    lo_send(sender_thread, "/RMS/L", "f", RMS_L);
    lo_send(sender_thread, "/RMS/R", "f", RMS_R);
  }


  return paContinue;
}

int main()
{
    int nchannels = initPa();

    data.sampleRate =
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

