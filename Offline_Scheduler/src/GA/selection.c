/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 16, 2013
 * Modified : May 30, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : selection.c
 * 
 * Purpose  : A library of selection policies
 ******************************************************************************/

#include "selection.h"
#include "replacement.h"

#include <stdlib.h>
#include <limits.h>



Population * tournamentSelection(Population * original, int pop_size){
    Population * mating_pool;
    int p1, p2;
    int i;

    mating_pool = malloc(sizeof (Population));
    mating_pool->member = malloc(sizeof (Individual) * pop_size);
    mating_pool->size = pop_size;

    for(i = 0; i < pop_size; i++){
        p1 = randomNumber() * original->size;
        p2 = randomNumber() * original->size;

        if(original->member[p1].fitness <= original->member[p2].fitness){
            duplicateIndividual(&(mating_pool->member[i]), &(original->member[p1]));
        }else{
            duplicateIndividual(&(mating_pool->member[i]), &(original->member[p2]));
        }
    }
    return mating_pool;
}


Population * randomSelection(Population * original, int pop_size){
    Population * mating_pool;
    int randInd;
    int i;

    mating_pool = malloc(sizeof (Population));
    mating_pool->member = malloc(sizeof (Individual) * pop_size);
    mating_pool->size = pop_size;

    for(i=0; i < pop_size; i++){
        randInd = randomNumber() * original->size;
        duplicateIndividual(&(mating_pool->member[i]), &(original->member[randInd]));
    }

    return mating_pool;
}



Individual * findBest(Population * pop){
    int best_index;
    int best_fitness;
    int i;
    
    best_index = 0;
    best_fitness = (pop->member[0]).fitness;
    
    for(i=1; i < pop->size; i++){
        if((pop->member[i]).fitness < best_fitness){
            best_fitness = (pop->member[i]).fitness;
            best_index = i;
        }
    }
    
    return &(pop->member[best_index]);
}
