#ifndef READER
#define READER

// Global variables includes

#include reader_global_variables.h

// Functions prototypes :

void* readerHandler(void* arg);


// Ultrasonic telemeter management

float* initUltrasonic();
void* addToSampleList(float sample, sampleList_t* sampleList);
float getFilteredUltrasonic(sampleList_t* sampleList);
shutdownUltrasonic(sampleList_t sampleList);
comp(const void *a, const void *b);



//Some defines relative to the median filtering of the ultrasonic sensor :

#define SAMPLESIZE 256 // NEED TO BE A PAIR NUMBER





// Structure declarations :

typedef struct sampleList_t
{
    float list[SAMPLESIZE];
    int lastEntryIndex;
    int numberOfSamples;

}sampleList_t;



#endif
