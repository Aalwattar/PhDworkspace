/*******************************************************************************
 * FILE NAME : fitness.h
 * 
 * Genetic Algorithm for Ahmed Al-Watter
 * 
 * PURPOSE : a header for fitness.c
 * 
 * AUTHOR : Jennifer Winer
 * Email  : jwiner@uoguelph.ca
 * 
 * DATE CREATED : May 16, 2013
 * LAST MODIFIED : May 16, 2013
 ******************************************************************************/

#include "population.h"

#ifndef FITNESS_H
#define	FITNESS_H

#define RUNTIME_WEIGHT 0.6
#define POWER_WEIGHT 1 - RUNTIME_WEIGHT

extern double CROSSOVER_RATE;
extern double MUTATION_RATE;

/******************************************************************************
 * NAME : evaluateFitness
 * 
 * PURPOSE : Evaluate the fitness of one possible solution (a chromosome)
 * ARGUMENTS : int * = the chromosome to be evaluated
 * 
 * RETURNS : the fitness of the chromosome argument
 *****************************************************************************/
double evaluateFitness(int *);

/******************************************************************************
 * NAME : evaluateRanks
 * 
 * PURPOSE : Orders the individuals in a population in ascending order
 *              based on fitness
 * ARGUMENTS : Population * = the Population that you wish to evaluate
 *****************************************************************************/
void evaluateRanks(Population *);

// FIX
Population * linearScalingSelection(Population *);

#endif	/* FITNESS_H */

