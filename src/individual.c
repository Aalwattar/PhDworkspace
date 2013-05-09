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
 * LAST MODIFIED : May 8, 2013
 ******************************************************************************/

/* NOTES
 * - each gene will correspond to one task (eg. addition, multiplication, etc.)
 * - each allele will correspond to a specific hardware implementation of that task
 *      - the allele will be an integer = the row number of a table provided
 */

/* PRECONDITIONS  
 * 
 * - the file that contains the table of values (areas, times, and power) has been
 *      previously initialized and set in the #DEFINE statements
 */

#include "individual.h"


Individual * initRandomIndividual(){
    Individual * individual;
    int i;
    int j = 0;
    
    individual = malloc(sizeof(Individual));
    individual->chrom = malloc(sizeof(char) * (template->chrom_length) + 1);
    
    for(i=0; i<template->num_genes; i++){
        generateRandGene(template->opr[i], &(individual->chrom[j]));
        j = strlen(individual->chrom);
    }
    
    return individual;
}

// FIX - needs to handle variable numbers of implementations
void generateRandGene(int opr, char * allele){
    int rand_arch;
    int i = 0, j;
    int length;
    char temp;
    
    rand_arch = (operation[opr].num_arch * randomNumber());
    
    while(rand_arch != 0){
        allele[i++] = rand_arch % 2 + '0';
        rand_arch = rand_arch / 2;
    }
    allele[i] = '\0';
    
    length = strlen(allele) - 1;
    for(i=0, j = length; i<j; i++, j--){
        temp = allele[i];
        allele[i] = allele[j];
        allele[j] = temp;
    }
}
/*
 * eg. 001 10 00 000 11 111
 * 
 * # of genes
 *      = the number of nodes in the task graph
 * 
 * # of bits of each gene
 *      = number of architectures for that gene's operation
 * 
 */

        
        