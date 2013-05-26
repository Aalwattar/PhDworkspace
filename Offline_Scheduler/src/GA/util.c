/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 7, 2013
 * Modified : May 25, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : problem.c
 * 
 * Purpose  : library for initializing and viewing the architecture library as
 *              well as random number generation
 ******************************************************************************/

#include "util.h"
#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>


#define BUFF_SIZE 500


/******************************************************************************
 **************         PROBLEM DEFINITION DATA STORAGE         ***************
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
    Implementation * impl;      // the Implementation properties of each architecture
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
    return arch_library[opr - 1].num_impl;
}


/******************************************************************************
 *****************          RANDOM NUMBER GENERATION          *****************
 *****************************************************************************/

void seedRandGenerator(int seed) {
    srand(seed);
}

void randSeed(void) {
    int seed;

    seed = time(NULL);
    fprintf(stdout, "Seed = %d\n", seed);

    srand(seed);
}

double randomNumber(void) {
    return rand() / (double) RAND_MAX;
}
