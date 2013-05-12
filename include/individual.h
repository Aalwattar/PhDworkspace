/*******************************************************************************
 * FILE NAME : individual.h
 * 
 * Genetic Algorithm practice for Ahmed Al-Watter
 * 
 * PURPOSE : a header for individual.c
 * 
 * AUTHOR : Jennifer Winer
 * Email  : jwiner@uoguelph.ca
 * 
 * DATE CREATED : May 7, 2013
 * LAST MODIFIED : May 12, 2013
 ******************************************************************************/

#include "problem.h"

#ifndef INDIVIDUAL_H
#define	INDIVIDUAL_H


#define RUNTIME_WEIGHT 0.6
#define POWER_WEIGHT 1 - RUNTIME_WEIGHT

#define CROSSOVER_RATE  0.8
#define MUTATION_RATE   0.05



/******************************************************************************
 *******************        PROBLEM REPRESENTATION         ********************
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
 * ARGUMENTS : Individual * = the individual that you wish to copy
 * 
 * RETURNS : An individual that contains the same data as the individual passed
 *              in as an argument
 * 
 * PRECONDITIONS : the Individual passed in as a parameter must have been
 *      previously created either through initRandIndividual() or 
 *      duplicateIndividual().
 *****************************************************************************/
Individual * duplicateIndividual(Individual *);

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
 * NAME : evaluateFitness
 * 
 * PURPOSE : Evaluate the fitness of one possible solution (an Individual) and
 *      store that value inside the Individual struct
 * ARGUMENTS : Individual * = the Individual that you wish to evaluate
 * 
 * PRECONDITIONS : the Individual passed in as a parameter must have been
 *      previously created either through initRandIndividual() or 
 *      duplicateIndividual().
 *****************************************************************************/
void evaluateFitness(Individual *);

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

