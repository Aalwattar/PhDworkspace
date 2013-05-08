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
 * LAST MODIFIED : May 8, 2013
 ******************************************************************************/

#include "problem.h"

/******************************************************************************
 *****************          RANDOM NUMBER GENERATION          *****************
 *****************************************************************************/

void seedRandGenerator(int seed){
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

/******************************************************************************
 *****************              PROBLEM FILE I/O              *****************
 *****************************************************************************/

bool initProblem(char * filename){
    FILE * fp;
    
    if(filename == NULL)
        return false;
    
    fp = fopen(filename, "r");
    if(fp == NULL){
        fprintf(stderr, "Unable to find or open file %s\n", filename);
        return false;
    }
    
    initPerformanceInfo(fp);
    fclose(fp);
    
    return true;
}

// FIX - CREATES MEMORY LEAKS IF THE FILE IS NOT FORMATTED CORRECTLY
bool initPerformanceInfo(FILE * fp){
    char op_type;
    int num_impl;
    int i, j;
    
    performance = malloc(sizeof(Operation) * 4);
    for(i=0; i<4; i++){
        // FIX - find a better way to handle newlines in the file
        fscanf(fp, "\n");
        if(fscanf(fp, "%c %d", &op_type, &num_impl) != 2)
            return false;
        
        (performance[i]).num_arch = num_impl;
        (performance[i]).arch = malloc(sizeof(Architecture) * (performance[i]).num_arch);
        
        for(j=0; j<num_impl; j++){
            if(initArchInfo(fp, &((performance[i]).arch[j])) == false)
                return false;
        }
    }
    
    return true;
}

bool initArchInfo(FILE * fp, Architecture * arch){
    if(fscanf(fp, "%lf %lf %lf", &(arch->runtime), &(arch->power), &(arch->area)) != 3)
        return false;
    
    return true;
}

void freePerformanceInfo(Operation * perf){
    int i;
    
    for(i=0; i<4; i++){
        free((perf[i]).arch);
    }
    
    free(perf);
}

/******************************************************************************
 *****************             TESTING FUNCTIONS              *****************
 *****************************************************************************/

void printProblem(Operation * perf){
    char * names[] = {"Addition", "Subtraction", "Multiplication", "Division"};
    int i, j;
    
    for(i=0; i<4; i++){
        fprintf(stdout, "%s:\n", names[i]);
        for(j=0; j<(perf[i]).num_arch; j++){
            fprintf(stdout, "%.1lf\t%.1lf\t%.1lf\n", ((perf[i]).arch[j]).runtime,
                        ((perf[i]).arch[j]).power, ((perf[i]).arch[j]).area);
        }
    }
}