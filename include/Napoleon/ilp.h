/*************************************************************************
 * Author   : Ziad Abuowaimer
 * Modifier : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 * 
 * Created  : May 22, 2013
 * Modified : May 22, 2013
 *************************************************************************/

/*************************************************************************
 * Filename    : ilp.h
 * 
 * Description : This file contains the function declarations in the file 
 *      ilp.c, which are referred to in main.c
 *************************************************************************/

#ifndef ILP_H
#define ILP_H

extern void ilp_equations(FILE *, t_task *, short int, short int *, short int *);
short int calc_T(t_task*, short int*);

#endif /* ILP_H */