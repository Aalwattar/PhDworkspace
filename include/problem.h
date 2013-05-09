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
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#ifndef PROBLEM_H
#define	PROBLEM_H


typedef struct{
    int num_genes;
    int * gene_length;
    int * opr;
    int chrom_length;
}Representation;

Representation * template;

// FIX - needs comment
bool initProblem(char *, char *);

void freeProblem();

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
 *****************          ARCHITECTURE DATA STORAGE         *****************
 *****************************************************************************/

#define ARCH_FILENAME "architecture_library.txt"

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

Operation * operation;
enum ops { a=0, s, m, d };



/******************************************************************************
 *****************           ARCHITECTURE FILE I/O            *****************
 *****************************************************************************/

/******************************************************************************
 * NAME : initArchLibrary
 * 
 * PURPOSE : Imports the architecture information from a file and initializes
 *              static performance variables
 * ARGUMENTS : char * = the name of the file that contains the architecture
 *               information
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
 * PURPOSE : Frees all static dynamically allocated memory for the problem 
 *              definition
 * 
 * PRECONDITIONS : the Operation struct MUST have been previously initialized 
 *                      by initProblem
 *****************************************************************************/
void freeArchLibrary();

/******************************************************************************
 * NAME : printProblem
 * 
 * PURPOSE : print the static information about the problem definition
 * 
 * PRECONDITIONS : the Operation struct MUST have been previously initialized 
 *                      by initProblem
 * 
 * NOTE : this function exists for testing and debugging purposes only.
 *****************************************************************************/
void printArchLibrary();



/******************************************************************************
 *********************           DFG DATA STORAGE         *********************
 *****************************************************************************/

#define DFG_FILENAME "DFG.txt"

typedef struct{
    int num_nodes;
    int * operation; 
}DFG;



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
DFG * initDFG(char *);

void freeDFG(DFG *);

#endif	/* PROBLEM_H */

