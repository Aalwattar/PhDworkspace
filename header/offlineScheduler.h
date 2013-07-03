/*************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 * 
 * Created  : June 27, 2013
 * Modified : June 27, 2013
 *************************************************************************/

/*************************************************************************
 * Filename    : offlineScheduler.h
 * 
 * Description : This file contains the function declarations in the file 
 *               offlineScheduler.c
 *************************************************************************/

#ifndef OFFLINE_SCHEDULER_H
#define OFFLINE_SCHEDULER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "fitness.h"
#include "ecodes.h"
#include "functions.h"
#include "napoleon.h"
#include "io.h" 
#include "types.h"


/******************************************************************************
 * NAME : initNapoleon
 * 
 * PURPOSE : Create and initialize all of Napoleon's static data structures
 * ARGUMENTS : char * = the name of the file that contains the DFG that you
 *                        wish to schedule (please see Ahmed Al-Wattar for 
 *                        more information
 * 
 * RETURNS : true if Napoleon was successfully initialized
 *           false if anything has gone wrong (error message printed to stderr)
 *****************************************************************************/
t_task * initNapoleon(char *);

/******************************************************************************
 * NAME : freeNapoleon
 * 
 * PURPOSE : free all of Napoleon's static data structures
 * PRECONDITION : This function should only be called if initNapoleon() 
 *                  returned true.
 *****************************************************************************/
void freeNapoleon(void);


// FIX
GA_Info getSchedule(t_task * task);

#endif /* OFFLINE_SCHEDULER_H */