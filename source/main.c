/*************************************************************************
 * Author   : Ziad Abuowaimer
 * Modifier : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 * 
 * Created  : May 22, 2013
 * Modified : July 3, 2013
 *************************************************************************/

/*************************************************************************/
// Filename: main.c
// Description:
// This file is the top level file which integrates all the Scheduler tasks.
/*************************************************************************/

//global header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>

//local header files
#include <bounds.h>
#include <types.h>
#include <ecodes.h>
#include <napoleon.h>
#include <io.h>
#include <ilp.h>
#include <functions.h>
#include <main.h>
#include <stdbool.h>
#include <confuse.h>    // WARNING - NON STANDARD LIBRARY

#include "common_interfaces.h"

FILE *log_strm;
void print_help(void);
int parse_cmd_line_opts(int, char**, t_config*);

int main(int argc, char *argv[]){
    DFG dfg;
    Architecture_Library arch_lib;
    FILE *aif_strm, *res_strm, *ilp_strm, *grid_strm, *impl_strm;
    t_task_interface *task_interface;
    t_task *task;
    t_task_type *task_type;
    t_task_impl *task_impl;
    t_config config;
    int err = 0;
    int *succ_adj_mat;
    int *reuse_mat; //aij
    int T = 99; //upper_bound_total_exec_time
    int i = 0, j = 0;
    int random_num;

    if(argc > 2){
        if((err = parse_cmd_line_opts(argc, argv, &config))){
            print_help();
            fprintf(stderr, "Error Code: %d\n", err);
            exit(err);
        }
    }else{
        print_help();
        fprintf(stderr, "Error Code: %d\n", __LESSARGS);
        exit(__LESSARGS);
    }

    //if(!(log_strm=fopen(config.log_fname,"w")))
    //  print_error(__LOG_FILE);

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


    // MODIFIED BY: Jennifer Winer 
    initDFG(config.aif_fname, &dfg);
    
    // my file parsing function opens the file instead
    if((err = parse_aif(&dfg, task, task_interface)))
        print_error(err);
    
    freeDFG(&dfg);

//    //allocate memory for resources
//    task_type = (t_task_type*)malloc(sizeof (t_task_type) * __NUM_MAX_TASK_TYPES);
//    for(i = 0; i < __NUM_MAX_TASK_TYPES; i++){
//        (task_type + i)->latency = 0;
//        (task_type + i)->reconfig_time = 0;
//        (task_type + i)->columns = 0;
//        (task_type + i)->rows = 0;
//    }
//
//    //open the resource file for reading
//    if((res_strm = fopen(config.res_fname, "r"))){
//        //parse the resource file
//        //and exit on unsuccessful execution of the parse_res function
//        if((err = parse_res(res_strm, task_type)))
//            print_error(err);
//        fclose(res_strm);
//    }
//    //assert(res_strm);

    
    
//    task_impl = (t_task_impl*)malloc(sizeof (t_task_impl) * __NUM_MAX_TASK_IMPLS);
//    for(i = 0; i < __NUM_MAX_TASK_IMPLS; i++){
//        (task_impl + i)->id = 0;
//        (task_impl + i)->type = 0;
//        (task_impl + i)->impl = 0;
//        (task_impl + i)->latency = 0;
//        (task_impl + i)->reconfig_time = 0;
//        (task_impl + i)->columns = 0;
//        (task_impl + i)->rows = 0;
//        (task_impl + i)->reconfig_pwr = 0;
//        (task_impl + i)->exec_pwr = 0;
//    }

    // MODIFIED BY: Jennifer Winer
    initArchLibrary(config.arch_fname, &arch_lib);
         
    
//    if((impl_strm = fopen("input/arch_lib.txt", "r"))){
//        //parse the impl file
//        //and exit on unsuccessful execution of the parse_impl function
//        if((err = parse_impl(impl_strm, task_impl)))
//            print_error(err);
//        fclose(impl_strm);
//    }
#ifdef __DEBUG
    //display the task types info
    display_task_type(task_type);
#endif

    //set_task_parameter(task, task_type);

//    display_task_impl(task_impl);

    //allocate memory for the successor graph adjacency matrix
    succ_adj_mat = (int*)malloc(sizeof ( int)*(task->width + 2)*(task->width + 2));

    //create the successor matrix
    //and exit on unsuccessful execution of the parse_aif function
    if((err = create_graph(task, task_interface, succ_adj_mat)))
        print_error(err);

    //allocate memory for the reuse matrix
    reuse_mat = (int*)malloc(sizeof ( int)*(task->width + 2)*(task->width + 2));

    //create the reuse matrix
    if((err = create_reuse_mat(task, reuse_mat)))
        print_error(err);

    fprintf(stderr, "T = %d\n", calc_T(task, &T));
    //T = 20;

//    if(!(grid_strm = fopen("outScheduler.txt", "w")))
//        print_error(__LOG_FILE);

    for(j = 0; j < 20; j++){
        for(i = 1; i <= task->width; i++){
            switch((task + i)->type){
                case __ADD:
                {
                    srand(time(NULL) * i * (j + 1));
                    random_num = 1;
                    (task + i)->impl = 1;
                    (task + i)->latency = arch_lib.task[__ADD - 1].impl[(task + i)->impl - 1].exec_time;
                    (task + i)->reconfig_time = arch_lib.task[__ADD - 1].impl[(task + i)->impl - 1].config_time;
                    (task + i)->columns = arch_lib.task[__ADD - 1].impl[(task + i)->impl - 1].columns;
                    (task + i)->rows = arch_lib.task[__ADD - 1].impl[(task + i)->impl - 1].rows;
                    (task + i)->reconfig_pwr = arch_lib.task[__ADD - 1].impl[(task + i)->impl - 1].config_power;
                    (task + i)->exec_pwr = arch_lib.task[__ADD - 1].impl[(task + i)->impl - 1].exec_power;
                    break;
                }
                case __MULT:
                {
                    srand(time(NULL) * i * (j + 1));
                    random_num = 4;
                    (task + i)->impl = 1;
                    (task + i)->latency = arch_lib.task[__MULT - 1].impl[(task + i)->impl - 1].exec_time;
                    (task + i)->reconfig_time = arch_lib.task[__MULT - 1].impl[(task + i)->impl - 1].config_time;
                    (task + i)->columns = arch_lib.task[__MULT - 1].impl[(task + i)->impl - 1].columns;
                    (task + i)->rows = arch_lib.task[__MULT - 1].impl[(task + i)->impl - 1].rows;
                    (task + i)->reconfig_pwr = arch_lib.task[__MULT - 1].impl[(task + i)->impl - 1].config_power;
                    (task + i)->exec_pwr = arch_lib.task[__MULT - 1].impl[(task + i)->impl - 1].exec_power;
                    break;
                }
                case __SUB:
                {
                    srand(time(NULL) * i * (j + 1));
                    random_num = 8;
                    (task + i)->impl = 1;
                    (task + i)->latency = arch_lib.task[__SUB - 1].impl[(task + i)->impl - 1].exec_time;
                    (task + i)->reconfig_time = arch_lib.task[__SUB - 1].impl[(task + i)->impl - 1].config_time;
                    (task + i)->columns = arch_lib.task[__SUB - 1].impl[(task + i)->impl - 1].columns;
                    (task + i)->rows = arch_lib.task[__SUB - 1].impl[(task + i)->impl - 1].rows;
                    (task + i)->reconfig_pwr = arch_lib.task[__SUB - 1].impl[(task + i)->impl - 1].config_power;
                    (task + i)->exec_pwr = arch_lib.task[__SUB - 1].impl[(task + i)->impl - 1].exec_power;
                    break;
                }
                case __DIV:
                {
                    srand(time(NULL) * i * (j + 1));
                    random_num = 13;
                    (task + i)->impl = 1;
                    (task + i)->latency = arch_lib.task[__DIV - 1].impl[(task + i)->impl - 1].exec_time;
                    (task + i)->reconfig_time = arch_lib.task[__DIV - 1].impl[(task + i)->impl - 1].config_time;
                    (task + i)->columns = arch_lib.task[__DIV - 1].impl[(task + i)->impl - 1].columns;
                    (task + i)->rows = arch_lib.task[__DIV - 1].impl[(task + i)->impl - 1].rows;
                    (task + i)->reconfig_pwr = arch_lib.task[__DIV - 1].impl[(task + i)->impl - 1].config_power;
                    (task + i)->exec_pwr = arch_lib.task[__DIV - 1].impl[(task + i)->impl - 1].exec_power;
                    break;
                }
            }
            (task + i)->exec_sched = 0;
            (task + i)->reconfig_sched = 0;
            (task + i)->leftmost_column = 0;
            (task + i)->bottommost_row = 0;

        }
        
        display_task(task, task_interface);

        //call the napoleon scheduler
        Napoleon(NULL, succ_adj_mat, task->width, task);
    }
    
//    if(!(ilp_strm = fopen("ilp_equations.lp", "w")))
//        print_error(__LOG_FILE);

    //uncomment the next line to generate the ILP equations file.
    //ilp_equations(ilp_strm, task, T, succ_adj_mat, reuse_mat);

//    fclose(ilp_strm);
//    fclose(grid_strm);

    free(reuse_mat);
    free(succ_adj_mat);
//    free(task_impl);
//    free(task_type);
    free(task_interface);
    freeArchLibrary(&arch_lib);
    free(task);

    exit(0);
}


