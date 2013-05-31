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

int STOP_CONDITION = 500;
int generation_num = 0;


void initParameters(int num_tokens, char ** input_token);
bool populationConverged(Population * pop);
void freeParameters(Population *);

void bruteForce();


int main(int argc, char * argv[]) {
    Population * pop, * next_generation;
    int i, j;
    
    initParameters(argc, argv);
    pop = genRandPopulation();
    determineFitness(pop);

    fprintf(stdout, "\n----------------------------------------------------------\n\n");
    fprintf(stdout, "Starting Population:\n");
    printPopulation(pop);

    while (generation_num < STOP_CONDITION) {
        determineFitness(pop);
        
        #ifdef __DEBUG
            fprintf(stdout, "\n-----------------   GENERATION %d   -----------------\n", generation_num + 1);
            printPopulation(pop);
        #endif
        
        next_generation = tournamentSelection(pop);
        pop = next_generation;
        
        generation_num++;
    }

    fprintf(stdout, "\nFinal Population:\n");
    printPopulation(pop);

    freeParameters(pop);
    return EXIT_SUCCESS;
}



void initParameters(int num_tokens, char ** input_token) {
    int seed = randSeed();
    char * arch_filename = ARCH_FILENAME;
    char * aif_filename = AIF_FILENAME;
    int i;

    //    FIX - still missing the following
    //    5. selection method
    //    6. crossover type
    //    7. mutation type

    // FIX - make more robust
    for (i = 0; i < num_tokens; i++) {
        if (strncmp(input_token[i], "-ps=", 4) == 0) {
            setPopSize(&(input_token[i][4]));
        }
        if (strncmp(input_token[i], "-mr=", 4) == 0) {
            setMutationRate(&(input_token[i][4]));
        }
        if (strncmp(input_token[i], "-cr=", 4) == 0) {
            setCrossoverRate(&(input_token[i][4]));
        }
        if (strncmp(input_token[i], "-g=", 3) == 0) {
            STOP_CONDITION = atoi(&(input_token[i][3]));
        }
        if (strncmp(input_token[i], "-arch=", 6) == 0) {
            arch_filename = &(input_token[i][6]);
        }
        if (strncmp(input_token[i], "-aif=", 5) == 0) {
            aif_filename = &(input_token[i][5]);
        }
        if (strncmp(input_token[i], "-seed=", 6) == 0) {
            seed = atoi(&(input_token[i][6]));
        }
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
    


// a testing function for B1_10_5.aif
void bruteForce(void){
    int solution[10];
    int bestFitness = 100000;
    int fitness;
    
    for(int a=1; a<=5; a++){
        solution[0] = a;
        for(int b=1; b<=2; b++){
            solution[1] = b;
            for(int c=1; c<=4; c++){
                solution[2] = c;
                for(int d=1; d<=5; d++){
                    solution[3] = d;
                    for(int e=1; e<=4; e++){
                        solution[4] = e;
                        for(int f=1; f<=3; f++){
                            solution[5] = f;
                            for(int g=1; g<=5; g++){
                                solution[6] = g;
                                for(int h=1; h<=4; h++){
                                    solution[7] = h;
                                    for(int i=1; i<=3; i++){
                                        solution[8] = i;
                                        for(int j=1; j<=4; j++){
                                            solution[9] = j;
                                            
                                            fitness = evaluateFitness(solution);
                                            
                                            if(fitness <= bestFitness){
                                                for(int ind=0; ind<10; ind++){
                                                    printf("%d", solution[ind]);
                                                }
//                                                printf("%d%d%d%d%d%d%d%d%d%d", a, b, c, d, e, f, g, h, i, j);
                                                printf("\t Fitness = %d\n", fitness);
                                                
                                                bestFitness = fitness;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
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