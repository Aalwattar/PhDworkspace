/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 16, 2013
 * Modified : May 22, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : selection.h
 * 
 * Purpose  : a header for selection.c
 ******************************************************************************/


#include "population.h"

#ifndef SELECTION_H
#define	SELECTION_H


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


#endif	/* SELECTION_H */

