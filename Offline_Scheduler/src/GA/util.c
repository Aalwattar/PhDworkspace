/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 7, 2013
 * Modified : May 29, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : problem.c
 * 
 * Purpose  : library for utility functions such as random number generation
 ******************************************************************************/

#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


/******************************************************************************
 *****************          RANDOM NUMBER GENERATION          *****************
 *****************************************************************************/

void seedRandGenerator(int seed) {
    srand(seed);
}

int randSeed(void) {
    return time(NULL);
}

double randomNumber(void) {
    return rand() / (double) RAND_MAX;
}
