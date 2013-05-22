/*******************************************************************************
 * FILE NAME : population.c
 * 
 * Genetic Algorithm for Ahmed Al-Watter
 * 
 * PURPOSE : library for the representation and manipulation of a population
 *       of individuals
 * 
 * AUTHOR : Jennifer Winer
 * Email  : jwiner@uoguelph.ca
 * 
 * DATE CREATED : May 7, 2013
 * LAST MODIFIED : May 17, 2013
 ******************************************************************************/

#include "population.h"

Population * genRandPopulation(){
    Population * pop;
    int i;

    pop = malloc(sizeof(Population));
    pop->member = malloc(sizeof(Individual) * POP_SIZE);
    pop->total_fitness = 0;

    for(i=0; i<POP_SIZE; i++){
        initRandIndividual(&(pop->member[i]));
    }
    
    return pop;
}

void freePopulation(Population * pop){
    int i;
    
    for(i=0; i<POP_SIZE; i++){
        freeIndividual(&(pop->member[i]));
    }
    
    free(pop->member);
    free(pop);
}



// a Generational algorithm
void generateNextGeneration(Population * pop){
    int i;
    
    for(i=0; i<POP_SIZE; i = i + 2){
        if(randomNumber() < CROSSOVER_RATE){
                crossover(&(pop->member[i]), &(pop->member[i+1]));
        }
    }
}