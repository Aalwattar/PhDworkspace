#ifndef COMMON_INTERFACES_H
#define COMMON_INTERFACES_H

#define BUFF_SIZE 100

/******************************************************************************
 *****************         ARHITECTURE  DATA STORAGE         ******************
 *****************************************************************************/

// represents one architecture's implementation 
typedef struct{
    char name[BUFF_SIZE];
    
    int columns;
    int rows;

    int config_time; // configuration time
    int config_power; // configuration power
    
    int exec_time; // execution time (AKA latency)
    int exec_power; // execution power (or energy???)
    
    char mode[BUFF_SIZE];  // FIX
} Implementation;


// contains all of the architectures of an task
typedef struct{
    char name[BUFF_SIZE]; // FIX - Make constants
    int id;
    
    int num_impl;           // the number of architectures for this task
    Implementation * impl;  // the properties of each architecture
} Task_Type;


typedef struct{
    char name[BUFF_SIZE];
    char date[BUFF_SIZE];
    
    int num_tasks;
    Task_Type * task;
} Architecture_Library;


/******************************************************************************
 *****************              DFG DATA STORAGE             ******************
 *****************************************************************************/

// FIX - Needs commenting
typedef struct{
    char name[BUFF_SIZE];
    
    int task_type;
    char ** inputs;
    char output[BUFF_SIZE]; // FIX - Make dynamic?
} Node;


// FIX - Needs commenting
typedef struct{
    char name[BUFF_SIZE];
    char date[BUFF_SIZE];
    
    char ** inputs;
    char ** outputs;
    char ** regs;
    int num_regs;
    
    int num_nodes;  // the number of nodes in the DFG
    Node * node;  // the properties of a node
} DFG;


/******************************************************************************
 *****************           HARDWARE DATA STORAGE           ******************
 *****************************************************************************/

// FIX - Needs commenting
typedef struct{
    int num_prrs;
    int num_gpps;
    int * prr_config_time;
    
    char name[BUFF_SIZE];
} Processing_Elelements;


// FIX - Needs commenting
typedef struct{
    char name[BUFF_SIZE];
    char date[BUFF_SIZE];
    
    int num_setups;
    Processing_Elelements * setups;
} Hardware;


/******************************************************************************
 *****************           ENCAPSULATING STRUCT            ******************
 *****************************************************************************/

typedef struct {
    Architecture_Library archlib;
    DFG dfg;
    Processing_Elelements setup;
} Common_Interface;

#endif // COMMON_INTERFACES_H
