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
#include "problem.h"

// stopping conditions

// different combinations of the various sub-algorithms


int main(int argc, char * argv[]){
    Individual * organism;
    int i;
    
    initProblem(ARCH_FILENAME, DFG_FILENAME);

    for(i=0; i<50; i++){
        organism = generateRandIndividual();
        printf("%s\n", organism->chrom);
        freeIndividual(organism);
    }
    
    
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
 *****************************************************************************/


    // FIX - validate each of the arguments
    // FIX - Improve the error message
//    if(argc < 2){
//        fprintf(stderr, "Improper arguments. \nPlease try again, using the following format : "
//                "\n\tGA.exe <rand_seed> \n\nFor more information, please view the README file");
//        exit(1);
//    }