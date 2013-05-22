/*******************************************************************************
 * FILE NAME : fitness.c
 * 
 * Genetic Algorithm for Ahmed Al-Watter
 * 
 * PURPOSE : library for fitness evaluation
 * 
 * AUTHOR : Jennifer Winer
 * Email  : jwiner@uoguelph.ca
 * 
 * DATE CREATED : May 16, 2013
 * LAST MODIFIED : May 16, 2013
 ******************************************************************************/

#include "fitness.h"


double evaluateFitness(int * chromosome){
    double fitness;
    double runtime;
    double power;
    
    int oper;
    int allele;
    int i;
    
    fitness = 0.0;
    for(i=0; i<template->num_genes; i++){
        oper = template->oper[i];
        allele = chromosome[i];
        
        runtime = arch_library[oper].impl[allele].runtime;
        power = arch_library[oper].impl[allele].power;
    
        fitness = fitness + (RUNTIME_WEIGHT)*runtime + (POWER_WEIGHT)*power;
    }
    
    return fitness;
}



// The compare function for qsort
int compare(const void *, const void *);

void evaluateRanks(Population * pop){
    qsort(pop->member, POP_SIZE, sizeof(Individual), compare);
}

int compare(const void * p1, const void * p2){
    return ((Individual *)p1)->fitness - ((Individual *)p2)->fitness;
}


// a private helper function for linear scaling
void deriveParameters(Population *, double *, double *);

Population * linearScalingSelection(Population * original){
    Population * mating_pool;
    double alpha, beta;
    double parent;
    int i, j;
    
    
    deriveParameters(original, &alpha, &beta);
    
    original->total_fitness = 0;
    for(i=0; i<POP_SIZE; i++){
        original->member[i].rfitness = alpha * original->member[i].fitness + beta;
        original->total_fitness = original->total_fitness + original->member[i].rfitness;
    }
    
    for(i=0; i<POP_SIZE; i++)
        original->member[i].rfitness = 1.0 - (original->member[i].rfitness / original->total_fitness);
    
    original->total_fitness = 0;
    for(i=0; i<POP_SIZE; i++)
        original->total_fitness = original->total_fitness + original->member[i].rfitness;
    
    for(i=0; i<POP_SIZE; i++){
        if(i > 0)
            original->member[i].cfitness = original->member[i-1].cfitness + original->member[i].rfitness / original->total_fitness;
        else
            original->member[i].cfitness = original->member[i].rfitness / original->total_fitness; 
    }
    
//    for(i=0; i<POP_SIZE; i++){
//        printf("%.2d)  encoding = ", (i+1));
//        for(j=0; j<template->num_genes; j++){
//            printf("%d", original->member[i].encoding[j]);
//        }
//        printf("  rfit = %.5lf  cfit = %.5lf\n", original->member[i].rfitness, original->member[i].cfitness);
//    }
    
    
    mating_pool = malloc(sizeof(Population));
    mating_pool->member = malloc(sizeof(Individual) * POP_SIZE);

    for(i=0; i<POP_SIZE; i++){
        parent = randomNumber();
        
        for(j=0; (original->member[j].cfitness < parent) && (j <= POP_SIZE); j++);
        duplicateIndividual(&(mating_pool->member[i]), &(original->member[j]));
//        printf("%.2d) Random = %.5lf  Chosen = %.2d\n", (i+1), parent, (j+1));
    }

//    for(i=0; i<POP_SIZE; i++){
//        for(j=0; j<template->num_genes; j++){
//            fprintf(stdout, "%d", mating_pool->member[i].encoding[j]);
//        }
//        fprintf(stdout, "\n");
//    }
//    printf("\n\n");
    
    return mating_pool;
}

void deriveParameters(Population * pop, double * alpha, double * beta){
    double max, min, avg;
    // FIX - MAKE THIS A COMMAND LINE VARIABLE?
    // increasing this increases the selection pressure
    int scaler = 2;
    int i;
    
    pop->total_fitness = 0;
    for(i=0; i<POP_SIZE; i++){
        pop->member[i].fitness = evaluateFitness(pop->member[i].encoding);
        pop->total_fitness = pop->total_fitness + pop->member[i].fitness;
    }
    
    max = pop->member[0].fitness;
    min = pop->member[0].fitness;
    
    for(i=0; i<POP_SIZE; i++){
        if(pop->member[i].fitness > max)
            max = pop->member[i].fitness;
        else if(pop->member[i].fitness < min)
            min = pop->member[i].fitness;
    }
    avg = pop->total_fitness / POP_SIZE;
    
    // FIX - need some way to indicate when the population has converged
    
//    if(min > ((scaler * avg - max) / (scaler - 1))){
        *alpha = (avg * (scaler - 1)) / (max - avg);
        *beta  = (avg * (max - scaler * avg)) / (max - avg);
//    }
//    else{
//        *alpha = (avg) / (avg - min);
//        *beta  = (avg * min) / (avg - min);
//    }
    
//    printf("alpha = %lf\tbeta = %lf\n\n", *alpha, *beta);
}