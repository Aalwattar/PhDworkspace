/*******************************************************************************
 * FILE NAME : Individual.c
 * 
 * Genetic Algorithm for Ahmed Al-Watter
 * 
 * PURPOSE : library for the representation and manipulation of a possible
 *      solution to the problem (an individual in the population)
 * 
 * AUTHOR : Jennifer Winer
 * Email  : jwiner@uoguelph.ca
 * 
 * DATE CREATED : May 7, 2013
 * LAST MODIFIED : May 16, 2013
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