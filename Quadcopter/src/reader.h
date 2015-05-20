#ifndef READER_h
#define READER_h

#include <wiringSerial.h>

#include "shared_librairies.h"

// Global variables includes

#include "reader_global_variables.h"

// Some defines relative to Serial :

#define BAUDRATE 115200

//Some defines relative to the median filtering of the ultrasonic sensor :

#define SAMPLESIZE 32 // NEED TO BE A PAIR NUMBER

// Functions prototypes :

void* readerHandler(void* arg);


// Structure declarations :

typedef struct sampleList_t
{
    float list[SAMPLESIZE];
    int lastEntryIndex;
    int numberOfSamples;

}sampleList_t;


// Ultrasonic telemeter management

sampleList_t* initUltrasonic();
void addToSampleList(float sample, sampleList_t* sampleList);
float getFilteredUltrasonic(sampleList_t sampleList);
void shutdownUltrasonic(sampleList_t* sampleList);
int comp(const void *a, const void *b);
int notifyPwmChannelNull();




#endif
