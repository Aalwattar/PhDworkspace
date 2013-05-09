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
    
    individual->fitness = 0;
    individual->cfitness = 0;
    individual->rfitness = 0;
    
    return individual;
}

// FIX - needs to handle variable numbers of implementations
void generateRandGene(Individual * individual, int gene_num, int chrom_position){
    int rand_arch_num;
    char * gene;
    
    gene = &(individual->encoding[chrom_position]);
    rand_arch_num = (operation[template->opr[gene_num]].num_arch * randomNumber());
    
    alleleToEncoding(rand_arch_num, gene_num, gene);
}

void freeIndividual(Individual * i){
    free(i->encoding);
    free(i);
}



void calculateFitness(Individual * individual){
    double fitness;
    char allele[MAX_GENE_SIZE];
    int i, j;
    
    j = 0;
    fitness = 0;
    for(i=0; i<template->num_genes; i++){
        strncpy(allele, &(individual->encoding[j]), template->gene_length[i]);
        allele[template->gene_length[i]] = '\0';
        j = j + template->gene_length[i];
        
        fitness = fitness + evaluateGene(allele, template->opr[i]);
    }
    
    individual->fitness = fitness;
}

double evaluateGene(char * gene, int oper){
    int allele;
    double runtime;
    double power;
    
    allele = encodingToAllele(gene);
    runtime = operation[oper].arch[allele].runtime;
    power = operation[oper].arch[allele].power;
    
    return (RUNTIME_WEIGHT)*runtime + (POWER_WEIGHT)*power;
}



int encodingToAllele(char * encoding){
    int sum;
    int length;
    int i = 0;
    
    sum = 0;
    length = strlen(encoding);
    
    while(i < length){
        sum = sum + pow(2, length - i - 1) * (encoding[i++] - '0');
    }
    
    return sum;
}

void alleleToEncoding(int allele, int gene_num, char * gene){
    int gene_length;
    int i = 0;
    int j;
    char temp;
    
    gene_length = template->gene_length[gene_num];
    
    while(allele != 0){
        gene[i++] = allele % 2 + '0';
        allele = allele / 2;
    }
    gene[i] = '\0';
    
    while(strlen(gene) < gene_length){
        gene[i++] = '0';
        gene[i] = '\0';
    }
    
    gene_length = strlen(gene) - 1;
    for(i=0, j = gene_length; i<j; i++, j--){
        temp = gene[i];
        gene[i] = gene[j];
        gene[j] = temp;
    }
}