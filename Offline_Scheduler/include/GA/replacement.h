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
 * Filename : replacement.h
 * 
 * Purpose  : a header for replacement.c
 ******************************************************************************/


#ifndef REPLACEMENT_H
#define	REPLACEMENT_H

#include "population.h"

void replaceWorst(Population *, Population *, int);


#endif	/* REPLACEMENT_H */

