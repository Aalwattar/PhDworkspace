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

#include "population.h"

#ifndef FITNESS_H
#define	FITNESS_H

#define RUNTIME_WEIGHT 0.6
#define POWER_WEIGHT 1 - RUNTIME_WEIGHT


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

