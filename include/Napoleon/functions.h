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
 * Filename    : functions.h
 * 
 * Description : This file contains the function declarations in the 
 *      file functions.c, which are referred to in main.c
 *************************************************************************/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

extern int create_graph(t_task *, t_task_interface *, short int *);
extern int create_reuse_mat(t_task *, short int *);

#endif /* FUNCTIONS_H */