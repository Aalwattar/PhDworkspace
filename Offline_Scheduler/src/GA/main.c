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

int STOP_CONDITION = 1000;
int generation_num = 0;


void initParameters(int, char **);
bool populationConverged(Population * pop);
void freeParameters(Population *);

void bruteForce();


int main(int argc, char * argv[]){
    Population * pop;

    initParameters(argc, argv);
    pop = genRandPopulation();
    determineFitness(pop);

    fprintf(stdout, "\n----------------------------------------------------------\n\n");
    fprintf(stdout, "Starting Population:\n");
    printPopulation(pop);

    while(generation_num < STOP_CONDITION){
        #ifdef DEBUG
            fprintf(stdout, "\n-----------------   GENERATION %d   -----------------\n", generation_num + 1);
            printPopulation(pop);
        #endif
        
        determineFitness(pop);
        steadyStateSelection(pop);
        generation_num++;
    }

    fprintf(stdout, "\nFinal Population:\n");
    printPopulation(pop);

    freeParameters(pop);
    return EXIT_SUCCESS;
}


void initParameters(int argc, char ** argv){
    char * arch_filename = ARCH_FILENAME;
    char * aif_filename = AIF_FILENAME;
    int seed = randSeed();
    int c;

    opterr = 0;

    while((c = getopt(argc, argv, "a:c:d:g:m:p:r:s:")) != -1){
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
            case 'm':
                setMutationRate(atof(optarg));
                break;
            case 'p':
                setPopSize(atoi(optarg));
                break;
            case 'r':
                seed = atoi(optarg);
                break;
            case 's':
                // SELECTION METHOD
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
    
    seedRandGenerator(seed);
    initArchLibrary(arch_filename);
    initNapoleon(aif_filename);

    fprintf(stdout, "Parameters:\n");
    fprintf(stdout, "\tSeed = %d\n\n", seed);
    fprintf(stdout, "\tPopulation Size       = %d\n", getPopSize());
    fprintf(stdout, "\tNumber of Generations = %d\n\n", STOP_CONDITION);
    fprintf(stdout, "\tMutation Rate  = %.4lf\n", getMutationRate());
    fprintf(stdout, "\tCrossover Rate = %.4lf\n\n\t", getCrossoverRate());
}


void freeParameters(Population * pop){
    freePopulation(pop);
    freeArchLibrary();
    freeNapoleon();
}



//// a testing function for B1_10_5.aif
//void bruteForce(void){
//    int solution[10];
//    int bestFitness = 100000;
//    int fitness;
//
//    for(int a = 1; a <= 5; a++){
//        solution[0] = a;
//        for(int b = 1; b <= 2; b++){
//            solution[1] = b;
//            for(int c = 1; c <= 4; c++){
//                solution[2] = c;
//                for(int d = 1; d <= 5; d++){
//                    solution[3] = d;
//                    for(int e = 1; e <= 4; e++){
//                        solution[4] = e;
//                        for(int f = 1; f <= 3; f++){
//                            solution[5] = f;
//                            for(int g = 1; g <= 5; g++){
//                                solution[6] = g;
//                                for(int h = 1; h <= 4; h++){
//                                    solution[7] = h;
//                                    for(int i = 1; i <= 3; i++){
//                                        solution[8] = i;
//                                        for(int j = 1; j <= 4; j++){
//                                            solution[9] = j;
//
//                                            fitness = evaluateFitness(solution);
//
//                                            if(fitness <= bestFitness){
//                                                for(int ind = 0; ind < 10; ind++){
//                                                    printf("%d", solution[ind]);
//                                                }
//                                                //                                                printf("%d%d%d%d%d%d%d%d%d%d", a, b, c, d, e, f, g, h, i, j);
//                                                printf("\t Fitness = %d\n", fitness);
//
//                                                bestFitness = fitness;
//                                            }
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//}


// COMMENTING TEMPLATE
/******************************************************************************
 * NAME : initRandGenerator
 * PURPOSE : 
 * ARGUMENTS : 
 * PRECONDITIONS : 
 * RETURNS : 
 * NOTE : 
 *****************************************************************************/