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
 * LAST MODIFIED : May 10, 2013
 ******************************************************************************/

#include "problem.h"

#ifndef INDIVIDUAL_H
#define	INDIVIDUAL_H


#define RUNTIME_WEIGHT 0.6
#define POWER_WEIGHT 1 - RUNTIME_WEIGHT

#define CROSSOVER_RATE  0.8               /* probability of crossover */
#define MUTATION_RATE   0.05           /* probability of mutation */
// REPRESENTATION OF THE PROBLEM
typedef struct{
    int * encoding;
    
    double fitness;
    double rfitness;
    double cfitness;
} Individual;

void initRandIndividual(Individual *);
Individual * duplicateIndividual(Individual *);
void freeIndividual(Individual *);

// FITNESS FUNCTION
void evaluateFitness(Individual *);

// MUTATION OPERATOR
void mutate(Individual *);

// CROSSOVER OPERATOR
void crossover(Individual *, Individual *);

#endif	/* INDIVIDUAL_H */

