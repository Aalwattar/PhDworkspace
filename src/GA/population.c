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
 * Filename : population.c
 * 
 * Purpose  : A library for the creation and manipulation of a population
 *              of individuals
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