/*******************************************************************************
 * FILE NAME : Individual.c
 * 
 * Genetic Algorithm practice for Ahmed Al-Watter
 * 
 * PURPOSE : library for the representation and manipulation of a possible
 *      solution to the problem (an individual in the population)
 * 
 * AUTHOR : Jennifer Winer
 * Email  : jwiner@uoguelph.ca
 * 
 * DATE CREATED : May 7, 2013
 * LAST MODIFIED : May 10, 2013
 ******************************************************************************/

#include "individual.h"


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
    // FIX
    //free(i);
}

Individual * duplicateIndividual(Individual * original){
    Individual * copy;
    int i;
    
    copy = malloc(sizeof(Individual));
    copy->encoding = malloc(sizeof(int) * (template->num_genes));
    
    for(i=0; i<template->num_genes; i++){
        copy->encoding[i] = original->encoding[i];
    }
    
    copy->fitness = 0;
    copy->cfitness = 0;
    copy->rfitness = 0; 
    
    return copy;
}



void evaluateFitness(Individual * individual){
    double fitness;
    double runtime;
    double power;
    
    int oper;
    int allele;
    int i;
    
    fitness = 0;
    for(i=0; i<template->num_genes; i++){
        oper = template->oper[i];
        allele = individual->encoding[i];
        
        runtime = arch_library[oper].impl[allele].runtime;
        power = arch_library[oper].impl[allele].power;
    
        fitness = fitness + (RUNTIME_WEIGHT)*runtime + (POWER_WEIGHT)*power;
    }
    
    individual->fitness = fitness;
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
    
//    static int ind1 = 0;
//    static int ind2 = 0;
//    
//    printf("i=%d    j=%d\t", ind1, ind2);
//    
//    for(i=ind1; i <= ind2; i++){
//        temp = p1->encoding[i];
//        p1->encoding[i] = p2->encoding[i];
//        p2->encoding[i] = temp;
//    }
//    
//    if(ind1 > ind2){
//        printf("INVAILD %d is bigger than %d\n", ind1, ind2);
//    }
//    else{
//        for(i=0; i<template->num_genes; i++){
//            printf("%d", p1->encoding[i]);
//        }
//        printf("\n");
//    }
//    
//    
//    ind2++;
//    if(ind2 >= template->num_genes){
//        ind2 = 0;
//        ind1++;
//    }
    
    cross1 = template->num_genes * randomNumber();
    cross2 = template->num_genes * randomNumber();
    
    while(cross1 == cross2){
        cross2 = template->num_genes * randomNumber();
    }
    if(cross1 > cross2){
        temp = cross1;
        cross1 = cross2;
        cross2 = temp;
    }
    
    
    
    for(i=cross1; i <= cross2; i++){
        temp = p1->encoding[i];
        p1->encoding = p2->encoding[i];
        p2->encoding = temp;
    }
}