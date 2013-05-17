/*******************************************************************************
 * FILE NAME : population.h
 * 
 * Genetic Algorithm for Ahmed Al-Watter
 * 
 * PURPOSE : a header for population.c
 * 
 * AUTHOR : Jennifer Winer
 * Email  : jwiner@uoguelph.ca
 * 
 * DATE CREATED : May 7, 2013
 * LAST MODIFIED : May 17, 2013
 ******************************************************************************/

#include "individual.h"

#ifndef POPULATION_H
#define	POPULATION_H

//#define POP_SIZE 50      // MUST BE AN EVEN NUMBER
extern int POP_SIZE;

/******************************************************************************
 *****************         POPULATION REPRESENTATION         ******************
 *****************************************************************************/
typedef struct{
    Individual * member;
    double total_fitness;
}Population;



/******************************************************************************
 ******************         POPULATION MANIPULATION         *******************
 *****************************************************************************/

/******************************************************************************
 * NAME : genRandPopulation
 * 
 * PURPOSE : Creates a population of size POP_SIZE. Each individual will
 *              contain randomly generated genes, but is still a valid solution
 * RETURNS : A population of randomly generated individuals
 * 
 * NOTE : the population that is returned should eventually be freed through
 *              the usage of freePopulation();
 *****************************************************************************/
Population * genRandPopulation(void);

/******************************************************************************
 * NAME : freePopulation
 * 
 * PURPOSE : Frees all dynamically allocated data from within an Population
 * ARGUMENTS : Population * = the population that you wish to free
 * 
 * PRECONDITIONS : the Population passed in as a parameter must have been
 *      previously created by genRandPopulation();
 *****************************************************************************/
void freePopulation(Population *);



/******************************************************************************
 * NAME : selectMatingPool
 * 
 * PURPOSE : Select the individuals that will undergo recombination and 
 *              mutation to create the next generation
 * ARGUMENTS : Population * = the population that we are selecting individuals
 *              from
 * 
 * PRECONDITIONS : the Population passed in as a parameter must have been
 *      previously created by genRandPopulation();
 * RETURNS : a new population that consists of individuals chosen to mate
 *              (in a random order)
 *           the same Individual may appear more than once in the mating 
 *              population
 *****************************************************************************/
Population * tournamentSelection(Population *); //

Population * rouletteWheelSelection(Population *); //

Population * linearRankingSelection(Population *);

//Population * linearScalingSelection(Population *); //

Population * sigmaScalingSelection(Population *);

Population * powerScalingSelection(Population *);

Population * softTournamentSelection(Population *);

// implement a form of elitism?
        // this can mean only reproducing with 2 individuals at a time
// Boltzmann selection
// steady state algorithms
        // replace worst instead of parents???


/******************************************************************************
 * NAME : generateNextGeneration
 * 
 * PURPOSE : Perform recombination and mutation on the mating pool
 * ARGUMENTS : Population * = the population that serves as the mating pool
 * 
 * PRECONDITIONS : the Population passed in as a parameter must have been
 *      previously created by genRandPopulation();
 * RETURNS : a new population that consists of individuals chosen to mate
 *              (in a random order)
 *           the same Individual may appear more than once in the mating 
 *              population
 *****************************************************************************/
void generateNextGeneration(Population *);

// REPLACEMENT ALGORITHM - generational, steady state?, replace worst?
        // FIX - I AM ASSUMING THAT THE ENTIRE GENERATION IS REPLACED FOR NOW

// FIND THE BEST INDIVIDUAL - when the algorithm ends

// TERMINATION CONDITION

#endif	/* POPULATION_H */

