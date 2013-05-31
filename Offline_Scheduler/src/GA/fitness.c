/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 16, 2013
 * Modified : May 30, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : fitness.c
 * 
 * Purpose  : A library for integration with Ziad's Napoleon
 * 
 *            Napoleon will act as our fitness function, taking a mapping of 
 *              architectures to each task as input, and returning the runtime
 *              and power required for that schedule as a fitness function
 ******************************************************************************/

#include "fitness.h"
#include "ecodes.h"
#include "functions.h"
#include "napoleon.h"
#include "io.h"
#include "types.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>


#define RUNTIME_WEIGHT 0.6
#define POWER_WEIGHT 1 - RUNTIME_WEIGHT

#define BUFF_SIZE 500

/******************************************************************************
 *****************         ARHITECTURE  DATA STORAGE         ******************
 *****************************************************************************/

// represents one architecture's implementation
typedef struct {
    int columns;
    int rows;

    int conf_t; // configuration time
    int exec_t; // execution time (AKA latency)
    int conf_p; // configuration power
    int exec_p; // execution power (or energy???)
} Implementation;


// contains all of the architectures of an task
typedef struct {
    int num_impl; // the number of architectures for this task
    Implementation * impl; // the properties of each architecture
} Operation;


static Operation * arch_library;


/******************************************************************************
 *****************            ARCHITECTURE FILE I/O           *****************
 *****************************************************************************/

bool parseArchLibrary(FILE *);
bool parseArch(char *);

bool initArchLibrary(char * filename) {
    FILE * fp;
    bool parse_status = true;

    if (filename == NULL)
        return false;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Unable to find or open file %s\n", filename);
        return false;
    }

    parse_status = parseArchLibrary(fp);
    fclose(fp);

    return parse_status;
}


bool parseArchLibrary(FILE * fp) {
    char buffer[BUFF_SIZE];
    int num_ops;
    int i;

    // read in the number of task types
    fscanf(fp, "%*s %d", &num_ops);

    // create and clear data structures
    arch_library = malloc(sizeof (Operation) * (num_ops + 1));
    for (i = 0; i < num_ops + 1; i++) {
        (arch_library[i]).impl = NULL;
        (arch_library[i]).num_impl = 0;
    }

    while (fgets(buffer, BUFF_SIZE, fp) != NULL) {
        if (strlen(buffer) < 2) // a blank line
            continue;

        if (buffer[0] == '#') // a comment
            continue;

        if (strncmp(buffer, "TASK", 4) == 0){
            if(parseArch(buffer) != true)
                return false;
        }

        else {
            fprintf(stderr, "GA parseArchLibrary Failed!\n");
            fprintf(stderr, "Unable to understand the line :\n\t%s", buffer);
            return false;
        }
    }

    return true;
}

bool parseArch(char * raw_arch) {
    int arch_num;
    int op_type;

    op_type = raw_arch[4] - '1';

    arch_num = (arch_library[op_type]).num_impl++;
    (arch_library[op_type]).impl = realloc((arch_library[op_type]).impl,
            sizeof (Implementation) * (arch_library[op_type]).num_impl);

    if (sscanf(raw_arch, "%*s%*[ ]%d%*[ ]%d%*[ ]%d%*[ ]%d%*[ ]%d%*[ ]%d",
            &(((arch_library[op_type]).impl[arch_num]).columns),
            &(((arch_library[op_type]).impl[arch_num]).rows),
            &(((arch_library[op_type]).impl[arch_num]).conf_t),
            &(((arch_library[op_type]).impl[arch_num]).exec_t),
            &(((arch_library[op_type]).impl[arch_num]).conf_p),
            &(((arch_library[op_type]).impl[arch_num]).exec_p)) < 6) {

        fprintf(stderr, "Could not parse the line : %s\n", raw_arch);
        return false;
    }

    return true;
}



void freeArchLibrary(void) {
    int i;

    for (i = 0; (arch_library[i]).impl != NULL; i++)
        free((arch_library[i]).impl);

    free(arch_library);
}



