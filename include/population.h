/*******************************************************************************
 * FILE NAME : population.h
 * 
 * Genetic Algorithm practice for Ahmed Al-Watter
 * 
 * PURPOSE : a header for population.c
 * 
 * AUTHOR : Jennifer Winer
 * Email  : jwiner@uoguelph.ca
 * 
 * DATE CREATED : May 7, 2013
 * LAST MODIFIED : May 12, 2013
 ******************************************************************************/

#include "individual.h"

#ifndef POPULATION_H
#define	POPULATION_H

#define POP_SIZE        50      // MUST BE AN EVEN NUMBER
#define STOP_CONDITION  10000

int generation_num;

// REPRESENTATION
typedef struct{
    Individual member[POP_SIZE];
    double total_fitness;

    int best_individual;
}Population;


Population * genRandPopulation();
void freePopulation(Population *);

// SELECTION ALGORITHM - generate mating pool
Population * generateMatingPool(Population *);

// COUPLING ALGORITHM - choose pairs to mate
void generateNextGeneration(Population *);

// REPLACEMENT ALGORITHM - generational, steady state?, replace worst?
        // FIX - I AM ASSUMING THAT THE ENTIRE GENERATION IS REPLACED FOR NOW

// FIND THE BEST INDIVIDUAL - when the algorithm ends

// TERMINATION CONDITION

#endif	/* POPULATION_H */

