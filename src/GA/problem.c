/*******************************************************************************
 * FILE NAME : problem.c
 * 
 * Genetic Algorithm for Ahmed Al-Watter
 * 
 * PURPOSE : library for the representation and manipulation of a problem
 *      instance
 * 
 * AUTHOR : Jennifer Winer
 * Email  : jwiner@uoguelph.ca
 * 
 * DATE CREATED : May 7, 2013
 * LAST MODIFIED : May 16, 2013
 ******************************************************************************/

#include "problem.h"

extern char * ARCH_FILENAME;
extern char * DFG_FILENAME;

static DFG * graph;

bool initProblem(void){
    int i;
    
    // FIX - make option to enter your own seed
    randSeed();
    //seedRandGenerator(1368463890);
    
    initArchLibrary(ARCH_FILENAME);
    initDFG(DFG_FILENAME);
    
    template = malloc(sizeof(Representation));
    template->num_genes = graph->num_nodes;
    template->oper = malloc(sizeof(int) * template->num_genes);
    
    for(i=0; i<(template->num_genes); i++){
        template->oper[i] = graph->oper[i];
    }
    
    freeDFG();
    return true;
}



void freeProblem(void){
    freeArchLibrary();
    
    free(template->oper);
    free(template);
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
    char oper_type;
    int num_impl;
    int i, j;
    
    arch_library = malloc(sizeof(Operation) * 4);
    for(i=0; i<4; i++){
        
        // FIX - find a better way to handle newlines in the file
        fscanf(fp, "\n");
        if(fscanf(fp, "%c %d", &oper_type, &num_impl) != 2)
            return false;
        
        (arch_library[i]).num_impl = num_impl;
        (arch_library[i]).impl = malloc(sizeof(Implementation) * (arch_library[i]).num_impl);
        
        for(j=0; j<num_impl; j++){
            if(fscanf(fp, "%lf %lf %lf", &(((arch_library[i]).impl[j]).runtime),
                    &(((arch_library[i]).impl[j]).power),
                    &(((arch_library[i]).impl[j]).area)) != 3){
                return false;
            }
        }
    }
    return true;
}

void freeArchLibrary(void){
    int i;
    
    for(i=0; i<4; i++){
        free((arch_library[i]).impl);
    }
    
    free(arch_library);
}

void printArchLibrary(void){
    char * names[] = {"Addition", "Subtraction", "Multiplication", "Division"};
    int i, j;
    
    for(i=0; i<4; i++){
        fprintf(stdout, "%s:\n", names[i]);
        for(j=0; j<(arch_library[i]).num_impl; j++){
            fprintf(stdout, "%.1lf\t%.1lf\t%.1lf\n", ((arch_library[i]).impl[j]).runtime,
                        ((arch_library[i]).impl[j]).power, ((arch_library[i]).impl[j]).area);
        }
    }
}

/******************************************************************************
 ***********************           DFG FILE I/O         ***********************
 *****************************************************************************/

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
 *****************          RANDOM NUMBER GENERATION          *****************
 *****************************************************************************/

void seedRandGenerator(int seed){
    srand(seed);
}

void randSeed(void){
    int seed;
    
    seed = time(NULL);
    fprintf(stdout, "Seed = %d\n", seed);
    
    srand(seed);
}

double randomNumber(void){
    return rand() / (double) RAND_MAX;
}