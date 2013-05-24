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
 * Filename : fitness.c
 * 
 * Purpose  : A library for integration with Ziad's Napoleon
 * 
 *            Napoleon will act as our fitness function, taking a mapping of 
 *              architectures to each task as input, and returning the runtime
 *              and power required for that schedule as a fitness function
 ******************************************************************************/

#include "fitness.h"

#include <stdlib.h>
#include "types.h"

extern t_task * task;

double evaluateFitness(int * chromosome){
   double fitness;
   double runtime;
   double power;
   
   int oper;
   int allele;
   int i;
   
   fitness = 0.0;
   for(i=0; i<template->num_genes; i++){
       oper = template->oper[i];
       allele = chromosome[i];
       
       runtime = arch_library[oper].impl[allele].exec_t;
       power = arch_library[oper].impl[allele].exec_p;
   
       fitness = fitness + (RUNTIME_WEIGHT)*runtime + (POWER_WEIGHT)*power;
   }
    
    //return Napoleon(grid_strm, succ_adj_mat, task->width, task);
    return fitness;
}



// The compare function for qsort
int compare(const void *, const void *);

void evaluateRanks(Population * pop){
    qsort(pop->member, POP_SIZE, sizeof(Individual), compare);
}

int compare(const void * p1, const void * p2){
    return ((Individual *)p1)->fitness - ((Individual *)p2)->fitness;
}
