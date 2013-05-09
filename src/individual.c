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


Individual * generateRandIndividual(){
    Individual * individual;
    int i;
    int j = 0;
    
    individual = malloc(sizeof(Individual));
    individual->encoding = malloc(sizeof(char) * (template->chrom_length + 1));
    
    for(i=0; i<template->num_genes; i++){
        generateRandGene(individual, i, j);
        j = strlen(individual->encoding);
    }
    
    return individual;
}

// FIX - needs to handle variable numbers of implementations
void generateRandGene(Individual * individual, int gene_num, int chrom_position){
    //template->gene_length[i]);
    int rand_arch_num;
    char * gene;
    int gene_length;
    
    int i = 0, j;
    int length;
    char temp;
    
    gene = &(individual->encoding[chrom_position]);
    gene_length = template->gene_length[template->opr[gene_num]];
    rand_arch_num = (operation[template->opr[gene_num]].num_arch * randomNumber());
    
    while(rand_arch_num != 0){
        gene[i++] = rand_arch_num % 2 + '0';
        rand_arch_num = rand_arch_num / 2;
    }
    gene[i] = '\0';
    
    while(strlen(gene) < gene_length){
        gene[i++] = '0';
        gene[i] = '\0';
    }
    
    length = strlen(gene) - 1;
    for(i=0, j = length; i<j; i++, j--){
        temp = gene[i];
        gene[i] = gene[j];
        gene[j] = temp;
    }
}

void freeIndividual(Individual * i){
    free(i->encoding);
    free(i);
}

int encodingToAllele(char * encoding){
    int sum;
    int length;
    int i;
    
    sum = 0;
    length = strlen(encoding);
    i = 0;
    
    while(i < length){
        sum = sum + pow(2, length - i - 1) * (encoding[i++] - '0');
    }
    
    return sum;
}