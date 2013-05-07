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

#include "problem.h"

// stopping conditions

// different combinations of the various sub-algorithms


/* ASSUMPTIONS
 * 
 * - ARG1 = the seed for the random number generator
 */
int main(int argc, char * argv[]){
    
    // FIX - validate each of the arguments
    // FIX - Improve the error message
    if(argc < 2){
        fprintf(stderr, "Improper arguments. \nPlease try again, using the following format : "
                "\n\tGA.exe <rand_seed> \n\nFor more information, please view the README file");
        exit(1);
    }
    
    return 0;
}

/*
 * Individual evolutionaryAlgorithm (Problem 
 */