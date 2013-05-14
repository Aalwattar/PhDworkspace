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
 * LAST MODIFIED : May 12, 2013
 ******************************************************************************/

#include "individual.h"

double CROSSOVER_RATE = 0.85;
double MUTATION_RATE = 0.001;

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
    
    copy->fitness = 0;
    copy->cfitness = 0;
    copy->rfitness = 0; 
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