/*-----------------------------------------------------------------
# Student Name: Damion Shillinglaw
# Assignment 3: Concurrent Implementation of MapReduce 
# Winter 2022, CMPT 360
# Instructor's Name: Dr. Cameron Macdonell 
*----------------------------------------------------------------*/

#include "gentfThreads.h"

/**
 * Creates a dummy document (InputDocument struct object), which
 * is MAINLY recgonized by negative index value (-1). 
 *
 * Return: dumnDoc is the dummy document 
 */
InputDocument createDummyDoc() {
    InputDocument dumnDoc;
    
    dumnDoc.column = NULL;
    dumnDoc.ncol = 0;
    dumnDoc.index = -1;
    dumnDoc.terms = NULL;
    dumnDoc.nRefs = 0;

    return dumnDoc;
}

/**
 * Utility function for spawnMapThreads: wait for all threads
 * to finish their map operations.
 *
 * Parameters: threads <-- array of pthread_t objects
 *             n <-- # of threads
 */
void waitOnThreads(pthread_t threads[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        pthread_join(threads[i],0);
    }
}

/**
 * Generate mapper threads to operate on input data.
 *
 * Parameters: inputArr <-- used to divide documents among
 *                          threads; non-modifiable
 *             mapper <-- is the func. performing map operations
 *             n < -- # of threads to generate
 *             len <-- # of documents in inputArr
 * Post: For every thread generated, there is a corresponding 
 *       dynamic array of InputDocuments (sub-set of inputArr) 
 *       passed to it
 */
void spawnMapThreads(const InputDocument * inputArr, 
        void * (*mapper) (void *), int n, int len) {
    enum OFFENCE error;
    pthread_t threads[n];
    int rc;
    int modulus, cycles, remainder;
    int containerSZ; //View threads as containers being filled with docs
    int i, j, inputArrIndex;
    InputDocument * subArr; //Dynamic array
    InputDocument dummyDoc; //Used by a mapper thread to determine 
                            //size of subArr passed to it

    dummyDoc = createDummyDoc();
    
    modulus = n;
    cycles = len / modulus; //For every cycle each thread gets one doc  
    remainder = len % modulus; // > 0 means uneven distribution of docs

    //fill containers (threads) with docs, then
    //spawn threads
    inputArrIndex = 0;
    for (i = 0; i < n; i++) { 
        if (remainder > 0) { // set containerSZ
            containerSZ = cycles + 1;
            remainder--;
        } else {
            containerSZ = cycles;
        }

        subArr = malloc(sizeof(InputDocument) * (containerSZ + 1));
        if (!subArr) { //Check space was allocated for subArr
            error = ERROUTS;
            printOffence(error,NULL);
            exit(1);
        }

        for (j = 0; j < containerSZ; j++) { //Fill container
            subArr[0] = inputArr[inputArrIndex];  
            inputArrIndex++;
        }
        subArr[containerSZ - 1] = dummyDoc; 

        rc = pthread_create(&threads[i], NULL, mapper, (void *) subArr);
        if( rc != 0) { //check thread was successfully created
            error = ERRTHR;
            printOffence(error,NULL);
            exit(1);
        }
    }
    waitOnThreads(threads,n);

}
