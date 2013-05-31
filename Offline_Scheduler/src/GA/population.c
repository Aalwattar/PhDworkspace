/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 7, 2013
 * Modified : May 30, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : population.c
 * 
 * Purpose  : A library for the creation and manipulation of a population
 *              of individuals
 ******************************************************************************/

#include "population.h"
#include "fitness.h"

#include <stdlib.h>
#include <stdio.h>

static double CROSSOVER_RATE = 0.85;
static double MUTATION_RATE  = 0.001;

static int POP_SIZE = 16;


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
    int i, j;
    
    for (i = 0; i < POP_SIZE; i++) {
        for (j = 0; j < getNumGenes(); j++) {
            fprintf(stdout, "%d", pop->member[i].encoding[j]);
        }
        fprintf(stdout, "\tfitness = %d\n", pop->member[i].fitness);
    }
}


void determineFitness(Population * pop){
    int i;
    
    for (i = 0; i < POP_SIZE; i++) {
        pop->member[i].fitness = evaluateFitness(pop->member[i].encoding);
    }
}


// a Generational algorithm
void generateNextGeneration(Population * pop){
    int i;
    
    for(i=0; i + 1 <POP_SIZE; i = i + 2)
        if(randomNumber() < CROSSOVER_RATE)
                crossover(&(pop->member[i]), &(pop->member[i + 1]));
    
    for(i=0; i<POP_SIZE; i++)
        mutate(&(pop->member[i]));
}


// FIX - ensure that the crossover rate is between 0 and 1 
void setCrossoverRate(char * raw_rate){
    CROSSOVER_RATE = atof(raw_rate);
}

double getCrossoverRate(void){
    return CROSSOVER_RATE;
}

// FIX - ensure that the mutation rate is between 0 and 1 
void setMutationRate(char * raw_rate){
    MUTATION_RATE = atof(raw_rate);
}

double getMutationRate(void){
    return MUTATION_RATE;
}

// FIX - ensure that the population size is between 1 and 10000
void setPopSize(char * raw_size){
    POP_SIZE = atoi(raw_size);
}

int getPopSize(void){
    return POP_SIZE;
}