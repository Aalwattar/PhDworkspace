/*******************************************************************************
 * FILE NAME : problem.c
 * 
 * Genetic Algorithm practice for Ahmed Al-Watter
 * 
 * PURPOSE : library for the representation and manipulation of a problem
 *      instance
 * 
 * AUTHOR : Jennifer Winer
 * Email  : jwiner@uoguelph.ca
 * 
 * DATE CREATED : May 7, 2013
 * LAST MODIFIED : May 7, 2013
 ******************************************************************************/

#include "problem.h"

void initRandGenerator(int seed){
    srand(seed);
}

void randSeed(void){
    int seed;
    
    seed = time(NULL);
    fprintf(stdout, "Seed: %d\n", seed);
    
    srand(seed);
}

double randomNumber(void){
    return rand() / (double) RAND_MAX;
}

void initPerformanceInfo(){
    FILE * fp;
    char op_Type;
    int num_arch;
    int i, j;
    
    fp = fopen(arch_filename, "r");
    if(fp == NULL){
        fprintf(stderr, "Unable to find or open file %s\n", arch_filename);
        exit(1);
    }
    
    performance = malloc(sizeof(Architecture *) * 4);    
    for(i=0; i<4; i++){
        fscanf(fp, "%c %d", op_Type, num_arch);
        performance[i] = malloc(sizeof(Architecture) * num_arch + 1);
        
        for(j=0; j<num_arch; j++){
            
        }
    }
}

void freePerformanceInfo(){
    
}