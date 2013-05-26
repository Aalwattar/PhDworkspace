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
 * NAME : getNumArch
 * 
 * PURPOSE : return the number of architectures that exist for that task
 * ARGUMENTS : int = which task that we are implementing
 * 
 * RETURNS : false if the filename could not be found or opened or the file did 
 *              not follow the specified format
 *           true otherwise (successful completion)
 * 
 * NOTE : please see the README file for more information about the format
 *              and contents of the architecture information file
 *****************************************************************************/
short int getNumArch(int);

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

