/*************************************************************************
 * Author   : Ziad Abuowaimer
 * Modifier : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 * 
 * Created  : May 22, 2013
 * Modified : May 22, 2013
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

//local header files
#include <bounds.h>
#include <types.h>
#include <ecodes.h>
#include <napoleon.h>
#include <io.h>
#include <ilp.h>
#include <functions.h>
#include <main.h>


FILE *log_strm;
void print_help(void);
int parse_cmd_line_opts(int, char**, t_config*);

int mainFunction(int argc, char *argv[]) {
    FILE *aif_strm, *res_strm, *ilp_strm, *grid_strm, *impl_strm;
    t_task_interface *task_interface;
    t_task *task;
    t_task_type *task_type;
    t_task_impl *task_impl;
    t_config config;
    int err = 0;
    int		*succ_adj_mat;
    int		*reuse_mat;		//aij
    int		T=99;			//upper_bound_total_exec_time
  char			*fname;
    int i = 0, j = 0;
    int random_num;


    if (argc > 1) {
        if ((err = parse_cmd_line_opts(argc, argv, &config))) {
            print_help();
            fprintf(stderr, "Error Code: %d\n", err);
            exit(err);
        }
    } else {
        print_help();
        fprintf(stderr, "Error Code: %d\n", __LESSARGS);
        exit(__LESSARGS);
    }

    //if(!(log_strm=fopen(config.log_fname,"w")))
    //  print_error(__LOG_FILE);

    //allocate memory for the tasks and the task interfaces
    task = (t_task*) malloc(sizeof (t_task) * __NUM_MAX_TASKS);
    for (i = 0; i < __NUM_MAX_TASKS; i++) {
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

    task_interface = (t_task_interface*) malloc(sizeof (t_task_interface) * __NUM_MAX_TASK_INTFC);
    for (i = 0; i < __NUM_MAX_TASK_INTFC; i++) {
        (task_interface + i)->mode = 0;
        (task_interface + i)->width = 0;
        (task_interface + i)->reg_out = 0;
    }

    //open the aif input file for reading
    if ((aif_strm = fopen(config.aif_fname, "r"))) {
        //parse the aif file
        //and exit on unsuccessful execution of the parse_aif function
        if ((err = parse_aif(aif_strm, task, task_interface)))
            print_error(err);
        //fname = strtok(config.aif_fname, ".");
        fclose(aif_strm);
    }
    //assert(aif_strm);

    //allocate memory for resources
    task_type = (t_task_type*) malloc(sizeof (t_task_type) * __NUM_MAX_TASK_TYPES);
    for (i = 0; i < __NUM_MAX_TASK_TYPES; i++) {
        (task_type + i)->latency = 0;
        (task_type + i)->reconfig_time = 0;
        (task_type + i)->columns = 0;
        (task_type + i)->rows = 0;
    }

    //open the resource file for reading
    if ((res_strm = fopen(config.res_fname, "r"))) {
        //parse the resource file
        //and exit on unsuccessful execution of the parse_res function
        if ((err = parse_res(res_strm, task_type)))
            print_error(err);
        fclose(res_strm);
    }
    //assert(res_strm);

    task_impl = (t_task_impl*) malloc(sizeof (t_task_impl) * __NUM_MAX_TASK_IMPLS);
    for (i = 0; i < __NUM_MAX_TASK_IMPLS; i++) {
        (task_impl + i)->id = 0;
        (task_impl + i)->type = 0;
        (task_impl + i)->impl = 0;
        (task_impl + i)->latency = 0;
        (task_impl + i)->reconfig_time = 0;
        (task_impl + i)->columns = 0;
        (task_impl + i)->rows = 0;
        (task_impl + i)->reconfig_pwr = 0;
        (task_impl + i)->exec_pwr = 0;
    }

    //open the resource file for reading
    if ((impl_strm = fopen("input/arch_lib.txt", "r"))) {
        //parse the impl file
        //and exit on unsuccessful execution of the parse_impl function
        if ((err = parse_impl(impl_strm, task_impl)))
            print_error(err);
        fclose(impl_strm);
    }

#ifdef __DEBUG
    //display the task types info
    display_task_type(task_type);
#endif

    set_task_parameter(task, task_type);

    display_task_impl(task_impl);

    //allocate memory for the successor graph adjacency matrix
    succ_adj_mat = (int*)malloc(sizeof ( int)*(task->width + 2)*(task->width + 2));

    //create the successor matrix
    //and exit on unsuccessful execution of the parse_aif function
    if ((err = create_graph(task, task_interface, succ_adj_mat)))
        print_error(err);

    //allocate memory for the reuse matrix
    reuse_mat = (int*)malloc(sizeof ( int)*(task->width + 2)*(task->width + 2));

    //create the reuse matrix
    if ((err = create_reuse_mat(task, reuse_mat)))
        print_error(err);

    fprintf(stderr, "T = %d\n", calc_T(task, &T));
    //T = 20;

    if (!(grid_strm = fopen("output/outScheduler.txt", "w")))
        print_error(__LOG_FILE);

    for (j = 0; j < 20; j++) {
        for (i = 1; i <= task->width; i++) {
            switch ((task + i)->type) {
                case __ADD:
                {
                    srand(time(NULL) * i * (j + 1));
                    //srand(time(NULL)*rand());
                    random_num = (rand() % 2) + 1;
                    (task + i)->impl = (task_impl + random_num)->impl;
                    (task + i)->latency = (task_impl + random_num)->latency;
                    (task + i)->reconfig_time = (task_impl + random_num)->reconfig_time;
                    (task + i)->columns = (task_impl + random_num)->columns;
                    (task + i)->rows = (task_impl + random_num)->rows;
                    (task + i)->reconfig_pwr = (task_impl + random_num)->reconfig_pwr;
                    (task + i)->exec_pwr = (task_impl + random_num)->exec_pwr;
                    break;
                }
                case __MULT:
                {
                    srand(time(NULL) * i * (j + 1));
                    //srand(time(NULL)*rand());
                    random_num = (rand() % 3) + 4;
                    (task + i)->impl = (task_impl + random_num)->impl;
                    (task + i)->latency = (task_impl + random_num)->latency;
                    (task + i)->reconfig_time = (task_impl + random_num)->reconfig_time;
                    (task + i)->columns = (task_impl + random_num)->columns;
                    (task + i)->rows = (task_impl + random_num)->rows;
                    (task + i)->reconfig_pwr = (task_impl + random_num)->reconfig_pwr;
                    (task + i)->exec_pwr = (task_impl + random_num)->exec_pwr;
                    break;
                }
                case __SUB:
                {
                    srand(time(NULL) * i * (j + 1));
                    //srand(time(NULL)*rand());
                    random_num = (rand() % 4) + 8;
                    (task + i)->impl = (task_impl + random_num)->impl;
                    (task + i)->latency = (task_impl + random_num)->latency;
                    (task + i)->reconfig_time = (task_impl + random_num)->reconfig_time;
                    (task + i)->columns = (task_impl + random_num)->columns;
                    (task + i)->rows = (task_impl + random_num)->rows;
                    (task + i)->reconfig_pwr = (task_impl + random_num)->reconfig_pwr;
                    (task + i)->exec_pwr = (task_impl + random_num)->exec_pwr;
                    break;
                }
                case __DIV:
                {
                    srand(time(NULL) * i * (j + 1));
                    //srand(time(NULL)*rand());
                    random_num = (rand() % 1) + 13;
                    (task + i)->impl = (task_impl + random_num)->impl;
                    (task + i)->latency = (task_impl + random_num)->latency;
                    (task + i)->reconfig_time = (task_impl + random_num)->reconfig_time;
                    (task + i)->columns = (task_impl + random_num)->columns;
                    (task + i)->rows = (task_impl + random_num)->rows;
                    (task + i)->reconfig_pwr = (task_impl + random_num)->reconfig_pwr;
                    (task + i)->exec_pwr = (task_impl + random_num)->exec_pwr;
                    break;
                }
            }
            (task + i)->exec_sched = 0;
            (task + i)->reconfig_sched = 0;
            (task + i)->leftmost_column = 0;
            (task + i)->bottommost_row = 0;
            //(task+i)->reconfig_pwr = 0;
            //(task+i)->exec_pwr = 0;


        }
        /*  (task+1)->latency = 75;
        (task + 1)->reconfig_time = 30;
        (task + 1)->columns = 3;
        (task + 1)->rows = 2;
        (task + 1)->exec_sched = 0;
        (task + 1)->reconfig_sched = 0;
        (task + 1)->leftmost_column = 0;
        (task + 1)->bottommost_row = 0;
        (task + 1)->reconfig_pwr = 0;
        (task + 1)->exec_pwr = 0;
          (task+1)->impl = 2;

        (task + 2)->latency = 40;
        (task + 2)->reconfig_time = 125;
        (task + 2)->columns = 5;
        (task + 2)->rows = 5;
        (task + 2)->exec_sched = 0;
        (task + 2)->reconfig_sched = 0;
        (task + 2)->leftmost_column = 0;
        (task + 2)->bottommost_row = 0;
        (task + 2)->reconfig_pwr = 0;
        (task + 2)->exec_pwr = 0;
          (task+2)->impl = 2;

        (task + 3)->latency = 10;
        (task + 3)->reconfig_time = 20;
        (task + 3)->columns = 2;
        (task + 3)->rows = 2;
        (task + 3)->exec_sched = 0;
        (task + 3)->reconfig_sched = 0;
        (task + 3)->leftmost_column = 0;
        (task + 3)->bottommost_row = 0;
        (task + 3)->reconfig_pwr = 0;
        (task + 3)->exec_pwr = 0;
          (task+3)->impl = 3;

        (task + 4)->latency = 75;
        (task + 4)->reconfig_time = 30;
        (task + 4)->columns = 3;
        (task + 4)->rows = 2;
        (task + 4)->exec_sched = 0;
        (task + 4)->reconfig_sched = 0;
        (task + 4)->leftmost_column = 0;
        (task + 4)->bottommost_row = 0;
        (task + 4)->reconfig_pwr = 0;
        (task + 4)->exec_pwr = 0;
          (task+4)->impl = 2;

        (task + 5)->latency = 30;
        (task + 5)->reconfig_time = 5;
        (task + 5)->columns = 1;
        (task + 5)->rows = 1;
        (task + 5)->exec_sched = 0;
        (task + 5)->reconfig_sched = 0;
        (task + 5)->leftmost_column = 0;
        (task + 5)->bottommost_row = 0;
        (task + 5)->reconfig_pwr = 0;
        (task + 5)->exec_pwr = 0;
          (task+5)->impl = 1;

        (task + 6)->latency = 10;
        (task + 6)->reconfig_time = 10;
        (task + 6)->columns = 1;
        (task + 6)->rows = 2;
        (task + 6)->exec_sched = 0;
        (task + 6)->reconfig_sched = 0;
        (task + 6)->leftmost_column = 0;
        (task + 6)->bottommost_row = 0;
        (task + 6)->reconfig_pwr = 0;
        (task + 6)->exec_pwr = 0;
          (task+6)->impl = 2;

          (task+7)->latency = 125;
          (task+7)->reconfig_time = 20;
          (task+7)->columns = 2;
        (task + 7)->rows = 2;
        (task + 7)->exec_sched = 0;
        (task + 7)->reconfig_sched = 0;
        (task + 7)->leftmost_column = 0;
        (task + 7)->bottommost_row = 0;
        (task + 7)->reconfig_pwr = 0;
        (task + 7)->exec_pwr = 0;
          (task+7)->impl = 1;

        (task + 8)->latency = 10;
        (task + 8)->reconfig_time = 20;
        (task + 8)->columns = 2;
        (task + 8)->rows = 2;
        (task + 8)->exec_sched = 0;
        (task + 8)->reconfig_sched = 0;
        (task + 8)->leftmost_column = 0;
        (task + 8)->bottommost_row = 0;
        (task + 8)->reconfig_pwr = 0;
        (task + 8)->exec_pwr = 0;
          (task+8)->impl = 3;

        (task + 9)->latency = 10;
        (task + 9)->reconfig_time = 10;
        (task + 9)->columns = 1;
        (task + 9)->rows = 2;
        (task + 9)->exec_sched = 0;
        (task + 9)->reconfig_sched = 0;
        (task + 9)->leftmost_column = 0;
        (task + 9)->bottommost_row = 0;
        (task + 9)->reconfig_pwr = 0;
        (task + 9)->exec_pwr = 0;
          (task+9)->impl = 2;

        (task + 10)->latency = 5;
        (task + 10)->reconfig_time = 30;
        (task + 10)->columns = 3;
        (task + 10)->rows = 2;
        (task + 10)->exec_sched = 0;
        (task + 10)->reconfig_sched = 0;
        (task + 10)->leftmost_column = 0;
        (task + 10)->bottommost_row = 0;
        (task + 10)->reconfig_pwr = 0;
        (task + 10)->exec_pwr = 0;
          (task+10)->impl = 4;
         */
        display_task(task, task_interface);

        //call the napoleon scheduler
        Napoleon(grid_strm, succ_adj_mat, task->width, task);
    }
    if (!(ilp_strm = fopen("ilp_equations.lp", "w")))
        print_error(__LOG_FILE);

    //uncomment the next line to generate the ILP equations file.
    //ilp_equations(ilp_strm, task, T, succ_adj_mat, reuse_mat);

    fclose(ilp_strm);
    fclose(grid_strm);

    free(reuse_mat);
    free(succ_adj_mat);
    free(task_type);
    free(task_interface);
    free(task);

    exit(0);
}


void print_help(void) {
    fprintf(stderr, "hyperspace <options>\n");
    fprintf(stderr, "<options>: -aif <aif file>\n");
    fprintf(stderr, "           -res_file <resource constraints file>\n");
    fprintf(stderr, "          [-log_file <log file>]\n");
    fprintf(stderr, "          [-options_file <options file>]\n");
}


int parse_cmd_line_opts(int argc, char *argv[], t_config *config) {
    int err = 0;
    int count = 1;

    strcpy(config->log_fname, "hs.log");

    while (count < argc) {
        if (!strcasecmp(*(argv + count), "-aif")) {
            char *aif_fname;
            if ((aif_fname = *(argv + count + 1)))
                strcpy(config->aif_fname, aif_fname);
            else
                err = __AIF_FILE;
            count++;
        } else if (!strcasecmp(*(argv + count), "-log_file")) {
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
        } else {
            err = __UNRECOPT;
        }

        count++;
    }

    return err;
}


