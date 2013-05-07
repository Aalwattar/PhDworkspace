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


// Random number generator