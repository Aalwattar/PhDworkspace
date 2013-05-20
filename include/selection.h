/*******************************************************************************
 * FILE NAME : selection.h
 * 
 * Genetic Algorithm for Ahmed Al-Watter
 * 
 * PURPOSE : a header for selection.c
 * 
 * AUTHOR : Jennifer Winer
 * Email  : jwiner@uoguelph.ca
 * 
 * DATE CREATED : May 16, 2013
 * LAST MODIFIED : May 16, 2013
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

