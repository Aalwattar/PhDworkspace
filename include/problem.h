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
 * LAST MODIFIED : May 12, 2013
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#ifndef PROBLEM_H
#define	PROBLEM_H


#define ARCH_FILENAME "architecture_library.txt"
#define DFG_FILENAME  "DFG.txt"


/******************************************************************************
 **************         PROBLEM DEFINITION DATA STORAGE         ***************
 *****************************************************************************/

// represents one architecture's implementation
typedef struct {
    double area;
    double runtime;
    double power;
} Implementation; 

// contains all of the architectures of an operation
typedef struct{
    int num_impl;               // the number of architectures for that operation
    Implementation * impl;      // the Implementation properties of each architecture
}Operation;

// encapsulates relevant aspects of the DFG we are trying to schedule
typedef struct{
    int num_nodes;      // The number of nodes (tasks) in the DFG
    int * oper;         // The operation of each task
}DFG;

// contains information for representing the problem as a chromosome
typedef struct{
    int num_genes;      // the number of tasks (the number of genes on a chromosome)
    int * oper;         // the type of operation of each task
}Representation;


Operation * arch_library;
enum ops { a=0, s, m, d };

DFG * graph;

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
bool initProblem(char *, char *);


/******************************************************************************
 * NAME : freeProblem
 * 
 * PURPOSE : Frees all dynamically allocated memory initialized for the 
 *              problem definition
 * 
 * PRECONDITIONS : this function should only be called if initProblem() 
 *                  returned true
 *****************************************************************************/
void freeProblem();



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
void freeArchLibrary();

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
void printArchLibrary();



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

