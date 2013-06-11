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

void generationalGA(void);
void elitestGA(void);

// FUTURE - implement this
bool populationConverged(Population * pop);


int main(int argc, char * argv[]){
    initParameters(argc, argv);
    
    // FIX - for now, change this to the preferred algorithm
    generationalGA();
    
    freeParameters();
    return EXIT_SUCCESS;
}


void elitestGA(void){
    Population * pop, * selected;
    Individual * best_solution;
    int half_pop;
    int generation_num = 0;

    half_pop = POP_SIZE / 2;
    if(POP_SIZE % 2 == 1)
        half_pop++;
    
    pop = genRandPopulation(POP_SIZE);

#ifdef VERBOSE
    fprintf(stdout, "\n----------------------------------------------------------\n\n");
    fprintf(stdout, "Starting Population:\n");
    determineFitness(pop);
    printPopulation(pop);
#endif

    while(generation_num < STOP_CONDITION){
        determineFitness(pop);

#if (defined VERBOSE || defined EXE)
        fprintf(stdout, "\n-----------------   GENERATION %d   -----------------\n", generation_num + 1);
        printPopulation(pop);
#endif

        selected = tournamentSelection(pop, pop->size);
        evolvePopulation(selected);
        determineFitness(selected);

        replaceWorst(pop, selected, half_pop);
        freePopulation(selected);

        generation_num++;
    }

#ifdef VERBOSE
    fprintf(stdout, "\nFinal Population:\n");
    determineFitness(pop);
    printPopulation(pop);
#endif

    fprintf(stdout, "\n-----------------   FINAL RESULT   -----------------\n");
    best_solution = findBest(pop);
    evaluateFitness(best_solution);
    printIndividual(best_solution);

    freePopulation(pop);
}



void generationalGA(void){
    Population * pop, * selected;
    Individual * best_solution;
    int generation_num = 0;
    
    pop = genRandPopulation(POP_SIZE);

    #ifdef VERBOSE
        fprintf(stdout, "\n----------------------------------------------------------\n\n");
        fprintf(stdout, "Starting Population:\n");
        determineFitness(pop);
        printPopulation(pop);
    #endif

    while(generation_num < STOP_CONDITION){
        determineFitness(pop);
        
        #if  (defined VERBOSE || defined EXE)
            fprintf(stdout, "\n-----------------   GENERATION %d   -----------------\n", generation_num + 1);
            printPopulation(pop);
        #endif
        
        selected = tournamentSelection(pop, pop->size);
        evolvePopulation(selected);
        
        freePopulation(pop);
        pop = selected;
        
        generation_num++;
    }

    #ifdef VERBOSE
        fprintf(stdout, "\nFinal Population:\n");
        determineFitness(pop);
        printPopulation(pop);
    #endif
    
    fprintf(stdout, "\n-----------------   FINAL RESULT   -----------------\n");
    determineFitness(pop);
    best_solution = findBest(pop);
    printIndividual(best_solution);
    
    freePopulation(pop);
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
    initArchLibrary(arch_filename);
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
