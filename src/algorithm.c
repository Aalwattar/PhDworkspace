/*******************************************************************************
 * FILE NAME : algorithm.c
 * 
 * Genetic Algorithm practice for Ahmed Al-Watter
 * 
 * PURPOSE : Contains main and executes the GA
 * 
 * AUTHOR : Jennifer Winer
 * Email  : jwiner@uoguelph.ca
 * 
 * DATE CREATED : May 7, 2013
 * LAST MODIFIED : May 13, 2013
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "population.h"

// stopping conditions
#define STOP_CONDITION 10000

int generation_num;

// different combinations of the various sub-algorithms


// IDEAL SOLUTION:
//      - all 0's
//      - fitness = 8379

int main(int argc, char * argv[]){
    Population * pop;
    Population * mating_pop;
    int i, j;
    
    initProblem(ARCH_FILENAME, DFG_FILENAME);
    pop = genRandPopulation();
    
    printf("Starting Population:\n");
    for(i=0; i<POP_SIZE; i++){
        for(j=0; j<template->num_genes; j++){
            printf("%d", pop->member[i].encoding[j]);
        }
        printf("\n");
    }
    
    while(generation_num < STOP_CONDITION){
        mating_pop = selectMatingPool(pop);
        freePopulation(pop);
        
        generateNextGeneration(mating_pop);
        pop = mating_pop;
        
        generation_num++;
    }
    
    printf("\nFinal Population:\n");
    for(i=0; i<POP_SIZE; i++){
        for(j=0; j<template->num_genes; j++){
            printf("%d", pop->member[i].encoding[j]);
        }
        printf("\n");
    }
    
    freePopulation(pop);
    freeProblem();
    return 0;
}


// COMMENTING TEMPLATE

/******************************************************************************
 * NAME : initRandGenerator
 * PURPOSE : 
 * ARGUMENTS : 
 * PRECONDITIONS : 
 * RETURNS : 
 * NOTE : 
 *****************************************************************************/


    // FIX - validate each of the arguments
    // FIX - Improve the error message
//    if(argc < 2){
//        fprintf(stderr, "Improper arguments. \nPlease try again, using the following format : "
//                "\n\tGA.exe <rand_seed> \n\nFor more information, please view the README file");
//        exit(1);
//    }