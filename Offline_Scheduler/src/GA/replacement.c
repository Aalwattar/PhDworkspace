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

void replaceWorst(Population * original, Population * replacements, int num_replaced){
    int pop_size = original->size;
    int i, j;
    
    // FUTURE - should only have 1 sort ?? 
    sortByFitness(original);
    sortByFitness(replacements);
    
    j=0;
    for(i = 0 ; i < num_replaced; i++){
        freeIndividual(&(original->member[pop_size -1 -i]));
        duplicateIndividual(&(original->member[pop_size -1 -i]), &(replacements->member[j++]));
    }
}

void replaceAll(Population * original, Population * replacements){
    int pop_size = original->size;
    int i;
    
    for(i = 0 ; i < pop_size; i++){
        freeIndividual(&(original->member[i]));
        duplicateIndividual(&(original->member[i]), &(replacements->member[i]));
    }
}

// FUTURE - replace parents instead of replace worst???
