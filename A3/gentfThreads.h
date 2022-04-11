/*-----------------------------------------------------------------
# Student Name: Damion Shillinglaw
# Assignment 3: Concurrent Implementation of MapReduce 
# Winter 2022, CMPT 360
# Instructor's Name: Dr. Cameron Macdonell 
*----------------------------------------------------------------*/

/** Module handles mapping threads and updates shared data structure. **/

#ifndef GENTF_THREAD
#define GENTF_THREAD
#include "gentfCommon.h"
#include <pthread.h>

void spawnMapThreads(const InputDocument * inputArr, 
        void * (*mapper) (void *), int n, int len);
#endif
