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
 * LAST MODIFIED : May 7, 2013
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "individual.h"

// stopping conditions

// different combinations of the various sub-algorithms


// IDEAL SOLUTION:
//      - all 0's
//      - fitness = 8379

int main(int argc, char * argv[]){
    Individual p1, p2;
    Individual * copy1, * copy2;
    int i;
    
    initProblem(ARCH_FILENAME, DFG_FILENAME);
    
    // for(i=0; i<50; i++){
    //     person = generateRandIndividual();
    //     calculateFitness(person);
    //     printf("Person : %s\tFitness : %.2lf\n", person->encoding, person->fitness);
    //     freeIndividual(person);
    // }
    
    p1.encoding = malloc(sizeof(int) * template->num_genes);
    p2.encoding = malloc(sizeof(int) * template->num_genes);
    
    for(i=0; i<template->num_genes; i++){
        p1.encoding[i] = 1;
        p2.encoding[i] = 0;
    }
    
    for(int i=0; i<template->num_genes; i++){
        copy1 = duplicateIndividual(&p1);
        copy2 = duplicateIndividual(&p2);

        crossover(copy1, copy2);

        freeIndividual(copy1);
        freeIndividual(copy2);
        free(copy1);
        free(copy2);
    }
    
    free(p1.encoding);
    free(p2.encoding);
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

//
//j = 0;
//    for(i=0; i<template->num_genes; i++){
//        strncpy(gene, &(person->encoding[j]), template->gene_length[i]);
//        gene[template->gene_length[i]] = '\0';
//
//        j = j + template->gene_length[i];
//        
//        op = template->opr[i];
//        arch = encodingToAllele(gene);
//        
//        evaluateGene(gene, op);
//        printf("Gene %.2d)\nOper : %d\nArch : %d\nFit  : %lf\n\n", 
//                (i+1), (op+1), (arch+1), evaluateGene(gene, op));
//    }
//    
//    calculateFitness(person);
//    printf("The overall fitness is : %lf", person->fitness);