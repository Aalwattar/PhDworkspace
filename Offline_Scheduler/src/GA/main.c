/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 7, 2013
 * Modified : May 31, 2013
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
#include <stdbool.h>
#include <getopt.h>

#include <bounds.h>
#include <types.h>
#include <ecodes.h>
#include <napoleon.h>
#include <io.h>
#include <ilp.h>
#include <functions.h>
#include <main.h>

#define ARCH_FILENAME "input/architecture_library.txt"
#define AIF_FILENAME  "input/B1_10_5.aif"

static int STOP_CONDITION = 500;
static int POP_SIZE = 50;


void initParameters(int, char **);
void freeParameters(void);
void setPopSize(int);

// FUTURE - implement this
bool populationConverged(Population * pop);

// a generational keep-best approach
int main(int argc, char * argv[]){
    Population * pop, * selected;
    Individual * best_solution;
    int generation_num = 0;
    
    initParameters(argc, argv);
    pop = genRandPopulation(POP_SIZE);
    determineFitness(pop);
    sortByFitness(pop);

    #ifdef VERBOSE
        fprintf(stdout, "\n----------------------------------------------------------\n\n");
        fprintf(stdout, "Starting Population:\n");
        printPopulation(pop);
    #endif

    while(generation_num < STOP_CONDITION){
        #if  (defined VERBOSE || defined EXE)
            fprintf(stdout, "\n-----------------   GENERATION %d   -----------------\n", generation_num + 1);
            printPopulation(pop);
        #endif
        
        selected = tournamentSelection(pop, pop->size);
        evolvePopulation(selected);
        determineFitness(selected);
        
        pop = retainBest(pop, selected);

        generation_num++;
    }

    #ifdef VERBOSE
        fprintf(stdout, "\nFinal Population:\n");
        printPopulation(pop);
    #endif
    
    fprintf(stdout, "\n-----------------   FINAL RESULT   -----------------\n");
    best_solution = findBest(pop);
    printIndividual(best_solution);
    
    freePopulation(pop);
    freeParameters();
    return EXIT_SUCCESS;
}

void initParameters(int argc, char ** argv){
    char * arch_filename = ARCH_FILENAME;
    char * aif_filename = AIF_FILENAME;
    int seed = randSeed();
    int c;

    opterr = 0;

    while((c = getopt(argc, argv, "a:c:d:g:m:p:r:s:t:")) != -1){
        switch(c){
            case 'a':
                arch_filename = optarg;
                break;
            case 'c':
                setCrossoverRate(atof(optarg));
                break;
            case 'd':
                aif_filename = optarg;
                break;
            case 'g':
                STOP_CONDITION = atoi(optarg);
                break;
            case 'm':
                setMutationRate(atof(optarg));
                break;
            case 'p':
                setPopSize(atoi(optarg));
                break;
            case 'r':
                // FIX - REPLACEMENT METHOD = generatinoal or replaceWorst with varying parameters
                break;
            case 's':
                // FIX - SELECTION METHOD = tournament selection or random
                break;
                
            case 't':
                seed = atoi(optarg);
                break;
                
            case ':':   
                fprintf(stderr, "Option -%c requires an operand\n", optopt);
                break;
            case '?':
                fprintf(stderr, "Unrecognized option: -%c\n", optopt);

            default:
                exit(1);
        }
    }

    if(optind < argc){
        printf("Non-option argument %s\n", argv[optind]);
        exit(1);
    }
    
    // FIX - Check the return values
    seedRandGenerator(seed);
    
    if(initArchLibrary(arch_filename) != true)
        exit(1);
    
    initNapoleon(aif_filename);

    fprintf(stdout, "Parameters:\n");
    fprintf(stdout, "\tSeed = %d\n\n", seed);
    fprintf(stdout, "\tPopulation Size       = %d\n", POP_SIZE);
    fprintf(stdout, "\tNumber of Generations = %d\n\n", STOP_CONDITION);
    fprintf(stdout, "\tMutation Rate  = %.4lf\n", getMutationRate());
    fprintf(stdout, "\tCrossover Rate = %.4lf\n\n", getCrossoverRate());
}


void freeParameters(void){
    freeArchLibrary();
    freeNapoleon();
}

void setPopSize(int size){
    if(2 <= size && size <= 10000){
        POP_SIZE = size;
        return;
    }
    
    fprintf(stderr, "Invalid population size %d.\n", size);
    fprintf(stderr, "The population size must be an integer between 0 and 1\n");
    
    exit(1);
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
