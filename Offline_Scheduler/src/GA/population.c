/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 7, 2013
 * Modified : May 31, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : population.c
 * 
 * Purpose  : A library for the creation and manipulation of a population
 *              of individuals
 ******************************************************************************/

#include "config.h"
#include "population.h"
#include "fitness.h"
#include "selection.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

// FIX
static double CROSSOVER_RATE = DEFAULT_CROSSOVER_RATE;
static double MUTATION_RATE  = DEFAULT_MUTATION_RATE;

Population * genRandPopulation(int pop_size){
    Population * pop;
    int i;
    
    pop = malloc(sizeof(Population));
    pop->member = malloc(sizeof(Individual) * pop_size);

    for(i=0; i < pop_size; i++)
        initRandIndividual(&(pop->member[i]));
    
    pop->size = pop_size;
    
    return pop;
}

void freePopulation(Population * pop){
    int i;
    
    for(i = 0; i < pop->size; i++)
        freeIndividual(&(pop->member[i]));
    
    free(pop->member);
    free(pop);
}



void determineFitness(Population * pop){
    int i;
    
    for (i = 0; i < pop->size; i++)
        evaluateFitness(&(pop->member[i]));
}


// The compare function for qsort
int compare(const void * p1, const void * p2){
    return ((Individual *)p1)->fitness - ((Individual *)p2)->fitness;
}

void sortByFitness(Population * pop){
    qsort(pop->member, pop->size, sizeof(Individual), compare);
}



void evolvePopulation(Population * pop){
    int i;
    
    for(i=0; i + 1 < pop->size; i = i + 2)
        if(randomNumber() < CROSSOVER_RATE)
                onePointCrossover(&(pop->member[i]), &(pop->member[i + 1]));
    
    for(i=0; i < pop->size; i++)
        mutateRotationally(&(pop->member[i]));
}



void setCrossoverRate(double rate){
    if(0 <= rate && rate <= 1){
        CROSSOVER_RATE = rate;
        return;
    }
    
    fprintf(stderr, "Invalid crossover rate %.3lf.\n", rate);
    fprintf(stderr, "The crossover rate must be a decimal number between 0 and 1\n");
    
    exit(1);
}

double getCrossoverRate(void){
    return CROSSOVER_RATE;
}


void setMutationRate(double rate){
    if(0 <= rate && rate <= 1){
        MUTATION_RATE = rate;
        return;
    }
    
    fprintf(stderr, "Invalid mutation rate %.3lf.\n", rate);
    fprintf(stderr, "The mutation rate must be a decimal number between 0 and 1\n");
    
    exit(1);
}

double getMutationRate(void){
    return MUTATION_RATE;
}


void printPopulation(Population * pop){
    #if (defined VERBOSE || defined DEBUG)
        int i;

        for (i = 0; i < pop->size; i++)
            printIndividual(&(pop->member[i]));
    #endif
    
    printSummaryStatistics(pop);
    
}

void printSummaryStatistics(Population * pop){
    double mean;
    double differenceSum;
    double sd;
    
    int max = 0;
    int min = INT_MAX;
    int fitnessSum;
    int i;
    
    fitnessSum = 0;
    for(i = 0; i < pop->size; i++){
        
        fitnessSum = fitnessSum + pop->member[i].fitness;
        
        if(pop->member[i].fitness > max)
            max = pop->member[i].fitness;
        
        if(pop->member[i].fitness < min)
            min = pop->member[i].fitness;
    }
    
    mean = (double) fitnessSum / pop->size;
    
    differenceSum = 0;
    for(i = 0; i < pop->size; i++)
        differenceSum = differenceSum + pow(mean - pop->member[i].fitness, 2);
    
    sd = sqrt(differenceSum / pop->size);
    
    fprintf(stdout, "\nAverage = %.3lf\n", mean);
    fprintf(stdout, "SD      = %.3lf\n", sd);
    fprintf(stdout, "Min     = %d\n", min);
    fprintf(stdout, "Max     = %d\n", max);
    
//    // concise version
//    fprintf(stdout, "Stats : %.5lf,\t%.5lf,\t%d,\t%d\n", mean, sd, min, max);
}