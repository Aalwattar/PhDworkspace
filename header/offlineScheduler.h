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

#ifndef RCSSIMULATOR_H_
#define RCSSIMULATOR_H_

struct SimData {
    int dFGID;
    int noPRR;
    int noGPP;
    int noOfNodes;
    int typeData[500];
};


struct SimResults {
    int totalTime;
    int noOfReuse;
    int noOfConfiguration;
    int noSW2HWMigration;
    int noHW2SWMigration;
    int noOfSWTasks;
    int noSWBusyCounter;
    int noHWBusyCounter;
    int power;
};

#endif // RCSSIMULATOR_H_

// DEPRECIATED FIX - remove if time
typedef struct{
    int power;    // the power required to execute the schedule
    int runtime;  // the total runtime of the schedule
    int prefetch;
    int reuse;
}GA_Info;   

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
int initNapoleon(char * arch_lib_filename, char * dfg_filename);

/******************************************************************************
 * NAME : freeNapoleon
 * 
 * PURPOSE : free all of Napoleon's static data structures
 * PRECONDITION : This function should only be called if initNapoleon() 
 *                  returned true.
 *****************************************************************************/
void freeNapoleon(void);


// FIX - needs a comment
void getSchedule(struct SimData *, struct SimResults *);

#endif /* OFFLINE_SCHEDULER_H */
