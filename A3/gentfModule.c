/*-----------------------------------------------------------------
# Student Name: Damion Shillinglaw
# Assignment 3: concurrent Implementation of MapReduce 
# Winter 2022, CMPT 360
# Instructor's Name: Dr. Cameron Macdonell 
*----------------------------------------------------------------*/

#include "gentfModule.h"

/**
 * Function is invoked when program wasn't provided appropriate 
 * arguments, which a message for usuage is outputted.
 *
 * Parameters: name <-- string name of program
 */
void printUsuage(char * name) {
    printf("Usuage: %s <pathToTestDirectory> -n <#> <pathToBOW> \n",name);
}

/**
 * Display type of error commited (called an enum OFFENCE for program),
 * more info about errors can be found in the header file of module.
 *
 * Parameters: e <-- type of error that occoured
 *             docFil <-- name of document that caused error
 * Pre: If the error isn't associated with docFil then pass NULL 
 */
void printOffence(enum OFFENCE e,char * docFil) {
    switch (e) {
        case 0: //DEFAULT
            break;
        case 1: //ERRARG
            printf("Incorrect number of arguments passed.\n");
            break;
        case 2: //MISSARG
            printf("-n requires an argument.\n");
            break;
        case 3: //ERRDIR
            printf("Failed to open test directory.\n");
            break;
        case 4: //ERRTERMS
            printf("Failed to open terms file from test directory.\n");
            break;
        case 5: //ERROPT,
            printf("Invalid option passed to program.\n");
            break;
        case 6: //ERRMAPB
            printf("Failed to create mapping for BOW.\n");
            break;
        case 7: //ERRDOC
            printf("Failed to open document %s\n", docFil);
            break;
        case 8: //STRLEN
            printf("Path is too long.\n");
            break;
        case 9: //ERROUTS
            printf("Failed to allocate space for shared structure.\n");
            break;
        case 10: //ERRBOW
            printf("Failed to open bow file.\n");
            break;
        case 11: //ERRNUM
            printf("Option -n requires a number.\n");
            break;
    }
}

/**
 * Get desired directory.
 * Parameter: d is test directory (string)
 * Pre: d must be a valid directory
 * Return: DIR * object is return
 */
DIR * getDirectory(char *d) {
    return opendir(d);
}

/**
 * Utility function that counts number of text files in 
 * test directory, excluding terms file
 */
int getNumFiles(char * d) {
    int n = 0;
    DIR * dir = getDirectory(d);
    struct dirent * entry = NULL;
    char * tempName;

    while ((entry = readdir(dir)) != NULL) {
        tempName = entry -> d_name;
        if ((entry->d_type == 8) && (strcmp(tempName,"terms") != 0))
            n++;
    }
    closedir(dir);
    return n;
}

/**
 * Function checks for the existence of test directory.
 *
 * Parameters: d is test directory
 * Return: valid <-- false or true
 */
bool isDirectory(char *d) {
    bool valid = false; 
    DIR * dir = getDirectory(d);

    if (dir) {
        valid = true;
        closedir(dir);
    }

    return valid;
}

/**
 * Check desired file exists. 
 * Can be used to determine if bow file was passed.
 *
 * Parameters: fil <-- file of interest (string)
 *             subStr <-- check fil has desired substring
 * Pre: if subStr isn't used then set to NULL
 *Return: valid <-- true or false
 */
bool isFile(char *fil, char *subStr) {
    bool valid;
    if( access(fil, F_OK ) == 0 ) {
        // file exists
        if (subStr != NULL) {
            //In addition to exists, check fil contains subStr
            if (strstr(fil,subStr)) { 
                valid = true;
            } else valid = false;
        } else { //Only interested if file exists
            valid = true;
        }
    } else {
        // file doesn't exist
        valid = false;
    }
    return valid;
}

/**
 * Function checks if test directory contains a file called terms.
 *
 * Parameters: d is test directory
 * return: valid <-- false or true
 */
bool hasTerms(char *d) {
    bool valid = false; 
    DIR * dir = getDirectory(d);
    struct dirent * entry = NULL;

    while (!valid && ((entry = readdir(dir)) != NULL)) {
        if ((strcmp(entry->d_name,"terms")) == 0) {
            valid = true;
        }
    }

    closedir(dir);
    return valid;
}

/**
 * Validate arguments & option passed to gentf programs
 *
 * Parameters: argn <-- # of arguments passed
 *             args <-- array of arguments (strings)
 * Returns: valid which is true or false 
 */
