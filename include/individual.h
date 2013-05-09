/*******************************************************************************
 * FILE NAME : individual.h
 * 
 * Genetic Algorithm practice for Ahmed Al-Watter
 * 
 * PURPOSE : a header for individual.c
 * 
 * AUTHOR : Jennifer Winer
 * Email  : jwiner@uoguelph.ca
 * 
 * DATE CREATED : May 7, 2013
 * LAST MODIFIED : May 8, 2013
 ******************************************************************************/

#include "problem.h"

#ifndef INDIVIDUAL_H
#define	INDIVIDUAL_H

// CHANGE ALL STATIC VARIABLES TO NON-POINTERS



// REPRESENTATION OF THE PROBLEM
typedef struct{
    char * gene;
    
    double fitness;
    double rfitness;
    double cfitness;
} Individual;

static int num_genes;
static int num_alleles[4];

//Individual * initRandomIndividual(){
//    Individual * chromosome;
//    int i;
//    
//    chromosome = malloc(sizeof(Individual));
//    chromosome->gene = malloc(sizeof(char) * )
//    
//    for(i=0; i<)
//}

// FITNESS FUNCTION

// MUTATION OPERATOR

// CROSSOVER OPERATOR

#endif	/* INDIVIDUAL_H */

