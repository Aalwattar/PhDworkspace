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
double evaluateFitness(int *);

void evaluateRanks(Population *);

Population * linearScalingSelection(Population *);

#endif	/* FITNESS_H */