void print_help(void){
    fprintf(stderr, "hyperspace <options>\n");
    fprintf(stderr, "<options>: -aif <aif file>\n");
    fprintf(stderr, "           -res_file <resource constraints file>\n");
    fprintf(stderr, "          [-log_file <log file>]\n");
    fprintf(stderr, "          [-options_file <options file>]\n");
}


int parse_cmd_line_opts(int argc, char *argv[], t_config *config){
    int err = 0;
    int count = 1;

    strcpy(config->log_fname, "hs.log");

    while(count < argc){
        if(!strcasecmp(*(argv + count), "-aif")){
            char *aif_fname;
            if((aif_fname = *(argv + count + 1)))
                strcpy(config->aif_fname, aif_fname);
            else
                err = __AIF_FILE;
            count++;
        }else if(!strcasecmp(*(argv + count), "-log_file")){
            char *log_fname;
            if ((log_fname = *(argv + count + 1)))
                strcpy(config->log_fname, log_fname);
            else
                err = __LOG_FILE;
            count++;
        } else if ((!strcasecmp(*(argv + count), "-res_file"))) {
            char * res_fname;
            if ((res_fname = *(argv + count + 1)))
                strcpy(config->res_fname, res_fname);
            else
                err = __RES_FILE;
            count++;
        } else if ((!strcasecmp(*(argv + count), "-options_file"))) {
            char *opt_fname;
            if ((opt_fname = *(argv + count + 1)))
                strcpy(config->opt_fname, opt_fname);
            else
                err = __OPT_FILE;
            count++;
            
        } else if ((!strcasecmp(*(argv + count), "-arch_file"))) {
            char * arch_fname;
            if ((arch_fname = *(argv + count + 1)))
                strcpy(config->arch_fname, arch_fname);
            else
                err = __RES_FILE;
            count++;
            
        }else{
            err = __UNRECOPT;
        }

        count++;
    }

    return err;
}
