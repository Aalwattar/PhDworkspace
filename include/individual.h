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

#ifndef INDIVIDUAL_H
#define	INDIVIDUAL_H


// REPRESENTATION OF THE PROBLEM
typedef struct{
    char * gene;
    int numGenes;
    int * geneLength;
    
    double fitness;
    double rfitness;
    double cfitness;
} Individual;

// FITNESS FUNCTION

// MUTATION OPERATOR

// CROSSOVER OPERATOR

#endif	/* INDIVIDUAL_H */

