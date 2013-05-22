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
 * Filename : individual.h
 * 
 * Purpose  : a header for individual.c
 ******************************************************************************/

#include "problem.h"

#ifndef INDIVIDUAL_H
#define	INDIVIDUAL_H

extern double CROSSOVER_RATE;
extern double MUTATION_RATE;

/******************************************************************************
 ******************         SOLUTION REPRESENTATION         *******************
 *****************************************************************************/

typedef struct{
    int * encoding;     // their genotype
    
    double fitness;     // true fitness value
    double rfitness;    // fitness relative to population
    double cfitness;    // cumulative fitness
} Individual;



/******************************************************************************
 ******************         INDIVIDUAL MANIPULATION         *******************
 *****************************************************************************/

/******************************************************************************
 * NAME : initRandIndividual
 * 
 * PURPOSE : Initializes an individual with a randomly generated genotype
 * ARGUMENTS : Individual * = the individual to initialize
 * 
 * PRECONDITIONS : the Individual struct must already exist in memory
 *****************************************************************************/
void initRandIndividual(Individual *);

/******************************************************************************
 * NAME : duplicateIndividual
 * 
 * PURPOSE : Creates a deep copy of the individual passed in as an argument 
 * ARGUMENTS : Individual * = the destination for the copy
 *             Individual * = the individual that you wish to copy
 * 
 * RETURNS : An individual that contains the same data as the individual passed
 *              in as an argument
 * 
 * PRECONDITIONS : the Individual passed in as a parameter must have been
 *      previously created either through initRandIndividual() or 
 *      duplicateIndividual().
 *****************************************************************************/
void duplicateIndividual(Individual *, Individual *);

/******************************************************************************
 * NAME : freeIndividual
 * 
 * PURPOSE : Frees all dynamically allocated data from within an Individual
 * ARGUMENTS : Individual * = the individual that you wish to free
 * 
 * PRECONDITIONS : the Individual passed in as a parameter must have been
 *      previously created either through initRandIndividual() or 
 *      duplicateIndividual().
 *****************************************************************************/
void freeIndividual(Individual *);



/******************************************************************************
 * NAME : mutate
 * 
 * PURPOSE : Perform mutation on an Individual. The probability of mutating one
 *      individual gene (MUTATION_RATE) is pre-determined by the user.
 * ARGUMENTS : Individual * = the Individual that you wish to mutate
 * 
 * PRECONDITIONS : the Individual passed in as a parameter must have been
 *      previously created either through initRandIndividual() or 
 *      duplicateIndividual().
 *****************************************************************************/
void mutate(Individual *);

/******************************************************************************
 * NAME : crossover
 * 
 * PURPOSE : Perform crossover on the two individuals provided. The probability
 *       of mutating one individual gene (CROSSOVER_RATE) is pre-determined by
 *       the user.
 * ARGUMENTS : Individual * = the first individual that will undergo crossover
 *             Individual * = the second individual that will undergo crossover
 * 
 * PRECONDITIONS : the Individuals passed in as a parameter must have been
 *      previously created either through initRandIndividual() or 
 *      duplicateIndividual().
 *****************************************************************************/
void crossover(Individual *, Individual *);

#endif	/* INDIVIDUAL_H */

