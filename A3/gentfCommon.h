/*-----------------------------------------------------------------
# Student Name: Damion Shillinglaw
# Assignment 3: concurrent Implementation of MapReduce 
# Winter 2022, CMPT 360
# Instructor's Name: Dr. Cameron Macdonell 
*----------------------------------------------------------------*/

#ifndef GENTF_COMMON
#define GENTF_COMMON
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <linux/limits.h>
#include <sys/mman.h>
#include <fcntl.h>

/**
 * Module contains common routines and input data between gentf-mr-1 
 * & gentf-mr-2.
 */


/******************Input Data Passed to Map functions****************/
typedef struct {
    int * column; //array of counts; each element is a count from BOW  
    int ncol;
    int index; //documents index; used to identify column in BOW 
    int * terms; //Elements ref. a row of BOW; used to identify term 
    int nRefs; //# of referenced terms; # of elems. in terms
} InputDocument;
/********************************************************************/

enum OFFENCE { //used to identify types of errors specific to program
    DEFAULT, //No error
    ERRARG, //Invalid number of arguments passed
    MISSARG, //option -n is missing an argument
    ERRDIR, //Failed to find test directory
    ERRTERMS, //Failed to open terms file
    ERROPT, //Option not recognized
    ERRMAPB, //Failed to create mapping for BOW
    ERRDOC, //Failed to open text/document file
    STRLEN, //# number of characters for path is too large ( > PATH_MAX)
    ERROUTS,//Failed to allocate space after using malloc
    ERRBOW,//Failed to open bow file
    ERRNUM,//option -n wasn't given a #
    ERRTHR,// Failed to create thread
};

/**
 * Validate arguments & option passed to gentf programs
 *
 * Parameters: argn <-- # of arguments passed
 *             args <-- array of arguments (strings)
 * Returns: valid which is true or false 
 */
bool checkCMDArguments(int argn, char *args[]);

/**
 * Maps BOW file to program's address space (array of ints)
 *
 * Parameters: filePath <-- absolute path to bow file
 *             size <-- # number of counts in bow
 * Pre: filePath must exist; size must be declared outside of func. 
 * Post: space for mapping is allocated in programs address space;
 *       size is set by # counts
 * Return: bowPtr <-- a pointer to bow matrix
 */
int * createBOWspace(char *filePath, int * size);
/**
 * Utility function that counts number of text files in 
 * test directory, excluding terms file
 */
int getNumFiles(char *d);
/**
 * Display type of error commited (called an enum OFFENCE for program),
 * more info about errors can be found in the above under OFFENCE.
 *
 * Parameters: e <-- type of error that occoured
 *             docFil <-- name of document that caused error
 * Pre: If the error isn't associated with docFil then pass NULL 
 */
void printOffence(enum OFFENCE e,char * docFil);
/**
 * Split matrix (bow) by documents (columns). 
 * Used to create dataset for mapping threads/func. to operate on.
 *
 * Parameters: m <-- pointer to matrix (viewed as a flat matrix)
 *             inputArr <-- used to fill array with columns
 *             ncol <-- # of columns; # of splits performed
 *             nrow <-- # of rows
 * Pre: m MUST not be NULL 
 * Post: inputArr is placed in heap; member 'column' of elements of inputArr
 *       is, also, placed in heap
 * Return: inputArr is a pointer to an array of InputDocument objects
 */
InputDocument * splitMatrix(int * m, int ncol, int nrow);
/**
 * After using splitMatrix func., use this function to free objects. 
 *
 * Parameters: arr <-- array of InputDocument objects
 *             n <-- # of elements in arr
 * Post: heap space utilized by splitMatrix is freed 
 */
void freeSplitMatrix(InputDocument * arr, int n); 
#endif

