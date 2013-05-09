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
#define MAX_GENE_SIZE 20

typedef struct{
    char * encoding;
    
    double fitness;
    double rfitness;
    double cfitness;
} Individual;

Individual * generateRandIndividual();
void generateRandGene(Individual * individual, int gene_num, int chrom_position);
void freeIndividual(Individual *);

// FITNESS FUNCTION
// FIX - FOR TESTING PURPOSES ONLY

#define RUNTIME_WEIGHT 0.6
#define POWER_WEIGHT 1 - RUNTIME_WEIGHT

int encodingToAllele(char * encoding);
// MUTATION OPERATOR

// CROSSOVER OPERATOR

#endif	/* INDIVIDUAL_H */

