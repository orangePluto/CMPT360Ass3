/*-----------------------------------------------------------------
# Student Name: Damion Shillinglaw
# Assignment 3: concurrent Implementation of MapReduce 
# Winter 2022, CMPT 360
# Instructor's Name: Dr. Cameron Macdonell 
*----------------------------------------------------------------*/

#ifndef GENTF_MODULE
#define GENTF_MODULE
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
    ERRMAPT, //Failed to create mapping for terms
    ERRDOC, //Failed to open text/document file
    STRLEN, //# number of characters for path is too large ( > PATH_MAX)
    ERROUTS,//Failed to allocate space for output space
    ERRBOW,//Failed to open bow file
    ERRNUM,//option -n wasn't given a #
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
#endif

