/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 16, 2013
 * Modified : June 6, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : selection.h
 * 
 * Purpose  : a header for selection.c
 ******************************************************************************/


#ifndef SELECTION_H
#define	SELECTION_H

#include "population.h"

/******************************************************************************
 * NAME : tournamentSelection
 * 
 * PURPOSE : Select individuals from a population based on the the hard
 *              tournament selection algorithm
 * ARGUMENTS : Population * = The population that you wish to select from 
 *             int = the number of individuals that you wish to select
 * 
 * RETURNS : A Population that contains deep copies of all of the winners from 
 *              the requested number of tournaments. 
 *****************************************************************************/
Population * tournamentSelection(Population *);


/******************************************************************************
 * NAME : randomSelection
 * 
 * PURPOSE : Randomly selects individuals from a population.
 * ARGUMENTS : Population * = The population that you wish to select from 
 *             int = the number of individuals that you wish to select
 * 
 * RETURNS : A Population that contains deep copies of all of the winners from 
 *              the requested number of tournaments. 
 *****************************************************************************/
Population * randomSelection(Population *);


/******************************************************************************
 * NAME : findBest
 * 
 * PURPOSE : Finds the individual with the best fitness
 * ARGUMENTS : Population * = the population that you wish to search
 * 
 * RETURNS : a pointer to the individual with the best fitness
 *****************************************************************************/
Individual * findBest(Population *);


#endif	/* SELECTION_H */

