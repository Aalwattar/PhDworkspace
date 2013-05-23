/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 7, 2013
 * Modified : May 22, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : problem.c
 * 
 * Purpose  : library for the representation and manipulation of a problem
 *              instance
 * 
 * FIX - COMBINE THIS FILE WITH PROBLEM.C????
 *     - MOST OF THIS FILE WAS A TESTING HARNESS, TRIM THE EXCESS
 ******************************************************************************/

#include "problem.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>


#define BUFF_SIZE 500

extern char * ARCH_FILENAME;
extern char * DFG_FILENAME;

//bool initProblem(void) {
//    int i;
//
//    // FIX - make option to enter your own seed
//    randSeed();
//    //seedRandGenerator(1368463890);
//
//    initArchLibrary(ARCH_FILENAME);
//    // FIX - Integrate with Napoleon here
//    initDFG(DFG_FILENAME);
//
//    template = malloc(sizeof (Representation));
//    template->num_genes = graph->num_nodes;
//    template->oper = malloc(sizeof (int) * template->num_genes);
//
//    for (i = 0; i < (template->num_genes); i++) {
//        template->oper[i] = graph->oper[i];
//    }
//
//    freeDFG();
//    return true;
//}
//
//void freeProblem(void) {
//    freeArchLibrary();
//
//    free(template->oper);
//    free(template);
//}



/******************************************************************************
 *****************            ARCHITECTURE FILE I/O           *****************
 *****************************************************************************/

bool parseArchLibrary(FILE * fp);

bool initArchLibrary(char * filename) {
    FILE * fp;
    bool status = true;

    if (filename == NULL)
        return false;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Unable to find or open file %s\n", filename);
        return false;
    }

    if (parseArchLibrary(fp) == false)
        status = false;

    fclose(fp);

    return status;
}

bool parseArchLibrary(FILE * fp){
    
}


/******************************************************************************
 ***********************           DFG FILE I/O         ***********************
 *****************************************************************************/


//t_task_interface *task_interface;
//t_task *task;
//t_task_type *task_type;
//t_config config;
//int err = 0;
//short int *succ_adj_mat;
//short int *reuse_mat; //aij
//short int T = 99; //upper_bound_total_exec_time
//
//bool initDFG(char * filename){
//   FILE *aif_strm, *res_strm, *ilp_strm, *grid_strm;
//   int i = 0;
//
//   //allocate memory for the tasks and the task interfaces
//   task = (t_task*) malloc(sizeof (t_task) * __NUM_MAX_TASKS);
//   for (i = 0; i < __NUM_MAX_TASKS; i++) {
//       (task + i)->type = 0;
//       (task + i)->exec_sched = 0;
//       (task + i)->reconfig_sched = 0;
//       (task + i)->leftmost_column = 0;
//       (task + i)->bottommost_row = 0;
//       (task + i)->latency = 0;
//       (task + i)->columns = 0;
//       (task + i)->width = 0;
//       (task + i)->input1 = 0;
//       (task + i)->input2 = 0;
//       (task + i)->output = 0;
//   }
//
//   task_interface = (t_task_interface*) malloc(sizeof (t_task_interface) * __NUM_MAX_TASK_INTFC);
//   for (i = 0; i < __NUM_MAX_TASK_INTFC; i++) {
//       (task_interface + i)->mode = 0;
//       (task_interface + i)->width = 0;
//       (task_interface + i)->reg_out = 0;
//   }
//
//   //open the aif input file for reading
//   if ((aif_strm = fopen(config.aif_fname, "r"))) {
//       //parse the aif file
//       //and exit on unsuccessful execution of the parse_aif function
//       if ((err = parse_aif(aif_strm, task, task_interface)))
//           print_error(err);
//       //fname = strtok(config.aif_fname, ".");
//       fclose(aif_strm);
//   }
//   //assert(aif_strm);
//
//   //allocate memory for resources
//   task_type = (t_task_type*) malloc(sizeof (t_task_type) * __NUM_MAX_TASK_TYPES);
//   for (i = 0; i < __NUM_MAX_TASK_TYPES; i++) {
//       (task_type + i)->latency = 0;
//       (task_type + i)->reconfig_time = 0;
//       (task_type + i)->columns = 0;
//       (task_type + i)->rows = 0;
//   }
//
//   //open the resource file for reading
//   if ((res_strm = fopen(config.res_fname, "r"))) {
//       //parse the resource file
//       //and exit on unsuccessful execution of the parse_res function
//       if ((err = parse_res(res_strm, task_type)))
//           print_error(err);
//       fclose(res_strm);
//   }
//   //assert(res_strm);
//
//#ifdef __DEBUG
//   //display the task types info
//   display_task_type(task_type);
//#endif
//
//   set_task_parameter(task, task_type);
//
//   //allocate memory for the successor graph adjacency matrix
//   succ_adj_mat = (short int*) malloc(sizeof (short int)*(task->width + 2)*(task->width + 2));
//
//   //create the successor matrix
//   //and exit on unsuccessful execution of the parse_aif function
//   if ((err = create_graph(task, task_interface, succ_adj_mat)))
//       print_error(err);
//
//   //allocate memory for the reuse matrix
//   reuse_mat = (short int*) malloc(sizeof (short int)*(task->width + 2)*(task->width + 2));
//
//   //create the reuse matrix
//   if ((err = create_reuse_mat(task, reuse_mat)))
//       print_error(err);
//
//   fprintf(stderr, "T = %d\n", calc_T(task, &T));
//   
//   return true;
//}
//
//void freeDFG(void){
//   free(reuse_mat);
//   free(succ_adj_mat);
//   free(task_type);    
//   free(task_interface);
//   free(task);
//}

/******************************************************************************
 *****************          RANDOM NUMBER GENERATION          *****************
 *****************************************************************************/

void seedRandGenerator(int seed) {
    srand(seed);
}

void randSeed(void) {
    int seed;

    seed = time(NULL);
    fprintf(stdout, "Seed = %d\n", seed);

    srand(seed);
}

double randomNumber(void) {
    return rand() / (double) RAND_MAX;
}
