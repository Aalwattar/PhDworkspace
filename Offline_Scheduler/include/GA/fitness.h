/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 16, 2013
 * Modified : May 22, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : fitness.h
 * 
 * Purpose  : a header for fitness.c
 ******************************************************************************/


#ifndef FITNESS_H
#define	FITNESS_H

#include <stdbool.h>
#include "individual.h"

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
int getNumArch(int);



/******************************************************************************
 *****************        FITNESS FUNCTION (NAPOLEON)         *****************
 *****************************************************************************/

// A struct for returning the information relevant to the fitness of a 
//   schedule from Napoleon back to the Genetic Algorithm
typedef struct{
    int power;    // the power required to execute the schedule
    int runtime;  // the total runtime of the schedule
    int prefetch;
    int reuse;
}GA_Info;         

/******************************************************************************
 * NAME : initNapoleon
 * 
 * PURPOSE : Create and initialize all of Napoleon's static data structures
 * ARGUMENTS : char * = the name of the file that contains the DFG that you
 *                        wish to schedule (please see Ahmed Al-Wattar for 
 *                        more information
 * 
 * RETURNS : true if Napoleon was successfully initialized
 *           false if anything has gone wrong (error message printed to stderr)
 *****************************************************************************/
bool initNapoleon(char *);

/******************************************************************************
 * NAME : freeNapoleon
 * 
 * PURPOSE : free all of Napoleon's static data structures
 * PRECONDITION : This function should only be called if initNapoleon() 
 *                  returned true.
 *****************************************************************************/
void freeNapoleon(void);

/******************************************************************************
 * NAME : evaluateFitness
 * 
 * PURPOSE : Evaluate the fitness of one possible solution (a chromosome)
 * ARGUMENTS : Individual * = the possible solution to be evaluated
 * 
 * RETURNS : the fitness of the chromosome argument
 *****************************************************************************/
void evaluateFitness(Individual *);



/******************************************************************************
 * NAME : getNumGenes
 * 
 * PURPOSE : getter for the number of genes in a chromosome (AKA the number of
 *              nodes in the DFG that we are trying to schedule)
 * PRECONDITIONS : This function should only be called after initNapoleon()
 *                      returns true.
 * 
 * RETURNS : the number of genes in a chromosome
 *****************************************************************************/
int getNumGenes(void);

/******************************************************************************
 * NAME : getTaskType
 * 
 * PURPOSE : getter for the type of each gene in a chromosome (AKA the type of
 *              task of each node in the DFG)
 * ARGUMENTS : int = the position of the gene (task) in a chromosome
 * 
 * PRECONDITIONS : This function should only be called after initNapoleon()
 *                      returns true.
 * 
 * RETURNS : the type of task (or Operation) of the chosen task
 *****************************************************************************/
int getTaskType(int);



#endif	/* FITNESS_H */

