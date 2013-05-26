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
 * Filename    : napoleon.h
 * 
 * Description : This file contains the function declarations in the file 
 *      napoleon.c, which are referred to in main.c
 *************************************************************************/

#ifndef NAPOLEON_H
#define NAPOLEON_H

#include <stdio.h>

extern short int Napoleon(FILE *, short int *, short int, t_task *);

#endif /* NAPOLEON_H */