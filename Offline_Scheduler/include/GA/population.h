/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 7, 2013
 * Modified : May 30, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : population.h
 * 
 * Purpose  : a header for population.c
 ******************************************************************************/



#ifndef POPULATION_H
#define	POPULATION_H

#include "individual.h"

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
 * PURPOSE : Creates a population of individuals. Each individual will
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
 * PURPOSE : Frees all dynamically allocated data from within a Population
 * ARGUMENTS : Population * = the population that you wish to free
 * 
 * PRECONDITIONS : the Population passed in as a parameter must have been
 *      previously created by genRandPopulation();
 *****************************************************************************/
void freePopulation(Population *);



/******************************************************************************
 * NAME : generateNextGeneration
 * 
 * PURPOSE : Perform recombination and mutation on the provided Population
 * ARGUMENTS : Population * = the population that you wish to transform
 *****************************************************************************/
void generateNextGeneration(Population *);




/******************************************************************************
 * NAME : setCrossoverRate
 * 
 * PURPOSE : a setter for the crossover rate
 * ARGUMENTS : char * = a string that contains a real number between 0 and 1 
 *****************************************************************************/
void setCrossoverRate(char *);

/******************************************************************************
 * NAME : getCrossoverRate
 * 
 * PURPOSE : getter for the crossover rate 
 * RETURNS : The rate of crossover as a number between 0 and 1
 *****************************************************************************/
double getCrossoverRate(void);


/******************************************************************************
 * NAME : setMutationRate
 * 
 * PURPOSE : a setter for the mutation rate
 * ARGUMENTS : char * = a string that contains a real number between 0 and 1 
 *****************************************************************************/
void setMutationRate(char *);

/******************************************************************************
 * NAME : getMutationRate
 * 
 * PURPOSE : getter for the mutation rate 
 * RETURNS : The rate of mutation as a number between 0 and 1
 *****************************************************************************/
double getMutationRate(void);


/******************************************************************************
 * NAME : setPopSize
 * 
 * PURPOSE : a setter for the population size
 * ARGUMENTS : char * = a string that contains a number between 1 and 10000
 *****************************************************************************/
void setPopSize(char *);

/******************************************************************************
 * NAME : getCrossoverRate
 * 
 * PURPOSE : getter for the population size
 * RETURNS : the number of individuals in the population
 *****************************************************************************/
int getPopSize(void);



// FIX - incomplete!!!
// REPLACEMENT ALGORITHM - generational, steady state?, replace worst?
        // FIX - I AM ASSUMING THAT THE ENTIRE GENERATION IS REPLACED FOR NOW

// FIND THE BEST INDIVIDUAL - when the algorithm ends

// TERMINATION CONDITION

#endif	/* POPULATION_H */

