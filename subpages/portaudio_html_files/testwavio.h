#include<stdio.h>
#include<stdlib.h>

#ifndef TESTWAVIO_HEADER
#define TESTWAVIO_HEADER

namespace{

typedef struct{
        // 0x00000000 means no such chunk
        char RIFFChunkID[4];
        unsigned int RIFFChunkDataSize;
        unsigned int RIFFType;
        char fmtChunkID[4];
        unsigned int fmtChunkDataSize;
        unsigned short CompressionCode;
        unsigned short NumberOfChannels;
        unsigned int SampleRate;
        unsigned int AverageBytesPerSecond;
        unsigned short BlockAlign;
        unsigned short SignificantBitsPerSample;
        unsigned short ExtraFormatBytes;
        char dataChunkID[4]; 
        unsigned int dataChunkDataSize;
        char factChunkID[4];
        unsigned int factChunkDataSize;
        char wavlChunkID[4];
        unsigned int wavlChunkDataSize;
        char slntChunkID[4];
        unsigned int slntChunkDataSize;
        char cueChunkID[4];
        unsigned int cueChunkDataSize;
        char plstChunkID[4];
        unsigned int plstChunkDataSize;
        char listChunkID[4];
        unsigned int listChunkDataSize;
        char lablChunkID[4];
        unsigned int lablChunkDataSize;
        char noteChunkID[4];
        unsigned int noteChunkDataSize;
        char ltxtChunkID[4];
        unsigned int ltxtChunkDataSize;
        char smplChunkID[4];
        unsigned int smplChunkDataSize;
        char instChunkID[4];
        
        // custom parameters
        unsigned int nSample;
        unsigned int DataOffset;
}wavinfo;

wavinfo setwavinfo(int , int);
// SampleRate , BitsPerSample
// default chunk: RIFF, fmt, data

int getwavinfo(const char* , wavinfo*);
// filename , wavinfo
int dispwavinfo(wavinfo* );
int wavtoshort(const char* , wavinfo* , short*); // don't have to read header again, but throw the parameter again
// filename , wavinfo , data
int wavtodouble(const char* , wavinfo* , double*); // don't have to read header again, but throw the parameter again
// filename , wavinfo , data

wavinfo setwavinfo(int SampleRate, int BitsPerSample)
{
     wavinfo info;
     
        info.RIFFChunkID[4] = 0x46464952;
        info.RIFFChunkDataSize;
        info.RIFFType = 0x57415645;
        info.fmtChunkID[4] = 0x20746D66;
        info.fmtChunkDataSize;
        info.CompressionCode;
        info.NumberOfChannels;
        info.SampleRate = SampleRate;
        info.AverageBytesPerSecond;
        info.BlockAlign;
        info.SignificantBitsPerSample;
        info.ExtraFormatBytes;
        info.dataChunkID[4] = 0x61746164; 
        info.dataChunkDataSize;
        info.factChunkID[4] = 0x00000000;
        info.factChunkDataSize = 0;
        info.wavlChunkID[4] = 0x00000000;
        info.wavlChunkDataSize = 0;
        info.slntChunkID[4] = 0x00000000;
        info.slntChunkDataSize = 0;
        info.cueChunkID[4] = 0x00000000;
        info.cueChunkDataSize = 0;
        info.plstChunkID[4] = 0x00000000;
        info.plstChunkDataSize = 0;
        info.listChunkID[4] = 0x00000000;
        info.listChunkDataSize = 0;
        info.lablChunkID[4] = 0x00000000;
        info.lablChunkDataSize = 0;
        info.noteChunkID[4] = 0x00000000;
        info.noteChunkDataSize = 0;
        info.ltxtChunkID[4] = 0x00000000;
        info.ltxtChunkDataSize = 0;
        info.smplChunkID[4] = 0x00000000;
        info.smplChunkDataSize = 0;
        info.instChunkID[4] = 0x00000000;
     
     return info;
}

int getwavinfo(const char *fname, wavinfo* getinfo)//得到wav檔案訊息
{
    // unsigned int are 4 chars;
    unsigned int chunklist[] = {0x46464952 , 0x20746D66 , 0x61746164 }; // RIFF fmt data fact
    FILE *fptr = NULL;
    fptr = fopen(fname,"rb");
    if(fptr == NULL)
    {
            printf("fopen fp_s error\n");
            return -1;
    }
    else
    {
            fseek(fptr,0,SEEK_SET);
            int exit = 0;
            while(exit == 0)
            {
           //     transfer id;
                unsigned int ChunkID;
                fread(&ChunkID , sizeof(unsigned int) , 1 , fptr);
           //     id.ui = ChunkID;
           //     printf("%c%c%c%c\n" , id.c[0], id.c[1],id.c[2],id.c[3]);
                if(ChunkID == chunklist[0]){
                    getinfo->RIFFChunkID[0] = 'R';
                    getinfo->RIFFChunkID[1] = 'I';
                    getinfo->RIFFChunkID[2] = 'F';
                    getinfo->RIFFChunkID[3] = 'F';
                    fread(&getinfo->RIFFChunkDataSize , sizeof(unsigned int) , 1 , fptr);
                    fread(&getinfo->RIFFType , sizeof(unsigned int) , 1 , fptr);
                }
                else if(ChunkID == chunklist[1]){
                    getinfo->fmtChunkID[0] = 'f';
                    getinfo->fmtChunkID[1] = 'm';
                    getinfo->fmtChunkID[2] = 't';
                    getinfo->fmtChunkID[3] = ' ';
                    fread(&getinfo->fmtChunkDataSize , sizeof(unsigned int) , 1 , fptr);
                    fread(&getinfo->CompressionCode, sizeof(unsigned short ) , 1 , fptr);
                    fread(&getinfo->NumberOfChannels, sizeof(unsigned short ) , 1 , fptr);
                    fread(&getinfo->SampleRate , sizeof(unsigned int) , 1 , fptr);
                    fread(&getinfo->AverageBytesPerSecond , sizeof(unsigned int) , 1 , fptr);
                    fread(&getinfo->BlockAlign, sizeof(unsigned short ) , 1 , fptr);
                    fread(&getinfo->SignificantBitsPerSample, sizeof(unsigned short ) , 1 , fptr);
                    
                    if(getinfo->fmtChunkDataSize == 16)
                    {
                         getinfo->ExtraFormatBytes = 0;
                    }                    
                    else{
                       fread(&getinfo->ExtraFormatBytes, sizeof(unsigned short ) , 1 , fptr);
                       fseek(fptr,getinfo->ExtraFormatBytes,SEEK_CUR); 
                       printf("ExtraFormatBytes exist");
                    }                    
                }
                else if(ChunkID == chunklist[2]){
                    getinfo->dataChunkID[0] = 'd';
                    getinfo->dataChunkID[1] = 'a';
                    getinfo->dataChunkID[2] = 't';
                    getinfo->dataChunkID[3] = 'a';
                    fread(&getinfo->dataChunkDataSize, sizeof(unsigned int),1 ,fptr);
                    getinfo->DataOffset = ftell(fptr);
                    exit = 1;
                }
                else
                {
                    exit = 3;
                }
            }
            if(exit == 3)
            {
                 printf("ILLEGAL WAVE CHUNK\n");
                 return -2;
            }
            if(exit == 1)
            {
                   getinfo->nSample = getinfo->dataChunkDataSize * 8 / (getinfo->SignificantBitsPerSample * getinfo->NumberOfChannels);
           //      printf("Header reading successfully\n");
            }
    }
    fclose(fptr); // 關閉檔案
}
int dispwavinfo(wavinfo* getinfo)
{
    union transfer{unsigned int ui; char c[4];};
    if(getinfo->RIFFChunkID[0] == 'R' &&
    getinfo->RIFFChunkID[1] == 'I' &&
    getinfo->RIFFChunkID[2] == 'F' &&
    getinfo->RIFFChunkID[3] == 'F' )
    {
        printf("RIFFChunkDataSize: %d\n" , getinfo->RIFFChunkDataSize);
        transfer id;
        id.ui = getinfo->RIFFType;
        printf("RIFFType: %c%c%c%c\n" , id.c[0], id.c[1],id.c[2],id.c[3]);
    }
    if(getinfo->fmtChunkID[0] == 'f' &&
    getinfo->fmtChunkID[1] == 'm' &&
    getinfo->fmtChunkID[2] == 't' &&
    getinfo->fmtChunkID[3] == ' ' )
    {
       printf("fmtChunkDataSize: %d\n" , getinfo->fmtChunkDataSize );
       printf("CompressionCode: %d\n" , getinfo->CompressionCode );
       printf("NumberOfChannels: %d\n" , getinfo->NumberOfChannels );
       printf("SampleRate: %d\n" , getinfo->SampleRate );
       printf("AverageBytesPerSecond: %d\n" , getinfo->AverageBytesPerSecond );
       printf("BlockAlign: %d\n" , getinfo->BlockAlign );
       printf("SignificantBitsPerSample: %d\n" , getinfo->SignificantBitsPerSample );
       printf("ExtraFormatBytes: %d\n" , getinfo->ExtraFormatBytes );
    }
    if(getinfo->dataChunkID[0] == 'd' &&
    getinfo->dataChunkID[1] == 'a' &&
    getinfo->dataChunkID[2] == 't' &&
    getinfo->dataChunkID[3] == 'a' )
    {
        printf("dataChunkDataSize: %d\n" , getinfo->dataChunkDataSize);
    }
    printf("nSample: %d\n" , getinfo->nSample);
    printf("DataOffset: %d\n" , getinfo->DataOffset);
}
int wavtoshort(const char* fname,  wavinfo* info ,short* data)
{
    FILE *fptr = NULL;
    fptr = fopen(fname,"rb");
    if(fptr == NULL)
    {
            printf("fopen fp_s error\n");
            return -1;
    }
    else
    {
          fseek(fptr,info->DataOffset,SEEK_SET);
          if(info->CompressionCode == 1)
          {
                if(info->SignificantBitsPerSample==16)
                {
                     fread(data, sizeof(short), info->nSample*info->NumberOfChannels ,fptr);
                }       
          }
    }
    fclose(fptr);
}
int wavtodouble(const char* fname, wavinfo* info, double* data)
{
    FILE *fptr = NULL;
    fptr = fopen(fname,"rb");
    if(fptr == NULL)
    {
            printf("fopen fp_s error\n");
            return -1;
    }
    else
    {
          fseek(fptr,info->DataOffset,SEEK_SET);
          if(info->CompressionCode == 1)
          {
                if(info->SignificantBitsPerSample==16)
                {
                    short* temp = (short*)malloc(info->dataChunkDataSize);
                    fread(temp, sizeof(short), info->nSample*info->NumberOfChannels ,fptr);
                                        
                    for(int i = 0; i<info->nSample*info->NumberOfChannels; i++)
                    *(data+i) = (double)*(temp+i) / (double)32768;
                    free(temp);
                }       
          }
    }
    fclose(fptr);
}



};//namespace
#endif
