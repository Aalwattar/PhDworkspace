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
 * LAST MODIFIED : May 13, 2013
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "population.h"

// stopping conditions
//#define STOP_CONDITION 500


extern char * ARCH_FILENAME;
extern char * DFG_FILENAME;

extern double CROSSOVER_RATE;
extern double MUTATION_RATE;
extern int POP_SIZE;

int STOP_CONDITION = 500;
int generation_num;


// IDEAL SOLUTION:
//      - all 0's
//      - fitness = 8379

void initParameters(int num_tokens, char ** input_token);
bool populationConverged(Population * pop){
    int i, j;
    
    for(i=0; i<POP_SIZE; i++){
        for(j=0; j<template->num_genes; j++){
            if(pop->member[i].encoding[j] != 0){
                break;
            }
        }
        if(j == template->num_genes){
            return true;
        }
    }
    return false;
}

int main(int argc, char * argv[]){
    Population * pop;
    Population * mating_pop;
    int i, j;
    
    
    initParameters(argc, argv);
    initProblem();
    
    pop = genRandPopulation();
    
    fprintf(stdout, "\n----------------------------------------------------------\n");
    fprintf(stdout, "Starting Population:\n");
    for(i=0; i<POP_SIZE; i++){
        for(j=0; j<template->num_genes; j++){
            fprintf(stdout, "%d", pop->member[i].encoding[j]);
        }
        fprintf(stdout, "\n");
    }
    
    //while(generation_num < STOP_CONDITION){
    while(!populationConverged(pop)){
        mating_pop = selectMatingPool(pop);
        freePopulation(pop);
        
        generateNextGeneration(mating_pop);
        pop = mating_pop;
        
        generation_num++;
    }
    
    fprintf(stdout, "\nGenerations to create best solution = %d\n", generation_num);
//    fprintf(stdout, "\nFinal Population:\n");
//    for(i=0; i<POP_SIZE; i++){
//        for(j=0; j<template->num_genes; j++){
//            fprintf(stdout, "%d", pop->member[i].encoding[j]);
//        }
//        fprintf(stdout, "\n");
//    }
    
    freePopulation(pop);
    freeProblem();
    return 0;
}

void initParameters(int num_tokens, char ** input_token){
    int i;
    
//    5. selection method
//    6. crossover type
//    7. mutation type
    
    // FIX - make more robust
    for(i=0; i<num_tokens; i++){
        if(strncmp(input_token[i], "-ps=", 4) == 0){
            POP_SIZE = atoi(&(input_token[i][4]));
        }
        if(strncmp(input_token[i], "-mr=", 4) == 0){
            MUTATION_RATE = atof(&(input_token[i][4]));
        }
        if(strncmp(input_token[i], "-cr=", 4) == 0){
            CROSSOVER_RATE = atof(&(input_token[i][4]));
        }
        if(strncmp(input_token[i], "-g=", 3) == 0){
            STOP_CONDITION = atoi(&(input_token[i][3]));
        }
        if(strncmp(input_token[i], "-arch=", 6) == 0){
            ARCH_FILENAME = &(input_token[i][6]);
        }
        if(strncmp(input_token[i], "-dfg=", 5) == 0){
            DFG_FILENAME = &(input_token[i][5]);
        }
        
        if(strncmp(input_token[i], "-seed=", 6) == 0){
            DFG_FILENAME = &(input_token[i][6]);
        }
    }
    
    fprintf(stdout, "Parameters:\n");
    fprintf(stdout, "\tPopulation Size       = %d\n", POP_SIZE);
    fprintf(stdout, "\tNumber of Generations = %d\n\n", STOP_CONDITION);
    fprintf(stdout, "\tMutation Rate  = %.4lf\n", MUTATION_RATE);
    fprintf(stdout, "\tCrossover Rate = %.4lf\n\n\t", CROSSOVER_RATE);
}


// COMMENTING TEMPLATE

/******************************************************************************
 * NAME : initRandGenerator
 * PURPOSE : 
 * ARGUMENTS : 
 * PRECONDITIONS : 
 * RETURNS : 
 * NOTE : 
 *****************************************************************************/


    // FIX - validate each of the arguments
    // FIX - Improve the error message
//    if(argc < 2){
//        fprintf(stderr, "Improper arguments. \nPlease try again, using the following format : "
//                "\n\tGA.exe <rand_seed> \n\nFor more information, please view the README file");
//        exit(1);
//    }


//    5. selection method
//    6. crossover type
//    7. mutation type