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
#include "population.h"
#include "replacement.h"

#include <stdlib.h>

// FIX -  Implement a steady state variant that selects two individuals at a time?


Population * tournamentSelection(Population * original){
    Population * mating_pool;
    int pop_size;
    int p1, p2;
    int i;

    pop_size = getPopSize();
    mating_pool = malloc(sizeof (Population));
    mating_pool->member = malloc(sizeof (Individual) * pop_size);

    for(i = 0; i < pop_size; i++){
        p1 = randomNumber() * pop_size;
        p2 = randomNumber() * pop_size;

        if(original->member[p1].fitness <= original->member[p2].fitness){
            duplicateIndividual(&(mating_pool->member[i]), &(original->member[p1]));
        }else{
            duplicateIndividual(&(mating_pool->member[i]), &(original->member[p2]));
        }
    }

    generateNextGeneration(mating_pool, pop_size);
    freePopulation(original);
    return mating_pool;
}



void steadyStateSelection(Population * original){
    Population * mating_pool;
    int pop_size;
    int randInd;
    int i;

    pop_size = getPopSize();
    mating_pool = malloc(sizeof (Population));
    mating_pool->member = malloc(sizeof (Individual) * 2);


    randInd = randomNumber() * pop_size;
    duplicateIndividual(&(mating_pool->member[0]), &(original->member[randInd]));

    randInd = randomNumber() * pop_size;
    duplicateIndividual(&(mating_pool->member[1]), &(original->member[randInd]));

    // FIX - make pop_size a variable of that population!
    generateNextGeneration(mating_pool, 2);
    evaluateRanks(original);

    replaceWorst(original, mating_pool, 2);

    for(i=0; i<2; i++)
        freeIndividual(&(mating_pool->member[i])); 
    free(mating_pool);
}



//// FIX - NON-FUNCTIONAL ATM = needs more testing. 
//        // My current fitness function creates fitnesses that are far too uniform!
//        // this method does not work unless we increase selection pressure (eg. scaling)
//Population * rouletteWheelSelection(Population * original){
//    Population * mating_pool;
//    double parent;
//    int i, j;
//    
//    original->total_fitness = 0;
//    for(i=0; i<POP_SIZE; i++){
//        original->member[i].fitness = evaluateFitness(original->member[i].encoding);
//        original->total_fitness = original->total_fitness + original->member[i].fitness;
//    }
//    
//    for(i=0; i<POP_SIZE; i++)
//        original->member[i].fitness = 1.0 - (original->member[i].fitness / (double) original->total_fitness);
//    
//    original->total_fitness = 0;
//    for(i=0; i<POP_SIZE; i++)
//        original->total_fitness = original->total_fitness + original->member[i].fitness;
//   
//    
//    for(i=0; i<POP_SIZE; i++){
//        original->member[i].rfitness = original->member[i].fitness / original->total_fitness;
//        
//        if(i > 0)
//            original->member[i].cfitness = original->member[i-1].cfitness + original->member[i].rfitness;
//        else
//            original->member[i].cfitness = original->member[i].rfitness;  
//    }
//    
////    for(i=0; i<POP_SIZE; i++){
////        printf("%.2d)  encoding = ", (i+1));
////        for(j=0; j<template->num_genes; j++){
////            printf("%d", original->member[i].encoding[j]);
////        }
////        printf("  fit = %.5lf  rfit = %.5lf  cfit = %.5lf\n", original->member[i].fitness, original->member[i].rfitness, original->member[i].cfitness);
////    }
//    
//    mating_pool = malloc(sizeof(Population));
//    mating_pool->member = malloc(sizeof(Individual) * POP_SIZE);
//    
//    for(i=0; i<POP_SIZE; i++){
//        parent = randomNumber();
//        
//        for(j=0; (original->member[j].cfitness < parent) && (j <= POP_SIZE); j++);
//        duplicateIndividual(&(mating_pool->member[i]), &(original->member[j]));
////        printf("%.2d) Random = %.5lf  Chosen = %.2d\n", (i+1), parent, (j+1));
//    }
//    
////    for(i=0; i<POP_SIZE; i++){
////        for(j=0; j<template->num_genes; j++){
////            fprintf(stdout, "%d", mating_pool->member[i].encoding[j]);
////        }
////        fprintf(stdout, "\n");
////    }
////    printf("\n\n");
//    
//    return mating_pool;
//}




