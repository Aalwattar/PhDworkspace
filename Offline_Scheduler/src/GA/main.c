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
#include <stdbool.h>


#include <bounds.h>
#include <types.h>
#include <ecodes.h>
#include <napoleon.h>
#include <io.h>
#include <ilp.h>
#include <functions.h>
#include <main.h>

static char * ARCH_FILENAME = "input/architecture_library.txt";
static char * DFG_FILENAME = "input/DFG.txt";

static char * AIF_FILENAME = "input/B1_25_5.aif";

int STOP_CONDITION = 500;
int generation_num;


void initParameters(int num_tokens, char ** input_token);
bool populationConverged(Population * pop);



int main(int argc, char * argv[]) {
    Population * pop, * mating_pop;
    int i, j;

    // FIX - make option to enter your own seed
    randSeed();
    //seedRandGenerator(1369671822);
    
    initParameters(argc, argv);
    initArchLibrary(ARCH_FILENAME);
    initNapoleon(AIF_FILENAME);
    
    pop = genRandPopulation();

    fprintf(stdout, "\n----------------------------------------------------------\n\n");
    fprintf(stdout, "Starting Population:\n");
    for (i = 0; i < getPopSize(); i++) {
        for (j = 0; j < getNumGenes(); j++) {
            fprintf(stdout, "%d", pop->member[i].encoding[j]);
        }
        fprintf(stdout, "\n");
    }

    while (generation_num < STOP_CONDITION) {
        for (i = 0; i < getPopSize(); i++) {
//            fprintf(stdout, "\n%2d)\n", i+1);
            pop->member[i].fitness = evaluateFitness(pop->member[i].encoding);
        }

//        fprintf(stdout, "----------GENERATION %d-----------------\n\n", generation_num);
//        for (i = 0; i < getPopSize(); i++) {
//            for (j = 0; j < getNumGenes(); j++) {
//                fprintf(stdout, "%d", pop->member[i].encoding[j]);
//            }
//            fprintf(stdout, "\tfitness = %d\n", pop->member[i].fitness);
//        }

        mating_pop = tournamentSelection(pop);
        freePopulation(pop);

        generateNextGeneration(mating_pop);
        pop = mating_pop;

        generation_num++;
    }

    //fprintf(stdout, "\nGenerations to create best solution = %d\n", generation_num);
    fprintf(stdout, "\nFinal Population:\n");
    for (i = 0; i < getPopSize(); i++) {
        for (j = 0; j < getNumGenes(); j++) {
            fprintf(stdout, "%d", pop->member[i].encoding[j]);
        }
        fprintf(stdout, "\tfitness = %d\n", evaluateFitness(pop->member[i].encoding));
    }

    freePopulation(pop);
    freeArchLibrary();
    freeNapoleon();
    
    return EXIT_SUCCESS;
}



void initParameters(int num_tokens, char ** input_token) {
    int i;

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
            ARCH_FILENAME = &(input_token[i][6]);
        }
        if (strncmp(input_token[i], "-dfg=", 5) == 0) {
            DFG_FILENAME = &(input_token[i][5]);
        }
        if (strncmp(input_token[i], "-aif=", 5) == 0) {
            AIF_FILENAME = &(input_token[i][5]);
        }

        if (strncmp(input_token[i], "-seed=", 6) == 0) {
            seedRandGenerator(atoi(&(input_token[i][6])));
        }
    }

    fprintf(stdout, "Parameters:\n\n");
    fprintf(stdout, "\tPopulation Size       = %d\n", getPopSize());
    fprintf(stdout, "\tNumber of Generations = %d\n\n", STOP_CONDITION);
    fprintf(stdout, "\tMutation Rate  = %.4lf\n", getMutationRate());
    fprintf(stdout, "\tCrossover Rate = %.4lf\n\n\t", getCrossoverRate());
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



//void bruteForce(void){
//    int solution[10];
//    int bestFitness = 100000;
//    int fitness;
//    
//    for(int a=0; a<5; a++){
//        solution[0] = a;
//        for(int b=0; b<2; b++){
//            solution[1] = b;
//            for(int c=0; c<4; c++){
//                solution[2] = c;
//                for(int d=0; d<5; d++){
//                    solution[3] = d;
//                    for(int e=0; e<4; e++){
//                        solution[4] = e;
//                        for(int f=0; f<3; f++){
//                            solution[5] = f;
//                            for(int g=0; g<5; g++){
//                                solution[6] = g;
//                                for(int h=0; h<4; h++){
//                                    solution[7] = h;
//                                    for(int i=0; i<3; i++){
//                                        solution[8] = i;
//                                        for(int j=0; j<4; j++){
//                                            solution[9] = j;
//                                            
//                                            fitness = evaluateFitness(solution);
//                                            
//                                            if(fitness <= bestFitness){
//                                                for(int ind=0; ind<10; ind++){
//                                                    printf("%d", solution[ind]);
//                                                }
////                                                printf("%d%d%d%d%d%d%d%d%d%d", a, b, c, d, e, f, g, h, i, j);
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