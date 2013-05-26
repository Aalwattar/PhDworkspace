/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 16, 2013
 * Modified : May 22, 2013
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
#include "types.h"

#include <stdlib.h>
#include <stdbool.h>
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
    short int columns;
    short int rows;
    
    short int conf_t;   // configuration time
    short int exec_t;   // execution time (AKA latency)
    short int conf_p;   // configuration power
    short int exec_p;   // execution power (or energy???)
} Implementation; 

// contains all of the architectures of an task
typedef struct{
    short int num_impl;         // the number of architectures for this task
    Implementation * impl;      // the properties of each architecture
}Operation;

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

// FIX - add error checking
bool parseArchLibrary(FILE * fp) {
    char buffer[BUFF_SIZE];
    int num_ops;
    int i;

    // read in the number of possible tasks
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

        // FIX - implement some form of file validation???
        if (buffer[0] == '#')   // a comment
            continue;

        if (strncmp(buffer, "TASK", 4) == 0)
            parseArch(buffer);

        else{
            fprintf(stderr, "GA parseArchLibrary Failed!\n");
            fprintf(stderr, "Unable to understand the line :\n\t%s", buffer);
            return false;
        }
    }
    
    return true;
}

// FIX - add error checking
bool parseArch(char * raw_arch){
    char * token;
    int arch_num;
    int op_type;
    
    op_type = raw_arch[4] - '1';

    arch_num = (arch_library[op_type]).num_impl++;
    (arch_library[op_type]).impl = realloc((arch_library[op_type]).impl, 
            sizeof (Implementation) * (arch_library[op_type]).num_impl);
    
    token = strtok(raw_arch, " ");
    token = strtok(NULL, "\n");
    
    if(sscanf(token, "%hd%*[ ]%hd%*[ ]%hd%*[ ]%hd%*[ ]%hd%*[ ]%hd",
            &(((arch_library[op_type]).impl[arch_num]).columns),
            &(((arch_library[op_type]).impl[arch_num]).rows),
            &(((arch_library[op_type]).impl[arch_num]).conf_t),
            &(((arch_library[op_type]).impl[arch_num]).exec_t),
            &(((arch_library[op_type]).impl[arch_num]).conf_p),
            &(((arch_library[op_type]).impl[arch_num]).exec_p)) < 6){
        
        printf("Could not parse %s\n", token);
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



// A testing function
void printArchLibrary(void){
    int i, j;

    for (i = 0; (arch_library[i]).impl; i++) {
        fprintf(stdout, "TASK%d has %d impls\n", i+1, (arch_library[i]).num_impl);
        
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


short int getNumArch(int opr){
    return arch_library[opr].num_impl;
}


/******************************************************************************
 **********        FITNESS FUNCTION (INTERFACE WITH NAPOLEON)        **********
 *****************************************************************************/

static t_task * task;
static t_task_interface * task_interface;

// must be called AFTER initArchLibrary
// FIX - presently a bit too long and convoluted
bool initNapoleon(char * aif_filename){
    FILE *aif_strm;
    int err = 0;
    int i;

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
        (task + i)->rows = 0;
        (task + i)->reconfig_time = 0;
        (task + i)->width = 0;
        (task + i)->input1 = 0;
        (task + i)->input2 = 0;
        (task + i)->output = 0;

        (task + i)->conf_power = 0;
        (task + i)->exec_power = 0;
        (task + i)->impl = 0;
    }

    task_interface = (t_task_interface*) malloc(sizeof (t_task_interface) * __NUM_MAX_TASK_INTFC);
    for (i = 0; i < __NUM_MAX_TASK_INTFC; i++) {
        (task_interface + i)->mode = 0;
        (task_interface + i)->width = 0;
        (task_interface + i)->reg_out = 0;
    }

    
    aif_strm = fopen(aif_filename, "r");
    if(aif_strm == NULL){
        fprintf(stderr, "Unable to find or open file %s\n", aif_filename);
        return false;
    }
        
    // parse the aif file and print any errors encountered
    err = parse_aif(aif_strm, task, task_interface);
    if (err != __NO_ERROR)
        print_error(err);
    
    fclose(aif_strm);
    return true;
}


void freeNapoleon(){
    free(task_interface);
    free(task);
}




void evaluateFitness(Individual * chromosome){
    short int * succ_adj_mat;
    int err = __NO_ERROR;
    int fitness;
    int i;

    // FIX - make into initialize archInfo function from util.c
    for (i = 1; i <= getNumGenes(); i++) {
        (task + i)->impl = chromosome[i - 1];

        (task + i)->columns       = ((arch_library[(task + i)->type - 1]).impl[(task + i)->impl]).columns;
        (task + i)->rows          = ((arch_library[(task + i)->type - 1]).impl[(task + i)->impl]).rows;
        (task + i)->conf_power    = ((arch_library[(task + i)->type - 1]).impl[(task + i)->impl]).conf_p;
        (task + i)->exec_power    = ((arch_library[(task + i)->type - 1]).impl[(task + i)->impl]).exec_p;
        (task + i)->latency       = ((arch_library[(task + i)->type - 1]).impl[(task + i)->impl]).exec_t;
        (task + i)->reconfig_time = ((arch_library[(task + i)->type - 1]).impl[(task + i)->impl]).conf_t;
    }

    succ_adj_mat = (short int*) malloc(sizeof (short int)*(task->width + 2)*(task->width + 2));
    if ((err = create_graph(task, task_interface, succ_adj_mat)))
        print_error(err);

    //call the scheduler (Napoleon)
    fitness = Napoleon(NULL, succ_adj_mat, getNumGenes(), task);

    free(succ_adj_mat);

    return fitness;
}


int getNumGenes(void){
    // the with member of the source node (task[0]) = the number of tasks
    //   (nodes) in the DFG = Number of genes on a chromosome
    return task[0].width;
}


int getTaskType(int task_num){
    // each task is offset by 1 in Napoleon to accomodate a source + sink
    // the types start at 1, not 0. I convert them to indicies.
    return task[i+1]->type - 1;
}