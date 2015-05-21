#ifndef PA_WRAPPER
#define PA_WRAPPER

#include "grain_wrapper.h"
#include "paUtils.h"

//#define DEVICE_SELECTOR
//#define PRINT_DEVICE_INFO

typedef struct
{
    int nchannels;	// Total number of channels
    Grain grain;
} paUserData;

paUserData data;

int main_device_index;

int initPa(void)
{
    // --------------------------
    // INIT PA AND CHOOSE DEVICE
    // --------------------------

    int deviceIndex = 0;
    PaError err;
    err = Pa_Initialize();

    #ifdef DEVICE_SELECTOR
    if (err != paNoError) PaErrorHandler();

    for(int i = 0; i < Pa_GetDeviceCount(); i++) {
       const PaDeviceInfo* info = Pa_GetDeviceInfo(i);
       printf("%2i - %s\n", i, info->name);
    }
    fflush(stdout);

    cout << "Please select device: ";
    scanf("%d", &deviceIndex);
    #endif

    PaDeviceInfo info = *Pa_GetDeviceInfo(deviceIndex);
    printf("* Device %i Selected: %s*\n", deviceIndex, info.name);

    #ifdef PRINT_DEVICE_INFO
    printDeviceInfo(info);
    #endif

    main_device_index = deviceIndex;

    // Return the number of output channels;
    return info.maxOutputChannels;
}

void startPa(
        int (*callback)(const void*, void*, long unsigned int, const PaStreamCallbackTimeInfo*, long unsigned int, void*),
        void *data )
{
    PaDeviceInfo info = *Pa_GetDeviceInfo(main_device_index);
    int bufferSize = 128;
    int sr = info.defaultSampleRate;
    int nchannels = info.maxOutputChannels;

    PaStream *stream;
    PaStreamParameters inparams;
    PaStreamParameters outparams;

    inparams.device = main_device_index;
    inparams.channelCount = info.maxInputChannels;
    inparams.sampleFormat = paInt32;
    inparams.suggestedLatency = info.defaultLowInputLatency;
    inparams.hostApiSpecificStreamInfo = NULL;

    outparams.device = main_device_index;
    outparams.channelCount = info.maxOutputChannels;
    outparams.sampleFormat = paInt32;
    outparams.suggestedLatency = info.defaultLowOutputLatency;
    outparams.hostApiSpecificStreamInfo = NULL;

    PaError err = Pa_OpenStream(&stream, &inparams, &outparams, sr, bufferSize, paNoFlag, callback, data);
    if (err != paNoError) PaErrorHandler();

    err = Pa_StartStream(stream);
    if (err != paNoError) PaErrorHandler();
}



#endif
