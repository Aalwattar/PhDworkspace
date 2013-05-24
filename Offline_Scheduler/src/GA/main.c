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
 * Filename : main.c
 * 
 * Purpose  : Contains main and executes the GA
 ******************************************************************************/

#include "fitness.h"
#include "selection.h"
#include "replacement.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <bounds.h>
#include <types.h>
#include <ecodes.h>
#include <napoleon.h>
#include <io.h>
#include <ilp.h>
#include <functions.h>
#include <main.h>


// FIX
char * ARCH_FILENAME = "input/architecture_library.txt";
char * DFG_FILENAME = "input/DFG.txt";

double CROSSOVER_RATE = 0.85;
double MUTATION_RATE = 0.001;
int POP_SIZE = 16; // MUST BE AN EVEN NUMBER

int STOP_CONDITION = 500;
int generation_num;


void initParameters(int num_tokens, char ** input_token);
bool populationConverged(Population * pop);

//FILE *log_strm;
void print_help(void);
int parse_cmd_line_opts(int, char**, t_config*);

short int *succ_adj_mat;
short int *reuse_mat; //aij
t_task_interface *task_interface;
t_task *task;
t_task_type *task_type;


// must be called AFTER initArchLibrary

int initNapoleon(Individual * ind) {
    FILE * grid_strm;
    short int T = 99; //upper_bound_total_exec_time
    int err = 0;
    int fitness;
    int i;

    for (i = 1; i <= task->width; i++) {
        (task + i)->impl = ind->encoding[i - 1];

        (task + i)->columns       = ((arch_library[(task + i)->type - 1]).impl[(task + i)->impl]).columns;
        (task + i)->rows          = ((arch_library[(task + i)->type - 1]).impl[(task + i)->impl]).rows;
        (task + i)->conf_power    = ((arch_library[(task + i)->type - 1]).impl[(task + i)->impl]).conf_p;
        (task + i)->exec_power    = ((arch_library[(task + i)->type - 1]).impl[(task + i)->impl]).exec_p;
        (task + i)->latency       = ((arch_library[(task + i)->type - 1]).impl[(task + i)->impl]).exec_t;
        (task + i)->reconfig_time = ((arch_library[(task + i)->type - 1]).impl[(task + i)->impl]).conf_t;
    }

    //allocate memory for the successor graph adjacency matrix
    succ_adj_mat = (short int*) malloc(sizeof (short int)*(task->width + 2)*(task->width + 2));

    //create the successor matrix
    //and exit on unsuccessful execution of the parse_aif function
    if ((err = create_graph(task, task_interface, succ_adj_mat)))
        print_error(err);

    //allocate memory for the reuse matrix
    reuse_mat = (short int*) malloc(sizeof (short int)*(task->width + 2)*(task->width + 2));

    //create the reuse matrix
    if ((err = create_reuse_mat(task, reuse_mat)))
        print_error(err);

//    fprintf(stderr, "T = %d\n", calc_T(task, &T));
    //T = 20;

    if (!(grid_strm = fopen("output/outScheduler.txt", "w")))
        print_error(__LOG_FILE);

    //call the napoleon scheduler
    fitness = Napoleon(grid_strm, succ_adj_mat, task->width, task);
//    if (!(ilp_strm = fopen("output/ilp_equations.lp", "w")))
//        print_error(__LOG_FILE);

    //uncomment the next line to generate the ILP equations file.
    //ilp_equations(ilp_strm, task, T, succ_adj_mat, reuse_mat);

    fclose(grid_strm);
    free(reuse_mat);
    free(succ_adj_mat);

    return fitness;
}

//    initArchLibrary(ARCH_FILENAME);
//    printArchLibrary();
//    freeArchLibrary();
//    
//    return EXIT_SUCCESS;

int main(int argc, char * argv[]) {
    Population * pop, * mating_pop;
    FILE *aif_strm;
    int err = 0;
    int i, j;

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

    // FIX
    //open the aif input file for reading
    if ((aif_strm = fopen("input/B1_10_5.aif", "r"))) {
        //parse the aif file
        //and exit on unsuccessful execution of the parse_aif function
        if ((err = parse_aif(aif_strm, task, task_interface)))
            print_error(err);
        //fname = strtok(config.aif_fname, ".");
        fclose(aif_strm);
    }
    //assert(aif_strm);


    initArchLibrary(ARCH_FILENAME);
    randSeed();


    pop = genRandPopulation();

    fprintf(stdout, "\n----------------------------------------------------------\n\n");
    fprintf(stdout, "Starting Population:\n");
    for (i = 0; i < POP_SIZE; i++) {
        for (j = 0; j < task->width; j++) {
            fprintf(stdout, "%d", pop->member[i].encoding[j]);
        }
        fprintf(stdout, "\n");
    }

    while (generation_num < STOP_CONDITION) {
        //swhile(!populationConverged(pop)){
        //        for (i = 0; i < POP_SIZE; i++) {
        //            pop->member[i].fitness = evaluateFitness(pop->member[i].encoding);
        //        }

        for (i = 0; i < POP_SIZE; i++) {
            pop->member[i].fitness = initNapoleon(&(pop->member[i]));
        }

//        fprintf(stdout, "\n");
//        for (i = 0; i < POP_SIZE; i++) {
//            for (j = 0; j < task->width; j++) {
//                fprintf(stdout, "%d", pop->member[i].encoding[j]);
//            }
//            fprintf(stdout, "\tfitness = %.5lf\n", pop->member[i].fitness);
//        }

        mating_pop = tournamentSelection(pop);
        freePopulation(pop);

        generateNextGeneration(mating_pop);
        pop = mating_pop;

        generation_num++;
    }

    //fprintf(stdout, "\nGenerations to create best solution = %d\n", generation_num);
    fprintf(stdout, "\nFinal Population:\n");
    for (i = 0; i < POP_SIZE; i++) {
        for (j = 0; j < task->width; j++) {
            fprintf(stdout, "%d", pop->member[i].encoding[j]);
        }
        fprintf(stdout, "\tfitness = %d\n", initNapoleon(&(pop->member[i])));
    }

    freePopulation(pop);



    freeArchLibrary();
    free(task_type);
    free(task_interface);
    free(task);
}


