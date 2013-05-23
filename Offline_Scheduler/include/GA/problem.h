/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 7, 2013
 * Modified : May 22, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : problem.h
 * 
 * Purpose  : a header for problem.c
 ******************************************************************************/


#ifndef PROBLEM_H
#define	PROBLEM_H

#include <stdbool.h>


/******************************************************************************
 **************         PROBLEM DEFINITION DATA STORAGE         ***************
 *****************************************************************************/

// represents one architecture's implementation
typedef struct {
    short int columns;
    short int rows;
    
    short int conf_t;
    short int exec_t;
    short int conf_p;
    short int exec_p;
} Implementation; 

// contains all of the architectures of an operation
typedef struct{
    short int num_impl;         // the number of architectures for that operation
    Implementation * impl;      // the Implementation properties of each architecture
}Operation;


// contains information for representing the problem as a chromosome
typedef struct{
    short int num_genes;// the number of tasks (the number of genes on a chromosome)
    int * oper;         // the type of operation of each task
}Representation;


Operation * arch_library;

Representation * template;


/******************************************************************************
 * NAME : initProblem
 * 
 * PURPOSE : Initializes problem information (available architectures and 
                task graph) from textfiles
 * ARGUMENTS : char * = name of the file containing architectre information
 *             char * = name of the file containing the DFG of tasks
 * 
 * RETURNS : false either of the filenames could not be found or opened or 
 *              either file did not follow the specified format
 *           true otherwise (static variables were successfuly initialized)
 * 
 * NOTE : please see the README file for more information about the format
 *              and contents of both required files
 *****************************************************************************/
bool initProblem(void);


/******************************************************************************
 * NAME : freeProblem
 * 
 * PURPOSE : Frees all dynamically allocated memory initialized for the 
 *              problem definition
 * 
 * PRECONDITIONS : this function should only be called if initProblem() 
 *                  returned true
 *****************************************************************************/
void freeProblem(void);



/******************************************************************************
 *****************           ARCHITECTURE FILE I/O            *****************
 *****************************************************************************/

/******************************************************************************
 * NAME : initArchLibrary
 * 
 * PURPOSE : Imports the architecture information from a file
 * ARGUMENTS : char * = name of the file containing the architecture data
 * 
 * RETURNS : false if the filename could not be found or opened or the file did 
 *              not follow the specified format
 *           true otherwise (successful completion)
 * 
 * NOTE : please see the README file for more information about the format
 *              and contents of the architecture information file
 *****************************************************************************/
bool initArchLibrary(char *);

/******************************************************************************
 * NAME : freePerformanceInfo
 * 
 * PURPOSE : Frees all dynamically allocated memory used to store the 
 *              problem's architecture information
 * 
 * PRECONDITIONS : This function should only be called if initArchLibrary()
 *                  returned true
 *****************************************************************************/
void freeArchLibrary(void);

/******************************************************************************
 * NAME : printArchLibrary
 * 
 * PURPOSE : print the library of architectures
 * 
 * PRECONDITIONS : the architecture library MUST have been previously  
 *                      initialized by initProblem()
 * 
 * NOTE : this function exists for testing and debugging purposes only.
 *****************************************************************************/
void printArchLibrary(void);


/******************************************************************************
 ***********************           DFG FILE I/O         ***********************
 *****************************************************************************/

/******************************************************************************
 * NAME : initDFG
 * 
 * PURPOSE : Imports a DFG from a template file
 * ARGUMENTS : char * = the name of the file that contains the DFG
 * 
 * RETURNS : false if the filename could not be found or opened or the file did 
 *              not follow the specified format
 *           true otherwise (successful completion)
 * 
 * NOTE : please see the README file for more information about the format
 *              and contents of the DFG file
 *****************************************************************************/
bool initDFG(char *);

/******************************************************************************
 * NAME : freeDFG
 * 
 * PURPOSE : Frees all static dynamically allocated memory for the 
                representation of the DFG
 * 
 * PRECONDITIONS : the DFG struct MUST have been previously initialized 
 *                      by initProblem
 *****************************************************************************/
void freeDFG(void);


/******************************************************************************
 *****************          RANDOM NUMBER GENERATION          *****************
 *****************************************************************************/

/******************************************************************************
 * NAME : seedRandGenerator
 * 
 * PURPOSE : Initialize the random number generator with a user defined seed
 * ARGUMENTS : int = the seed
 *****************************************************************************/
void seedRandGenerator(int);

/******************************************************************************
 * NAME : randSeed
 * 
 * PURPOSE : initialize the random number generator using the current system
 *              time as the seed
 *****************************************************************************/
void randSeed(void);

/******************************************************************************
 * NAME : randomNumber
 * 
 * PURPOSE : Generate a random number between 0 and 1
 * PRECONDITIONS : you MUST first seed the random number generator by using
 *                      either seedRandGenerator OR randSeed
 * 
 * RETURNS : a randomly generated double between 0 and 1
 *****************************************************************************/
double randomNumber(void);



#endif	/* PROBLEM_H */

