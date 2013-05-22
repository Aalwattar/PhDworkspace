#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>


/******************************************************************************
 ***********************           DFG FILE I/O         ***********************
 *****************************************************************************/

/******************************************************************************
 * NAME : initDFG
 * 
 * PURPOSE : Imports a DFG from a template file
 * ARGUMENTS : char * = the name of the file that contains the DFG
 * 
 * RETURNS : false if the filename could not be found or opened or the file did 
 *              not follow the specified format
 *           true otherwise (successful completion)
 * 
 * NOTE : please see the README file for more information about the format
 *              and contents of the DFG file
 *****************************************************************************/
bool initDFG(char *);

/******************************************************************************
 * NAME : freeDFG
 * 
 * PURPOSE : Frees all static dynamically allocated memory for the 
                representation of the DFG
 * 
 * PRECONDITIONS : the DFG struct MUST have been previously initialized 
 *                      by initProblem
 *****************************************************************************/
void freeDFG(void);


int getNextOperation(FILE *);

// FIX - could use some more error checking
bool initDFG(char * filename){
    FILE * fp;
    int num_nodes;
    int i;
    
    if(filename == NULL)
        return false;
    
    fp = fopen(filename, "r");
    if(fp == NULL){
        fprintf(stderr, "Unable to find or open file %s\n", filename);
        return false;
    }
    
    fscanf(fp, "%d", &num_nodes);
    graph = malloc(sizeof(DFG));
    graph->num_nodes = num_nodes;
    graph->oper = malloc(sizeof(int) * num_nodes);
    
    for(i=0; i<num_nodes; i++){
        graph->oper[i] = getNextOperation(fp);
    }
    
    fclose(fp);
    return true;
}

// FIX
int getNextOperation(FILE * fp){
    char buffer[20];
    
    fscanf(fp, "%20s", buffer);
    
    if(strcmp("1", buffer) == 0){
        return a;
    }
    if(strcmp("2", buffer) == 0){
        return s;
    }
    if(strcmp("3", buffer) == 0){
        return m;
    }
    return d;
}

void freeDFG(void){
    free(graph->oper);
    free(graph);
}