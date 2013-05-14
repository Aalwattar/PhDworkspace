/*******************************************************************************
 * FILE NAME : population.c
 * 
 * Genetic Algorithm practice for Ahmed Al-Watter
 * 
 * PURPOSE : library for the representation and manipulation of a population
 *       of individuals
 * 
 * AUTHOR : Jennifer Winer
 * Email  : jwiner@uoguelph.ca
 * 
 * DATE CREATED : May 7, 2013
 * LAST MODIFIED : May 13, 2013
 ******************************************************************************/

#include "population.h"

int POP_SIZE = 50;

Population * genRandPopulation(){
    Population * pop;
    int min_fitness;
    int i;

    pop = malloc(sizeof(Population));
    pop->member = malloc(sizeof(Individual) * POP_SIZE);

    pop->total_fitness = 0;
    pop->best_individual = -1;
    min_fitness = 1000000;       // FIX - Set to something larger then possible

    for(i=0; i<POP_SIZE; i++){
        initRandIndividual(&(pop->member[i]));
        evaluateFitness(&(pop->member[i]));
        pop->total_fitness = pop->total_fitness + pop->member[i].fitness;
        
        if(pop->member[i].fitness < min_fitness){
            pop->best_individual = i;
        }
    }
    return pop;
}

void freePopulation(Population * pop){
    int i;
    
    for(i=0; i<POP_SIZE; i++){
        freeIndividual(&(pop->member[i]));
    }
    
    free(pop->member);
    free(pop);
}

// tournament selection of size 2
Population * selectMatingPool(Population * original){
    Population * mating_pool;
    int p1, p2;
    int i;
    
    mating_pool = malloc(sizeof(Population));
    mating_pool->member = malloc(sizeof(Individual) * POP_SIZE);
    
    // FIX - do I need to ensure that the two chosen for comparison are different?
    for(i=0; i<POP_SIZE; i++){
        p1 = randomNumber() * POP_SIZE;
        p2 = randomNumber() * POP_SIZE;
        
        if(original->member[p1].fitness <= original->member[p2].fitness){
            duplicateIndividual(&(mating_pool->member[i]), &(original->member[p1]));
        }
        else{
            duplicateIndividual(&(mating_pool->member[i]), &(original->member[p2]));
        }
    }
    return mating_pool;
}

// a Generational algorithm
void generateNextGeneration(Population * pop){
    int i;
    
    for(i=0; i<POP_SIZE; i = i + 2){
        if(randomNumber() < CROSSOVER_RATE){
                crossover(&(pop->member[i]), &(pop->member[i+1]));
        }
    }
    
    for(i=0; i<POP_SIZE; i++){
        mutate(&(pop->member[i]));
        evaluateFitness(&(pop->member[i]));
    }
}