bool checkCMDArguments(int argn, char *args[]) {
    bool valid, nflag = false;
    int option; 
    enum OFFENCE error = DEFAULT; //Catch error flags

    if (argn != 5) { //Check number of arguments
        error = ERRARG;
    } else if (!(isDirectory(args[1]))) { //check test directory exists
        error = ERRDIR;
    } else if (!(hasTerms(args[1]))) { //check test directory has terms file
        error = ERRTERMS;
    } else if (!(isFile(args[4],".bow"))) { //check bow file exist
            error = ERRBOW;
    } else { //Begin processing options
        while ((option = getopt(argn,args,":n:")) != -1) {
            switch (option) {
                case 'n': //set nflag
                    nflag = true;
                    break;
                case ':': //missing argument
                    error = MISSARG;
                    break;
                case '?': //Incorrect option
                    error = ERROPT;
                    break;
            }
        }

    }

    if (error) {
        printOffence(error,NULL);
        printUsuage(args[0]);
        valid = false;
    } else if (nflag) {
        if (isdigit(args[2][0])) 
            valid = true;//From here everything checks out
        else { // option -n wasn't given an #  
            valid = false;
            printOffence(error=ERRNUM,NULL);
            printUsuage(args[0]);
        }
    } else { //Option -n is missing
        valid = false;
        printUsuage(args[0]);
    }

    return valid;
}

/*Use utility function to check file path is less than
 * PATH_MAX.
 * Parameters: filPath <-- absolute path to file
 * Return: 1 is true and 0 is false
 */
int checkLength(char * filPath) {
    if ((strlen(filPath) + 1) < PATH_MAX) {
        return 1; //True
    } else { //False
        return 0;
    }
}


/**
 * Maps BOW file to program's address space (array of ints).
 * Func. can terminate program if filePath is too long.
 *
 * Parameters: filePath <-- absolute path to bow file
 *             size <-- # number of bytes in bow file
 * Pre: filePath must exist; size must be declared outside of func. 
 * Post: space for mapping is allocated in programs address space;
 *       size is set by # of bytes in bow file
 * Return: matrixPtr <-- a pointer to bow matrix
 */
int * createBOWspace(char *filePath, int * size) {
    enum OFFENCE error;
    FILE *bowFil;
    int bowDescriptor;
    int * matrixPtr;

    //Check file path length 
    if (checkLength(filePath)) {
        bowFil = fopen(filePath,"rb"); 
    } else {
        error = STRLEN;
        printOffence(error,NULL);
        exit(1);
    }

    if (!bowFil) { //Check bow was opened
        error = ERRBOW;
        printOffence(error,NULL);
        exit(1);
    } //Get size of bow file
    fseek(bowFil,0,SEEK_END);
    *size = ftell(bowFil); 
    fseek(bowFil,0,SEEK_SET);

    //Get bow file's descriptor
    bowDescriptor = open(filePath,O_RDONLY);
    if (bowDescriptor < 0) {
        error = ERRBOW;
        printOffence(error,NULL);
        exit(1);
    }

    matrixPtr = mmap(NULL,*size,PROT_READ, //Create mapping
            MAP_PRIVATE,bowDescriptor,0);
    if (!matrixPtr) {
        error = ERRMAPB;
        printOffence(error,NULL);
        exit(1);
    }

    fclose(bowFil);
    close(bowDescriptor);
    return matrixPtr;
}

/**
 * Split matrix (bow) by documents (columns). 
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
InputDocument * splitMatrix(int * m, int ncol, int nrow) {
    int i,j; 
    enum OFFENCE error;
    InputDocument * inputArr = malloc(sizeof(InputDocument) * ncol);
    if (!inputArr) { //check space was allocated
        error = ERROUTS;
        printOffence(error,NULL);
        exit(1);
    }

    for (i = 0; i < ncol; i++) { //loop for # of splits
        inputArr[i].index = i;
        inputArr[i].ncol = nrow;
        inputArr[i].column = malloc(sizeof(int) * nrow);
        if (!(inputArr[i].column)) { //check space was allocated for 
                                     //member 'column'
            error = ERROUTS;
            printOffence(error,NULL);
            exit(1);
        }
        inputArr[i].nRefs = 0;
        inputArr[i].terms = NULL;
        for (j = 0; j < nrow; j++) { //loop by rows for doc.
            (inputArr[i].column)[j] = m[(j * ncol) + i]; 
        }

    }
            
    return inputArr;   
}

/**
 * After using splitMatrix func., use this function to free objects. 
 * Used to create dataset for mapping threads/func. to operate on.
 *
 * Parameters: arr <-- array of InputDocument objects
 *             n <-- # of elements in arr
 * Post: heap space utilized by splitMatrix is freed 
 */
void freeSplitMatrix(InputDocument * arr, int n) {
    int i;

    for (i = 0; i < n; i++) {
        free(arr[i].column);
    }
    free(arr);
    return;
}
