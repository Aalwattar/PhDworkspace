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


/******************************************************************************
 *****************           ARCHITECTURE FILE I/O            *****************
 *****************************************************************************/

/******************************************************************************
 * NAME : initArchLibrary
 * 
 * PURPOSE : Imports the architecture information from a file
 * ARGUMENTS : char * = name of the file containing the architecture data
 * 
 * RETURNS : false if the filename could not be found or opened or the file did 
 *              not follow the specified format
 *           true otherwise (successful completion)
 * 
 * NOTE : please see the README file for more information about the format
 *              and contents of the architecture information file
 *****************************************************************************/
bool initArchLibrary(char *);

/******************************************************************************
 * NAME : freePerformanceInfo
 * 
 * PURPOSE : Frees all dynamically allocated memory used to store the 
 *              problem's architecture information
 * 
 * PRECONDITIONS : This function should only be called if initArchLibrary()
 *                  returned true
 *****************************************************************************/
void freeArchLibrary(void);

/******************************************************************************
 * NAME : printArchLibrary
 * 
 * PURPOSE : print the library of architectures
 * 
 * PRECONDITIONS : the architecture library MUST have been previously  
 *                      initialized by initProblem()
 * 
 * NOTE : this function exists for testing and debugging purposes only.
 *****************************************************************************/
void printArchLibrary(void);




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

// FIX - make 4 into NUM_OPS???

bool parseArchLibrary(FILE * fp) {
    char buffer[BUFF_SIZE];
    char * tokens;
    int operation_type;
    int i, j;

    arch_library = malloc(sizeof (Operation) * 4);
    for (i = 0; i < 4; i++) {
        arch_library->impl = NULL;
        arch_library->num_impl = 0;
    }

    while (fgets(buffer, BUFF_SIZE, fp) != NULL) {
        if (strlen(BUFF_SIZE) < 2)
            continue;

        // FIX - implement some form of file validation???
        if (buffer[0] == '#')
            continue;

        if (strncmp(buffer, "TASK", 4) == 0) {
            operation_type = buffer[4] - '0';

            (arch_library[i]).num_impl++;
            (arch_library[i]).impl = malloc(sizeof (Implementation) * (arch_library[i]).num_impl);
        }
    }
    for (i = 0; i < 4; i++) {

        // FIX - find a better way to handle newlines in the file
        fscanf(fp, "\n");
        if (fscanf(fp, "%c %d", &oper_type, &num_impl) != 2)
            return false;

        (arch_library[i]).num_impl = num_impl;
        (arch_library[i]).impl = malloc(sizeof (Implementation) * (arch_library[i]).num_impl);

        for (j = 0; j < num_impl; j++) {
            if (fscanf(fp, "%lf %lf %lf", &(((arch_library[i]).impl[j]).runtime),
                    &(((arch_library[i]).impl[j]).power),
                    &(((arch_library[i]).impl[j]).area)) != 3) {
                return false;
            }
        }
    }
    return true;
}

void freeArchLibrary(void) {
    int i;

    for (i = 0; i < 4; i++) {
        free((arch_library[i]).impl);
    }

    free(arch_library);
}

void printArchLibrary(void) {
    char * names[] = {"Addition", "Subtraction", "Multiplication", "Division"};
    int i, j;

    for (i = 0; i < 4; i++) {
        fprintf(stdout, "%s:\n", names[i]);
        for (j = 0; j < (arch_library[i]).num_impl; j++) {
            fprintf(stdout, "%.1lf\t%.1lf\t%.1lf\n", ((arch_library[i]).impl[j]).runtime,
                    ((arch_library[i]).impl[j]).power, ((arch_library[i]).impl[j]).area);
        }
    }
}