/*******************************************************************************
 * FILE NAME : problem.h
 * 
 * Genetic Algorithm practice for Ahmed Al-Watter
 * 
 * PURPOSE : a header for problem.c
 * 
 * AUTHOR : Jennifer Winer
 * Email  : jwiner@uoguelph.ca
 * 
 * DATE CREATED : May 7, 2013
 * LAST MODIFIED : May 8, 2013
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#ifndef PROBLEM_H
#define	PROBLEM_H


/******************************************************************************
 *****************      ARCHITECTURE INFORMATION STORAGE      *****************
 *****************************************************************************/
#define ARCH_FILENAME "performance_data.txt"

// Contains all relevant data about a specific architecture
typedef struct {
    double area;
    double runtime;
    double power;
} Architecture; 

// contains all of the architectures of an operation
typedef struct{
    int num_arch;
    Architecture * arch;
}Operation;

enum operation { a=0, s, m, d };

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



/******************************************************************************
 *****************              PROBLEM FILE I/O              *****************
 *****************************************************************************/

/******************************************************************************
 * NAME : initProblem
 * 
 * PURPOSE : Imports the trivial problem definition from a file
 * ARGUMENTS : char * = the filename that contains the problem information
 * 
 * RETURNS : NULL if the filename could not be found or opened or the file did 
 *              not follow the specified format
 *           Otherwise, an Operation struct that contains the parameters of each
 *              architecture of each operation
 * 
 * NOTE : please see the README file for more information about the format
 *              and contents of the trivial problem definition.
 *        please see problem.h for more information about the Operation and 
 *              Architecture data structures.
 *****************************************************************************/
Operation * initProblem(char *);

/******************************************************************************
 * NAME : initPerformanceInfo
 * 
 * PURPOSE : creates and initializes the performance information from the file
 *              pointer provided by initProblem
 * ARGUMENTS : FILE * = a file pointer to a file that contains the 
 *              architecture information for each operation
 * 
 * PRECONDITIONS : The file pointer must currently point to the start of the
 *      file, and the file must be formatted exactly as described in the README
 * 
 * RETURNS : NULL if the file did not follow the specified format
 *           Otherwise, an Operation struct that contains the parameters of each
 *              architecture of each operation
 *****************************************************************************/
Operation * initPerformanceInfo(FILE *);

/******************************************************************************
 * NAME : initArchInfo
 * 
 * PURPOSE : Initializes a Architecture struct with one architecture
 * ARGUMENTS : FILE * = a file pointer to the file that contains the 
 *              architecture information
 * 
 * PRECONDITIONS : the file pointer must be currently placed at the start of an
 *      architecture line formatted exactly as described in the README file
 * 
 * RETURNS : true if the line followed the format specified in the README
 *           false otherwise
 * 
 * NOTE : this is a helper function for initPerformanceInfo. Please refrain
 *              from using otherwise.
 *****************************************************************************/
bool initArchInfo(FILE *, Architecture *);

/******************************************************************************
 * NAME : freePerformanceInfo
 * 
 * PURPOSE : Frees all memory from within a Operation struct
 * ARGUMENTS : Operation * = the operation that you wish to free
 * 
 * PRECONDITIONS : the Operation struct MUST have been previously initialized 
 *                      by initProblem
 *****************************************************************************/
void freePerformanceInfo(Operation *);

/******************************************************************************
 * NAME : printProblem
 * 
 * PURPOSE : print the contents of an Operation struct
 * ARGUMENTS : Operation * = the struct that you wish to print
 * 
 * PRECONDITIONS : the Operation struct MUST have been previously initialized 
 *                      by initProblem
 * 
 * NOTE : this function exists for testing and debugging purposes only.
 *****************************************************************************/
void printProblem(Operation *);

#endif	/* PROBLEM_H */

