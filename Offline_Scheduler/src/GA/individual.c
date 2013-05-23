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
 * Filename : Individual.c
 * 
 * Purpose  : All methods for the creation and manipulation of a possible
 *              solution to the problem (an individual in the population)
 ******************************************************************************/

#include "individual.h"

#include <stdlib.h>
#include "problem.h"

void initRandIndividual(Individual * individual){
    int i;
    
    individual->encoding = malloc(sizeof(int) * (template->num_genes));
    
    for(i=0; i<template->num_genes; i++){
        individual->encoding[i] = arch_library[template->oper[i]].num_impl
                * randomNumber();
    }
    
    individual->fitness = 0;
    individual->cfitness = 0;
    individual->rfitness = 0;
}

void freeIndividual(Individual * i){
    free(i->encoding);
}

void duplicateIndividual(Individual * copy, Individual * original){
    int i;
    
    copy->encoding = malloc(sizeof(int) * (template->num_genes));
    
    for(i=0; i<template->num_genes; i++){
        copy->encoding[i] = original->encoding[i];
    }
    
    copy->fitness = original->fitness;
    copy->cfitness = original->cfitness;
    copy->rfitness = original->rfitness; 
}

void mutate(Individual * ind){
    int i;
    
    for(i=0; i<template->num_genes; i++){
        if(randomNumber() < MUTATION_RATE){
            ind->encoding[i] = arch_library[template->oper[i]].num_impl
                * randomNumber();
        }
    }
}

void crossover(Individual * p1, Individual * p2){
    int cross1, cross2;
    int temp;
    int i;
    
    cross1 = template->num_genes * randomNumber();
    cross2 = template->num_genes * randomNumber();
    
    while(cross1 == cross2){
        cross2 = template->num_genes * randomNumber();
    }
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