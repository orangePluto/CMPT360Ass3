/*-----------------------------------------------------------------
# Student Name: Damion Shillinglaw
# Assignment 3: concurrent Implementation of MapReduce 
# Winter 2022, CMPT 360
# Instructor's Name: Dr. Cameron Macdonell 
*----------------------------------------------------------------*/

#include "gentfModule.h"

/*********************Result from Map Function****************************/
typedef struct { //InputDocument maps to OutDocument for this program
    int index; //Document's index
    int termIndex; //Used to identify term from bow; most common term
                   //of OutDocument
    int count;
} OutDocument; //used to fill shared structure
/**************************************************************************/
int nDocs; //# of documents from test directory
int nTerms; //# of terms from terms file 
OutDocument * mapPtr; //Dynamic array of OutDocument objects
                       //Shared structure between mapping threads

/**
 * Initialize global variables: nDocs,nterms, and mapPtr.
 *
 * Parameters: testDir <-- test directory used to set nDocs & nterms
 *             sz <-- # of bytes in bow file
 * Pre: testDir must be a valid directory
 * Post: All global variables are updated
 */
void initialize(char * testDir,int sz) {
    enum OFFENCE error; //Used in the event of an error
    int counts = sz / sizeof(int); //# of entries in BOW
    int i; // loop index
    nDocs = getNumFiles(testDir);
    nTerms = counts / nDocs;
    mapPtr = malloc(sizeof(OutDocument) * nDocs);
    if (!mapPtr) { //check space was allocated for shared structure
        error = ERROUTS;
        printOffence(error,NULL);
        exit(1);
    }
    
    for (i = 0; i < nDocs; i++) { //set dummy values for elements of
                                  //shared structure
        mapPtr[i].index = i;
        mapPtr[i].termIndex = -1;
        mapPtr[i].count = 0;
    }

    return;
}

int main(int argc,char *argv[]) {
    bool pass; // Used to validate CMD line arguments/option
    int bound; //size (# of bytes) of bow mapping 
    int * mxPtr; // pointer to maxtrix (bow)
    InputDocument * inputArr; // dataset used to perform map operations

    pass = checkCMDArguments(argc,argv);

    if (pass) {
        printf("Let's begin...\n");
        mxPtr = createBOWspace(argv[4],&bound);
        initialize(argv[3],bound);
        inputArr = splitMatrix(mxPtr,nDocs,nTerms);

        //Release resources
        munmap(mxPtr,bound);
        free(mapPtr);
        freeSplitMatrix(inputArr,nDocs);
    } else {
        printf("None shall pass!\n");
    }

    return 0;
}


