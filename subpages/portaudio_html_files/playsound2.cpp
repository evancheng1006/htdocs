#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "portaudio.h"
#include "testwavio.h"

#define SAMP_RATE (44100)

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#ifndef TABLE_SIZE
#define TABLE_SIZE  (1024)
#endif


typedef struct
{
    short input[TABLE_SIZE];
    short output[TABLE_SIZE];
    int ntime;
    int end;
    short * source;
}Signals;

/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int patestCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData ){
    Signals *data = (Signals*)userData;
    short *out = (short*)outputBuffer;
    short *in = (short*)inputBuffer;

    data->ntime++;
    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;

    memmove(out, data->source, TABLE_SIZE * sizeof(short));
    data->source += TABLE_SIZE ;

     return paContinue;     
}

static void StreamFinished( void* userData )
{
   float *data = (float *) userData;
}

/*******************************************************************/
int main(int argc, char * argv[])
{
    if (argc <= 1)
    {
        printf("fatal error: no input files\n");
        return 0;
    }

    PaStreamParameters outputParameters,
                       inputParameters;
    PaStream *stream;
    PaError err;
    Signals sig;

    char filename[200];
    strcpy(filename, argv[1]);

    //Read Header
    wavinfo info;
    getwavinfo(filename,&info);
    dispwavinfo(&info);

    short * alldata = (short *)malloc(info.dataChunkDataSize + TABLE_SIZE*2);
    if (alldata == NULL)
    {
        printf("memory allocation failed.\n");
        return 0;
    }
    sig.source = alldata;

    FILE *fptr;
    fptr = fopen(filename, "rb");
    if (fptr == NULL)
    {
         printf("filename: %s\n", filename);
         printf("ptrGJMI\n\n");

    }
    fseek(fptr,info.DataOffset,SEEK_SET);

    fread(alldata, sizeof(short), info.nSample * 2, fptr);



/*
    int contreadfile = 1;
    int iblock = 0;


    while (contreadfile)
    {
//        fseek(fptr, sizeof(short)*TABLE_SIZE, SEEK_CUR);
//        int received = fread(alldata+(iblock*TABLE_SIZE), sizeof(short), TABLE_SIZE , fptr);
        int received = 0;
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            received += fread((alldata + iblock * TABLE_SIZE + i * 2), sizeof(short), 1, fptr);
            alldata[iblock * TABLE_SIZE + i * 2 + 1] = alldata[iblock * TABLE_SIZE + i * 2];
//            fseek(fptr, sizeof(short), SEEK_CUR);
        }


//        fseek(fptr, sizeof(short)*TABLE_SIZE, SEEK_CUR);

        received = fread(alldata + iblock * TABLE_SIZE, sizeof(short), TABLE_SIZE, fptr);

        if (received <= 0) {
            contreadfile = 0;
        }
        iblock++;
    }*/
    fclose(fptr);
/*
#define FREQ 880
#define AMP 2500
    double nSampleFloat = (double)info.nSample;
    double mul = ((double)2 * (double)M_PI / nSampleFloat * (double)FREQ);
    printf("mul = %f\n", mul);
    for (int i = 0; i < info.nSample+TABLE_SIZE; i++)
    {
        alldata[i] = (short) (sin ((double)i * mul) * (double)AMP);
    }
   for (int i = 0; i < 20; i++)
   {
        printf("%d\n", alldata[i]);
    }
*/
    //Load File
    int nFrame = int ((info.nSample+TABLE_SIZE-1) / TABLE_SIZE);
    int totalpts = nFrame*TABLE_SIZE*info.NumberOfChannels;
    float time = (float)info.nSample / (float)info.SampleRate;
    printf("nFrame=%d\n", nFrame);

    printf("Test SR = %d, time = %f(s)\n", info.SampleRate, time);
    
    /* initialize sinusoidal wavetable */
    for(int i=0; i<TABLE_SIZE; i++ )
    {
        sig.output[i] = 0;
    }
    sig.end = 0;
    sig.ntime = -1;
    
    err = Pa_Initialize();

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    outputParameters.channelCount = 2;       /* mono output */
    outputParameters.sampleFormat = paInt16; /* 16 bit fixed point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    
    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    inputParameters.channelCount = 1;                    /* mono input */
    inputParameters.sampleFormat = paInt16;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
              &stream,
              &inputParameters, 
              &outputParameters,
              info.SampleRate,
              TABLE_SIZE / 2,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              patestCallback,
              &sig );

    err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
    err = Pa_StartStream( stream );

    int sleepTime = (int)((float)1000 * float(info.nSample) / (float)info.SampleRate);
    Pa_Sleep(sleepTime);

    err = Pa_StopStream( stream );
    err = Pa_CloseStream( stream );
    Pa_Terminate();

    printf("Test finished.\n");

    free(alldata);
    
    return err;

}
