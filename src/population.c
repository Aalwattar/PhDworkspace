/*******************************************************************************
 * FILE NAME : population.c
 * 
 * Genetic Algorithm practice for Ahmed Al-Watter
 * 
 * PURPOSE : library for the representation and manipulation of a population
 *       of individuals
 * 
 * AUTHOR : Jennifer Winer
 * Email  : jwiner@uoguelph.ca
 * 
 * DATE CREATED : May 7, 2013
 * LAST MODIFIED : May 9, 2013
 ******************************************************************************/

#include "population.h"

Population * genRandPopulation(){
	Population * pop;
	int i;

	pop = malloc(sizeof(Population));
	pop->organism = malloc(sizeof(Individual) * POP_SIZE);
    pop->total_fitness = 0;
	pop->best_individual = -1;

 	for (i=0; i<POP_SIZE; i++){
        initRandIndividual(&(pop->organism[i]));
    }
}