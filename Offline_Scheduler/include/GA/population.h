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
 * Filename : population.h
 * 
 * Purpose  : a header for population.c
 ******************************************************************************/



#ifndef POPULATION_H
#define	POPULATION_H

#include "individual.h"


extern int POP_SIZE;

/******************************************************************************
 *****************         POPULATION REPRESENTATION         ******************
 *****************************************************************************/
typedef struct{
    Individual * member;
    double total_fitness;
}Population;



/******************************************************************************
 ******************         POPULATION MANIPULATION         *******************
 *****************************************************************************/

/******************************************************************************
 * NAME : genRandPopulation
 * 
 * PURPOSE : Creates a population of size POP_SIZE. Each individual will
 *              contain randomly generated genes, but is still a valid solution
 * RETURNS : A population of randomly generated individuals
 * 
 * NOTE : the population that is returned should eventually be freed through
 *              the usage of freePopulation();
 *****************************************************************************/
Population * genRandPopulation(void);

/******************************************************************************
 * NAME : freePopulation
 * 
 * PURPOSE : Frees all dynamically allocated data from within an Population
 * ARGUMENTS : Population * = the population that you wish to free
 * 
 * PRECONDITIONS : the Population passed in as a parameter must have been
 *      previously created by genRandPopulation();
 *****************************************************************************/
void freePopulation(Population *);



/******************************************************************************
 * NAME : generateNextGeneration
 * 
 * PURPOSE : Perform recombination and mutation on the mating pool
 * ARGUMENTS : Population * = the population that serves as the mating pool
 * 
 * PRECONDITIONS : the Population passed in as a parameter must have been
 *      previously created by genRandPopulation();
 * RETURNS : a new population that consists of individuals chosen to mate
 *              (in a random order)
 *           the same Individual may appear more than once in the mating 
 *              population
 *****************************************************************************/
void generateNextGeneration(Population *);

// REPLACEMENT ALGORITHM - generational, steady state?, replace worst?
        // FIX - I AM ASSUMING THAT THE ENTIRE GENERATION IS REPLACED FOR NOW

// FIND THE BEST INDIVIDUAL - when the algorithm ends

// TERMINATION CONDITION

#endif	/* POPULATION_H */

