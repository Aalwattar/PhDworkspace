/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 16, 2013
 * Modified : May 26, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : selection.h
 * 
 * Purpose  : a header for selection.c
 ******************************************************************************/


#ifndef SELECTION_H
#define	SELECTION_H

#include "population.h"

Population * tournamentSelection(Population * original);

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


///******************************************************************************
// * NAME : evaluateRanks
// * 
// * PURPOSE : Orders the individuals in a population in ascending order
// *              based on fitness
// * ARGUMENTS : Population * = the Population that you wish to evaluate
// *****************************************************************************/
//void evaluateRanks(Population *);


#endif	/* SELECTION_H */

