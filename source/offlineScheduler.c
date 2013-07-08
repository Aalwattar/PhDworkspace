/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 * 
 * Created  : June 27, 2013
 * Modified : July 3, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : offlineScheduler.c
 * 
 * Purpose  : A library of wrapper functions for converting Napoleon into 
 *              an external library
 ******************************************************************************/

//global header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

//local header files"
#include "bounds.h"
#include "types.h"
#include "ecodes.h"
#include "napoleon.h"
#include "io.h"
#include "ilp.h"
#include "functions.h"
#include "main.h"
#include "offlineScheduler.h"
#include "common_interfaces.h"

// FIX - MAKE NON GLOBAL!!!
static t_task_interface * task_interface; 

static int * succ_adj_mat;
static int * reuse_mat;

// FIX - make smaller
// FIX - add error checking
t_task * initNapoleon(char * aif_filename){
    FILE *aif_strm;
    t_task * task;
    int err;
    int i;

    //allocate memory for the tasks and the task interfaces
    task = (t_task*)malloc(sizeof (t_task) * __NUM_MAX_TASKS);
    for(i = 0; i < __NUM_MAX_TASKS; i++){
        (task + i)->type = 0;
        (task + i)->exec_sched = 0;
        (task + i)->reconfig_sched = 0;
        (task + i)->leftmost_column = 0;
        (task + i)->bottommost_row = 0;
        (task + i)->latency = 0;
        (task + i)->columns = 0;
        (task + i)->width = 0;
        (task + i)->input1 = 0;
        (task + i)->input2 = 0;
        (task + i)->output = 0;
        (task + i)->impl = 0;
        (task + i)->reconfig_pwr = 0;
        (task + i)->exec_pwr = 0;
    }

    task_interface = (t_task_interface*)malloc(sizeof (t_task_interface) * __NUM_MAX_TASK_INTFC);
    for(i = 0; i < __NUM_MAX_TASK_INTFC; i++){
        (task_interface + i)->mode = 0;
        (task_interface + i)->width = 0;
        (task_interface + i)->reg_out = 0;
    }

    err = parse_aif(aif_strm, task, task_interface);

    if(err != __NO_ERROR){
        print_error(err);    //and exit on unsuccessful execution of the parse_aif function
        free(task);
        free(task_interface);
        return NULL;
    }

    //allocate memory for the successor graph adjacency matrix
    succ_adj_mat = (int*)malloc(sizeof (int)*(task->width + 2)*(task->width + 2));

    //create the successor matrix
    if((err = create_graph(task, task_interface, succ_adj_mat))){
        print_error(err);
        free(task);
        free(task_interface);
        free(succ_adj_mat);
        return NULL;
    }

    //allocate memory for the reuse matrix
    reuse_mat = (int*)malloc(sizeof (int)*(task->width + 2)*(task->width + 2));

    //create the reuse matrix
    if((err = create_reuse_mat(task, reuse_mat))){
        print_error(err);
        free(task);
        free(task_interface);
        free(succ_adj_mat);
        free(reuse_mat);
        return NULL;
    }
    
    return task;
}

GA_Info getSchedule(t_task * task){
    return ( Napoleon(NULL, succ_adj_mat, task->width, task) );
}


void freeNapoleon(void){
    free(reuse_mat);
    free(succ_adj_mat);
    
    free(task_interface);
}