//void deriveParameters(Population * pop, double * alpha, double * beta);
//
//Population * linearScalingSelection(Population * original){
//    Population * mating_pool;
//    double alpha, beta;
//    double parent;
//    int i, j;
//    
//    
//    deriveParameters(original, &alpha, &beta);
//    
//    original->total_fitness = 0;
//    for(i=0; i<POP_SIZE; i++){
//        original->member[i].rfitness = alpha * original->member[i].fitness + beta;
//        original->total_fitness = original->total_fitness + original->member[i].rfitness;
//    }
//    
//    for(i=0; i<POP_SIZE; i++)
//        original->member[i].rfitness = 1.0 - (original->member[i].rfitness / original->total_fitness);
//    
//    original->total_fitness = 0;
//    for(i=0; i<POP_SIZE; i++)
//        original->total_fitness = original->total_fitness + original->member[i].rfitness;
//    
//    for(i=0; i<POP_SIZE; i++){
//        if(i > 0)
//            original->member[i].cfitness = original->member[i-1].cfitness + original->member[i].rfitness / original->total_fitness;
//        else
//            original->member[i].cfitness = original->member[i].rfitness / original->total_fitness; 
//    }
//    
////    for(i=0; i<POP_SIZE; i++){
////        printf("%.2d)  encoding = ", (i+1));
////        for(j=0; j<template->num_genes; j++){
////            printf("%d", original->member[i].encoding[j]);
////        }
////        printf("  rfit = %.5lf  cfit = %.5lf\n", original->member[i].rfitness, original->member[i].cfitness);
////    }
//    
//    
//    mating_pool = malloc(sizeof(Population));
//    mating_pool->member = malloc(sizeof(Individual) * POP_SIZE);
//
//    for(i=0; i<POP_SIZE; i++){
//        parent = randomNumber();
//        
//        for(j=0; (original->member[j].cfitness < parent) && (j <= POP_SIZE); j++);
//        duplicateIndividual(&(mating_pool->member[i]), &(original->member[j]));
////        printf("%.2d) Random = %.5lf  Chosen = %.2d\n", (i+1), parent, (j+1));
//    }
//
////    for(i=0; i<POP_SIZE; i++){
////        for(j=0; j<template->num_genes; j++){
////            fprintf(stdout, "%d", mating_pool->member[i].encoding[j]);
////        }
////        fprintf(stdout, "\n");
////    }
////    printf("\n\n");
//    
//    return mating_pool;
//}
//
//void deriveParameters(Population * pop, double * alpha, double * beta){
//    double max, min, avg;
//    int scaler = 3;
//    int i;
//    
//    pop->total_fitness = 0;
//    for(i=0; i<POP_SIZE; i++){
//        pop->member[i].fitness = evaluateFitness(pop->member[i].encoding);
//        pop->total_fitness = pop->total_fitness + pop->member[i].fitness;
//    }
//    
//    max = pop->member[0].fitness;
//    min = pop->member[0].fitness;
//    
//    for(i=0; i<POP_SIZE; i++){
//        if(pop->member[i].fitness > max)
//            max = pop->member[i].fitness;
//        else if(pop->member[i].fitness < min)
//            min = pop->member[i].fitness;
//    }
//    avg = pop->total_fitness / POP_SIZE;
//    
//    // FIX - need some way to indicate when the population has converged
//    
//    if(min > ((scaler * avg - max) / (scaler - 1))){
//        *alpha = (avg * (scaler - 1)) / (max - avg);
//        *beta  = (avg * (max - scaler * avg)) / (max - avg);
//    }
//    else{
//        *alpha = (avg) / (avg - min);
//        *beta  = (avg * min) / (avg - min);
//    }
//    
////    printf("alpha = %lf\tbeta = %lf\n\n", *alpha, *beta);
//}


Population * sigmaScalingSelection(Population *);

Population * powerScalingSelection(Population *);

Population * linearRankingSelection(Population *);

Population * softTournamentSelection(Population *);


// The compare function for qsort
int compare(const void *, const void *);

void evaluateRanks(Population * pop){
    qsort(pop->member, getPopSize(), sizeof(Individual), compare);
}

int compare(const void * p1, const void * p2){
    return ((Individual *)p1)->fitness - ((Individual *)p2)->fitness;
}