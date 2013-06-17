/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 7, 2013
 * Modified : June 6, 2013
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
    int         size;
}Population;

/******************************************************************************
 ******************         POPULATION MANIPULATION         *******************
 *****************************************************************************/

/******************************************************************************
 * NAME : genRandPopulation
 * 
 * PURPOSE : Creates a population of individuals. Each individual will
 *              contain randomly generated genes, but is still a valid solution
 * ARGUMENTS : int = the number of individuals in the population
 * 
 * RETURNS : A population of randomly generated individuals
 * 
 * NOTE : the population that is returned should eventually be freed through
 *              the usage of freePopulation();
 *****************************************************************************/
Population * genRandPopulation(int);

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


// FIX
/******************************************************************************
 * NAME : evolvePopulation
 * 
 * PURPOSE : Perform recombination and mutation on the provided Population
 * ARGUMENTS : Population * = the population that you wish to transform
 *             int = the size of that population;
 *****************************************************************************/
void evolvePopulation(Population *, int, int);


/******************************************************************************
 * NAME : determineFitness
 * 
 * PURPOSE : Calculate the fitness of all members of a population
 * ARGUMENTS : Population * = the population that you wish evaluate
 *****************************************************************************/
void determineFitness(Population *);



/******************************************************************************
 * NAME : evaluateRanks
 * 
 * PURPOSE : Orders the individuals in a population in ascending order
 *              based on fitness
 * ARGUMENTS : Population * = the Population that you wish to evaluate
 *****************************************************************************/
void sortByFitness(Population *);



/******************************************************************************
 * NAME : setCrossoverRate
 * 
 * PURPOSE : a setter for the crossover rate
 * ARGUMENTS : char * = a real number between 0 and 1 
 *****************************************************************************/
void setCrossoverRate(double);

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
 * ARGUMENTS : char * = a real number between 0 and 1 
 *****************************************************************************/
void setMutationRate(double);

/******************************************************************************
 * NAME : getMutationRate
 * 
 * PURPOSE : getter for the mutation rate 
 * RETURNS : The rate of mutation as a number between 0 and 1
 *****************************************************************************/
double getMutationRate(void);



/******************************************************************************
 * NAME : printPopulation
 * 
 * PURPOSE : Print all chromosomes with their fitness values
 * ARGUMENTS : Population * = the population that you wish visualize
 *****************************************************************************/
void printPopulation(Population *);

/******************************************************************************
 * NAME : printSummaryStatistics
 * 
 * PURPOSE : Statistical information about a population. 
 * ARGUMENTS : Population * = the population that you wish visualize
 *****************************************************************************/
void printSummaryStatistics(Population *);


#endif	/* POPULATION_H */

