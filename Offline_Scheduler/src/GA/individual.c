/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 7, 2013
 * Modified : May 24, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : Individual.c
 * 
 * Purpose  : All methods for the creation and manipulation of a possible
 *              solution to the problem (an individual in the population)
 ******************************************************************************/

#include "individual.h"
#include "fitness.h"
#include "population.h"
#include "util.h"

#include <stdlib.h>


void initRandIndividual(Individual * ind){
    int i;
    
    ind->encoding = malloc(sizeof(int) * getNumGenes());
    
    for(i=0; i<getNumGenes(); i++)
        ind->encoding[i] = getNumArch(getTaskType(i)) * randomNumber();
    
    // FIX - remove AFTER testing the patch to the Napoleon 2D bug
//    ind->encoding[0] = 1;
//    ind->encoding[1] = 1;
//    ind->encoding[2] = 2;
//    ind->encoding[3] = 1;
//    ind->encoding[4] = 0;
//    ind->encoding[5] = 1;
//    ind->encoding[6] = 0;
//    ind->encoding[7] = 2;
//    ind->encoding[8] = 1;
//    ind->encoding[9] = 3;
    
    ind->fitness = 0;
    ind->cfitness = 0;
    ind->rfitness = 0;
}

void freeIndividual(Individual * i){
    free(i->encoding);
}

void duplicateIndividual(Individual * copy, Individual * original){
    int i;
    
    copy->encoding = malloc(sizeof(int) * getNumGenes());
    
    for(i=0; i<getNumGenes(); i++)
        copy->encoding[i] = original->encoding[i];
    
    copy->fitness = original->fitness;
    copy->cfitness = original->cfitness;
    copy->rfitness = original->rfitness; 
}

void mutate(Individual * ind){
    int i;
    
    for(i=0; i<getNumGenes(); i++)
        if(randomNumber() < getMutationRate())
            ind->encoding[i] = getNumArch(getTaskType(i)) * randomNumber();
}                              

void crossover(Individual * p1, Individual * p2){
    int cross1, cross2;
    int temp;
    int i;
    
    cross1 = getNumGenes() * randomNumber();
    cross2 = getNumGenes() * randomNumber();
    
    while(cross1 == cross2)
        cross2 = getNumGenes() * randomNumber();

    if(cross1 > cross2){
        // Unnecessary, but fun!
//        Individual * swap;
//        swap = p1;
//        p1 = p2;
//        p2 = swap;
        
        temp = cross1; 
        cross1 = cross2;
        cross2 = temp;
    }
    
    for(i=cross1; i <= cross2; i++){
        temp = p1->encoding[i];
        p1->encoding[i] = p2->encoding[i];
        p2->encoding[i] = temp;
    }
}