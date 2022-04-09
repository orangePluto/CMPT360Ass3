/*-----------------------------------------------------------------
# Student Name: Damion Shillinglaw
# Assignment 3: concurrent Implementation of MapReduce 
# Winter 2022, CMPT 360
# Instructor's Name: Dr. Cameron Macdonell 
*----------------------------------------------------------------*/

#include "gentfModule.h"

/*********************Result from Map Function****************************/
typedef struct {
    int index; //Document's index
    int termIndex; //Used to identify term from bow
    int count;
} OutDocument; //used to fill shared structure
/**************************************************************************/


int nDocs;
int nTerms; 

int main(int argc,char *argv[]) {
    bool pass;
    int bound,counts,i;
    int * mxPtr;

    pass = checkCMDArguments(argc,argv);

    if (pass) {
        printf("Lets begin...\n");
        mxPtr = createBOWspace(argv[4],&bound);
        counts = bound/sizeof(int);
        for(i=0;i < counts; i++) 
            printf("%d ",mxPtr[i]);
        printf("\n");
    } else {
        printf("None shall pass!\n");
    }
    
    munmap(mxPtr,bound);
    return 0;
}


