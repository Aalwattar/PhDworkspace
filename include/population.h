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
 * LAST MODIFIED : May 9, 2013
 ******************************************************************************/

#include "individual.h"

#ifndef POPULATION_H
#define	POPULATION_H

#define POP_SIZE  50               /* population size */
#define MAXGENS   1000             /* max. number of generations */



int generation_num;

// REPRESENTATION
typedef struct{
    Individual * organism;
    double total_fitness;

    int best_individual;
}Population;


Population * genRandPopulation();



// SELECTION ALGORITHM - generate mating pool

// COUPLING ALGORITHM - choose pairs to mate

// REPLACEMENT ALGORITHM - generational, steady state?, replace worst?

// FIND THE BEST INDIVIDUAL - when the algorithm ends

// TERMINATION CONDITION

#endif	/* POPULATION_H */