//int main(int argc, char * argv[]) {
//    Population * pop;
//    Population * mating_pop;
//    int i, j;
//
//
//    initParameters(argc, argv);
//    initProblem();
//
//    pop = genRandPopulation();
//
//    fprintf(stdout, "\n----------------------------------------------------------\n\n");
//    fprintf(stdout, "Starting Population:\n");
//    for (i = 0; i < POP_SIZE; i++) {
//        for (j = 0; j < template->num_genes; j++) {
//            fprintf(stdout, "%d", pop->member[i].encoding[j]);
//        }
//        fprintf(stdout, "\n");
//    }
//
//    while (generation_num < STOP_CONDITION) {
//        //swhile(!populationConverged(pop)){
//        for (i = 0; i < POP_SIZE; i++) {
//            pop->member[i].fitness = evaluateFitness(pop->member[i].encoding);
//        }
//
//        //        fprintf(stdout, "\n");
//        //        for(i=0; i<POP_SIZE; i++){
//        //            for(j=0; j<template->num_genes; j++){
//        //                fprintf(stdout, "%d", pop->member[i].encoding[j]);
//        //            }
//        //            fprintf(stdout, "\tfitness = %.5lf\n", pop->member[i].fitness);
//        //        }
//
//        mating_pop = tournamentSelection(pop);
//        freePopulation(pop);
//
//        generateNextGeneration(mating_pop);
//        pop = mating_pop;
//
//        generation_num++;
//    }
//
//    //fprintf(stdout, "\nGenerations to create best solution = %d\n", generation_num);
//    fprintf(stdout, "\nFinal Population:\n");
//    for (i = 0; i < POP_SIZE; i++) {
//        for (j = 0; j < template->num_genes; j++) {
//            fprintf(stdout, "%d", pop->member[i].encoding[j]);
//        }
//        fprintf(stdout, "\n");
//    }
//
//    freePopulation(pop);
//    freeProblem();
//    return 0;
//}

void initParameters(int num_tokens, char ** input_token) {
    int i;

    //    5. selection method
    //    6. crossover type
    //    7. mutation type

    // FIX - make more robust
    for (i = 0; i < num_tokens; i++) {
        if (strncmp(input_token[i], "-ps=", 4) == 0) {
            POP_SIZE = atoi(&(input_token[i][4]));
        }
        if (strncmp(input_token[i], "-mr=", 4) == 0) {
            MUTATION_RATE = atof(&(input_token[i][4]));
        }
        if (strncmp(input_token[i], "-cr=", 4) == 0) {
            CROSSOVER_RATE = atof(&(input_token[i][4]));
        }
        if (strncmp(input_token[i], "-g=", 3) == 0) {
            STOP_CONDITION = atoi(&(input_token[i][3]));
        }
        if (strncmp(input_token[i], "-arch=", 6) == 0) {
            ARCH_FILENAME = &(input_token[i][6]);
        }
        if (strncmp(input_token[i], "-dfg=", 5) == 0) {
            DFG_FILENAME = &(input_token[i][5]);
        }

        if (strncmp(input_token[i], "-seed=", 6) == 0) {
            DFG_FILENAME = &(input_token[i][6]);
        }
    }

    fprintf(stdout, "Parameters:\n\n");
    fprintf(stdout, "\tPopulation Size       = %d\n", POP_SIZE);
    fprintf(stdout, "\tNumber of Generations = %d\n\n", STOP_CONDITION);
    fprintf(stdout, "\tMutation Rate  = %.4lf\n", MUTATION_RATE);
    fprintf(stdout, "\tCrossover Rate = %.4lf\n\n\t", CROSSOVER_RATE);
}

bool populationConverged(Population * pop) {
    int i, j;

    for (i = 0; i < POP_SIZE; i++) {
        for (j = 0; j < template->num_genes; j++) {
            if (pop->member[i].encoding[j] != 0) {
                break;
            }
        }
        if (j == template->num_genes) {
            return true;
        }
    }
    return false;
}


// COMMENTING TEMPLATE

/******************************************************************************
 * NAME : initRandGenerator
 * PURPOSE : 
 * ARGUMENTS : 
 * PRECONDITIONS : 
 * RETURNS : 
 * NOTE : 
 *****************************************************************************/


// FIX - validate each of the arguments
// FIX - Improve the error message
//    if(argc < 2){
//        fprintf(stderr, "Improper arguments. \nPlease try again, using the following format : "
//                "\n\tGA.exe <rand_seed> \n\nFor more information, please view the README file");
//        exit(1);
//    }