void printArchLibrary(void) {
    int i, j;

    for (i = 0; (arch_library[i]).impl; i++) {
        fprintf(stdout, "TASK%d has %d impls\n", i + 1, (arch_library[i]).num_impl);

        for (j = 0; j < (arch_library[i]).num_impl; j++) {
            fprintf(stdout, "%d\t%d\t%d\t%d\t%d\t%d\n",
                    ((arch_library[i]).impl[j]).columns,
                    ((arch_library[i]).impl[j]).rows,
                    ((arch_library[i]).impl[j]).conf_t,
                    ((arch_library[i]).impl[j]).exec_t,
                    ((arch_library[i]).impl[j]).conf_p,
                    ((arch_library[i]).impl[j]).exec_p);
        }
    }
}

int getNumArch(int opr) {
    return arch_library[opr].num_impl;
}


/******************************************************************************
 **********        FITNESS FUNCTION (INTERFACE WITH NAPOLEON)        **********
 *****************************************************************************/

        // FIX - MAKE NON GLOBAL!!!
static t_task_interface *task_interface; //
static t_task *task; //
    
    
    // FIX - make smaller
// FIX - add error checking
bool initNapoleon(char * aif_filename) {
    FILE *aif_strm;
    int err;
    int i;

    // FIX - parse command line arguments

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

    aif_strm = fopen(aif_filename, "r");
    if(aif_strm == NULL)
        return false;
        
    err = parse_aif(aif_strm, task, task_interface);
    fclose(aif_strm);
    
    if(err != __NO_ERROR){
        print_error(err);
        return false;
    }
    
    return true;
}

void freeNapoleon(void){  
     free(task_interface);
     free(task);
}

// FIX - add error checking
int evaluateFitness(int * chromosome){
    GA_Info schedule;
    int * succ_adj_mat;
    int * reuse_mat;
    int err = __NO_ERROR;
    int i = 0;
    
    //allocate memory for the successor graph adjacency matrix
    succ_adj_mat = (int*) malloc(sizeof (int)*(task->width + 2)*(task->width + 2));

    //create the successor matrix
    //and exit on unsuccessful execution of the parse_aif function
    if ((err = create_graph(task, task_interface, succ_adj_mat)))
        print_error(err);

    //allocate memory for the reuse matrix
    reuse_mat = (int*) malloc(sizeof (int)*(task->width + 2)*(task->width + 2));

    //create the reuse matrix
    if ((err = create_reuse_mat(task, reuse_mat)))
        print_error(err);
    
    for (i = 0; i < getNumGenes(); i++) {
        task[i + 1].impl = chromosome[i];

        task[i + 1].columns = getColumns(i);
        task[i + 1].rows = getRows(i);
        task[i + 1].reconfig_pwr = getConfigPower(i);
        task[i + 1].exec_pwr = getExecPower(i);
        task[i + 1].latency = getExecTime(i);
        task[i + 1].reconfig_time = getConfigTime(i);
        
        task[i + 1].exec_sched = 0;
        task[i + 1].reconfig_sched = 0;
        task[i + 1].bottommost_row = 0;
        task[i + 1].leftmost_column = 0;
    }
    
    //display_task(task, task_interface);

    schedule = Napoleon(NULL, succ_adj_mat, task->width, task);
    
    free(reuse_mat);
    free(succ_adj_mat);
    
    return (schedule.power * POWER_WEIGHT) + (schedule.runtime * RUNTIME_WEIGHT);
}


int getNumGenes(void) {
    // the with member of the source node (task[0]) = the number of tasks
    //   (nodes) in the DFG = Number of genes on a chromosome
    return task[0].width;
}

// this function interfaces the GA's indices (which all start at 0) with Napoleon (starts at 1)
int getTaskType(int task_num) {
    // each task is offset by 1 in Napoleon to accomodate a source + sink
    return task[task_num + 1].type - 1;
}

int getColumns(int task_num) {
    return ((arch_library[getTaskType(task_num)]).impl[task[task_num + 1].impl]).columns;
}

int getRows(int task_num) {
    return ((arch_library[getTaskType(task_num)]).impl[task[task_num + 1].impl]).rows;
}

int getConfigTime(int task_num) {
    return ((arch_library[getTaskType(task_num)]).impl[task[task_num + 1].impl]).conf_t;
}

int getExecTime(int task_num) {
    return ((arch_library[getTaskType(task_num)]).impl[task[task_num + 1].impl]).exec_t;
}

int getConfigPower(int task_num) {
    return ((arch_library[getTaskType(task_num)]).impl[task[task_num + 1].impl]).conf_p;
}

int getExecPower(int task_num) {
    return ((arch_library[getTaskType(task_num)]).impl[task[task_num + 1].impl]).exec_p;
}