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
 * FILE NAME : replacement.c
 * 
 * PURPOSE : A library of replacement policies
 ******************************************************************************/

#include "replacement.h"
#include "individual.h"

#include <stdio.h>

void replaceWorst(Population * original, Population * replacements, int num_replaced){
    int pop_size = getPopSize();
    int i, j;
    
    j=0;
    for(i = 0 ; i < num_replaced; i++){
        freeIndividual(&(original->member[pop_size -1 -i]));
        duplicateIndividual(&(original->member[pop_size -1 -i]), &(replacements->member[j++]));
    }
}