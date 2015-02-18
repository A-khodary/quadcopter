#ifndef READER
#define READER

// Global variables includes

#include reader_global_variables.h

// Functions prototypes :

void* readerHandler(void* arg);


// Ultrasonic telemeter management

float* initUltrasonic();
void* addToSampleList(float sample, float* sampleList);
float getFilteredUltrasonic(float* sampleList);
shutdownUltrasonic(float* sampleList);



//Some defines relative to the median filtering of the ultrasonic sensor :

#define SAMPLESIZE 100





// Structure declarations :



#endif
