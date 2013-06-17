/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 16, 2013
 * Modified : June 6, 2013
 ******************************************************************************/

/*******************************************************************************
 * FILE NAME : replacement.c
 * 
 * PURPOSE : A library of replacement policies
 ******************************************************************************/

#include "replacement.h"
#include "population.h"

#include <stdio.h>
#include <stdlib.h>

// FIX - requires thorough testing before implementation
//Population * replaceWorst(Population * original, Population * replacements, int num_replaced){
//    Population * pop;
//    int pop_size = original->size;
//    int i, j;
//    
//    pop = malloc(sizeof(Population));
//    pop->size = pop_size;
//    pop->member = malloc(sizeof(Individual) * pop_size);
//    
//    // FIX - should only have 1 sort ?? 
//    sortByFitness(original);
//    sortByFitness(replacements);
//    
//    j=0;
//    for(i = 0 ; i < num_replaced; i++)
//        duplicateIndividual(&(pop->member[i]), &(replacements->member[j++]));
//
//    for(; i < pop_size; i++)
//        duplicateIndividual(&(pop->member[i]), &(original->member[i - num_replaced]));
//    
//    freePopulation(original);
//    freePopulation(replacements);
//    
//    return pop;
//}


Population * replaceAll(Population * original, Population * replacements){
    freePopulation(original);
    return replacements;
}

Population * retainBest(Population * original, Population * next_gen){
    Population * best_pop;
    int i;
    int o, n;
    
    best_pop = malloc(sizeof(Population));
    best_pop->size = original->size;
    best_pop->member = malloc(sizeof(Individual) * best_pop->size);
    
    sortByFitness(next_gen);
    o = n = 0;
    
    for(i=0; i<original->size; i++){
        if((original->member[0]).fitness < (next_gen->member[n]).fitness)
            duplicateIndividual(&(best_pop->member[i]), &(original->member[o++]));
        else
            duplicateIndividual(&(best_pop->member[i]), &(next_gen->member[n++]));
    }
    
    freePopulation(original);
    freePopulation(next_gen);
    
    return best_pop;
}

// FUTURE - replace parents instead of replace worst???
