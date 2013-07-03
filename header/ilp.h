/*************************************************************************
 * Author   : Ziad Abuowaimer
 * Modifier : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 * 
 * Created  : May 22, 2013
 * Modified : July 3, 2013
 *************************************************************************/

/*************************************************************************
 * Filename    : ilp.h
 * 
 * Description : This file contains the function declarations in the file 
 *      ilp.c, which are referred to in main.c
 *************************************************************************/

#ifndef ILP_H
#define ILP_H

#include <stdio.h>

#include "types.h"

extern void ilp_equations(FILE *, t_task *, int, int *, int *);
int calc_T(t_task *, int *);

#endif /* ILP_H */