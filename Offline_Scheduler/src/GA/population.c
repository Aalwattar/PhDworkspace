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

#include "population.h"
#include "fitness.h"
#include "selection.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static double CROSSOVER_RATE = 0.85;
static double MUTATION_RATE  = 0.005;

static int POP_SIZE = 50;


Population * genRandPopulation(){
    Population * pop;
    int i;

    pop = malloc(sizeof(Population));
    pop->member = malloc(sizeof(Individual) * POP_SIZE);
    pop->total_fitness = 0;

    for(i=0; i < POP_SIZE; i++)
        initRandIndividual(&(pop->member[i]));
    
    return pop;
}

void freePopulation(Population * pop){
    int i;
    
    for(i=0; i<POP_SIZE; i++)
        freeIndividual(&(pop->member[i]));
    
    free(pop->member);
    free(pop);
}



void printPopulation(Population * pop){
    int i;
    
    for (i = 0; i < POP_SIZE; i++)
        printIndividual(&(pop->member[i]));
    
    printSummaryStatistics(pop);
}

void printSummaryStatistics(Population * pop){
    double mean;
    double differenceSum;
    double sd;
    
    int max = 0;
    int min = 1000000;
    int fitnessSum;
    int i;
    
    fitnessSum = 0;
    for(i=0; i<POP_SIZE; i++){
        
        fitnessSum = fitnessSum + pop->member[i].fitness;
        
        if(pop->member[i].fitness > max)
            max = pop->member[i].fitness;
        
        if(pop->member[i].fitness < min)
            min = pop->member[i].fitness;
    }
    
    mean = (double) fitnessSum / POP_SIZE;
    
    differenceSum = 0;
    for(i=0; i<POP_SIZE; i++)
        differenceSum = differenceSum + pow(mean - pop->member[i].fitness, 2);
    
    sd = sqrt(differenceSum / POP_SIZE);
    
    
    fprintf(stdout, "\nAverage = %.3lf\n", mean);
    fprintf(stdout, "SD      = %.3lf\n", sd);
    fprintf(stdout, "Min     = %d\n", min);
    fprintf(stdout, "Max     = %d\n", max);
}


void determineFitness(Population * pop){
    int i;
    
    for (i = 0; i < POP_SIZE; i++)
        evaluateFitness(&(pop->member[i]));
}


// a Generational algorithm
void generateNextGeneration(Population * pop, int pop_size){
    int i;
    
    for(i=0; i + 1 <pop_size; i = i + 2)
        if(randomNumber() < CROSSOVER_RATE)
                crossover(&(pop->member[i]), &(pop->member[i + 1]));
    
    for(i=0; i<pop_size; i++)
        mutate(&(pop->member[i]));
}


// FIX - ensure that the crossover rate is between 0 and 1 
void setCrossoverRate(double rate){
    CROSSOVER_RATE = rate;
}

double getCrossoverRate(void){
    return CROSSOVER_RATE;
}

// FIX - ensure that the mutation rate is between 0 and 1 
void setMutationRate(double rate){
    MUTATION_RATE = rate;
}

double getMutationRate(void){
    return MUTATION_RATE;
}

// FIX - ensure that the population size is between 1 and 10000
void setPopSize(int size){
    POP_SIZE = size;
}

int getPopSize(void){
    return POP_SIZE;
}