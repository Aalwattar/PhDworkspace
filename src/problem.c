/*******************************************************************************
 * FILE NAME : problem.c
 * 
 * Genetic Algorithm practice for Ahmed Al-Watter
 * 
 * PURPOSE : library for the representation and manipulation of a problem
 *      instance
 * 
 * AUTHOR : Jennifer Winer
 * Email  : jwiner@uoguelph.ca
 * 
 * DATE CREATED : May 7, 2013
 * LAST MODIFIED : May 8, 2013
 ******************************************************************************/

#include "problem.h"

/******************************************************************************
 *****************          RANDOM NUMBER GENERATION          *****************
 *****************************************************************************/

void seedRandGenerator(int seed){
    srand(seed);
}

void randSeed(void){
    int seed;
    
    seed = time(NULL);
    fprintf(stdout, "Seed: %d\n", seed);
    
    srand(seed);
}

double randomNumber(void){
    return rand() / (double) RAND_MAX;
}

/******************************************************************************
 *****************            ARCHITECTURE FILE I/O           *****************
 *****************************************************************************/

bool parseArchLibrary(FILE * fp);

bool initArchLibrary(char * filename){
    FILE * fp;
    bool status = true;
    
    if(filename == NULL)
        return false;
    
    fp = fopen(filename, "r");
    if(fp == NULL){
        fprintf(stderr, "Unable to find or open file %s\n", filename);
        return false;
    }
    
    if(parseArchLibrary(fp) == false)
        status = false;
    
    fclose(fp);
    
    return status;
}

/******************************************************************************
 * NAME : parseArchLibrary
 * 
 * PURPOSE : initializes the architecture library with information from the file
 *              pointer provided by initArchLibrary()
 * ARGUMENTS : FILE * = a file pointer to a file that contains the 
 *              architecture information for each operation
 * 
 * PRECONDITIONS : This is a helper function that should only ever be called
 *                      from initArchLibrary()
 * 
 * RETURNS : false if the file did not follow the specified format
 *           true otherwise (successful completion)
 *****************************************************************************/

// FIX - CREATES MEMORY LEAKS IF THE FILE IS NOT FORMATTED CORRECTLY
bool parseArchLibrary(FILE * fp){
    char op_type;
    int num_impl;
    int i, j;
    
    operation = malloc(sizeof(Operation) * 4);
    for(i=0; i<4; i++){
        
        // FIX - find a better way to handle newlines in the file
        fscanf(fp, "\n");
        if(fscanf(fp, "%c %d", &op_type, &num_impl) != 2)
            return false;
        
        (operation[i]).num_arch = num_impl;
        (operation[i]).arch = malloc(sizeof(Architecture) * (operation[i]).num_arch);
        
        for(j=0; j<num_impl; j++){
            if(fscanf(fp, "%lf %lf %lf", &(((operation[i]).arch[j]).runtime),
                    &(((operation[i]).arch[j]).power),
                    &(((operation[i]).arch[j]).area)) != 3){
                return false;
            }
        }
    }
    return true;
}

void freeArchLibrary(){
    int i;
    
    for(i=0; i<4; i++){
        free((operation[i]).arch);
    }
    
    free(operation);
}

/******************************************************************************
 *****************             TESTING FUNCTIONS              *****************
 *****************************************************************************/

void printArchLibrary(){
    char * names[] = {"Addition", "Subtraction", "Multiplication", "Division"};
    int i, j;
    
    for(i=0; i<4; i++){
        fprintf(stdout, "%s:\n", names[i]);
        for(j=0; j<(operation[i]).num_arch; j++){
            fprintf(stdout, "%.1lf\t%.1lf\t%.1lf\n", ((operation[i]).arch[j]).runtime,
                        ((operation[i]).arch[j]).power, ((operation[i]).arch[j]).area);
        }
    }
}