/*************************************************************************
 * Author   : Ziad Abuowaimer
 * Modifier : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 * 
 * Created  : May 22, 2013
 * Modified : July 3, 2013
 *************************************************************************/

/*************************************************************************/
// Filename: main.c
// Description:
// This file is the top level file which integrates all the Scheduler tasks.
/*************************************************************************/

//global header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

//local header files
#include <bounds.h>
#include <types.h>
#include <ecodes.h>
#include <napoleon.h>
#include <io.h>
#include <ilp.h>
#include <functions.h>
#include <main.h>
#include <stdbool.h>
#include <confuse.h>    // WARNING - NON STANDARD LIBRARY

#include "DFG_library.h"

FILE *log_strm;
void print_help(void);
int parse_cmd_line_opts(int, char**, t_config*);

DFG * initDFG(char *);
void freeDFG(DFG);
void printDFG(DFG);


int main(int argc, char *argv[]){
    DFG dfg;
    FILE *aif_strm, *res_strm, *ilp_strm, *grid_strm, *impl_strm;
    t_task_interface *task_interface;
    t_task *task;
    t_task_type *task_type;
    t_task_impl *task_impl;
    t_config config;
    int err = 0;
    int *succ_adj_mat;
    int *reuse_mat; //aij
    int T = 99; //upper_bound_total_exec_time
    int i = 0, j = 0;
    int random_num;

    if(argc > 1){
        if((err = parse_cmd_line_opts(argc, argv, &config))){
            print_help();
            fprintf(stderr, "Error Code: %d\n", err);
            exit(err);
        }
    }else{
        print_help();
        fprintf(stderr, "Error Code: %d\n", __LESSARGS);
        exit(__LESSARGS);
    }

    //if(!(log_strm=fopen(config.log_fname,"w")))
    //  print_error(__LOG_FILE);

    //allocate memory for the tasks and the task interfaces
    task = (t_task*)malloc(sizeof (t_task) * __NUM_MAX_TASKS);
    for(i = 0; i < __NUM_MAX_TASKS; i++){
        
        (task + i)->type = 0;
        (task + i)->exec_sched = 0;
        (task + i)->reconfig_sched = 0;
        (task + i)->leftmost_column = 0;
        (task + i)->bottommost_row = 0;
        (task + i)->latency = 0;
        (task + i)->columns = 0;
        (task + i)->width = 0;
        (task + i)->input1 = 0;
        (task + i)->input2 = 0;
        (task + i)->output = 0;
        (task + i)->impl = 0;
        (task + i)->reconfig_pwr = 0;
        (task + i)->exec_pwr = 0;
    }

    task_interface = (t_task_interface*)malloc(sizeof (t_task_interface) * __NUM_MAX_TASK_INTFC);
    for(i = 0; i < __NUM_MAX_TASK_INTFC; i++){
        (task_interface + i)->mode = 0;
        (task_interface + i)->width = 0;
        (task_interface + i)->reg_out = 0;
    }


    // MODIFIED BY: Jennifer Winer 
    dfg = initDFG(aif_strm);
    
    // my file parsing function opens the file instead
    if((err = parse_aif(dfg, task, task_interface)))
        print_error(err);
    
    freeDFG();

    //allocate memory for resources
    task_type = (t_task_type*)malloc(sizeof (t_task_type) * __NUM_MAX_TASK_TYPES);
    for(i = 0; i < __NUM_MAX_TASK_TYPES; i++){
        (task_type + i)->latency = 0;
        (task_type + i)->reconfig_time = 0;
        (task_type + i)->columns = 0;
        (task_type + i)->rows = 0;
    }

    //open the resource file for reading
    if((res_strm = fopen(config.res_fname, "r"))){
        //parse the resource file
        //and exit on unsuccessful execution of the parse_res function
        if((err = parse_res(res_strm, task_type)))
            print_error(err);
        fclose(res_strm);
    }
    //assert(res_strm);

    task_impl = (t_task_impl*)malloc(sizeof (t_task_impl) * __NUM_MAX_TASK_IMPLS);
    for(i = 0; i < __NUM_MAX_TASK_IMPLS; i++){
        (task_impl + i)->id = 0;
        (task_impl + i)->type = 0;
        (task_impl + i)->impl = 0;
        (task_impl + i)->latency = 0;
        (task_impl + i)->reconfig_time = 0;
        (task_impl + i)->columns = 0;
        (task_impl + i)->rows = 0;
        (task_impl + i)->reconfig_pwr = 0;
        (task_impl + i)->exec_pwr = 0;
    }

    //open the resource file for reading
    if((impl_strm = fopen("input/arch_lib.txt", "r"))){
        //parse the impl file
        //and exit on unsuccessful execution of the parse_impl function
        if((err = parse_impl(impl_strm, task_impl)))
            print_error(err);
        fclose(impl_strm);
    }

#ifdef __DEBUG
    //display the task types info
    display_task_type(task_type);
#endif

    set_task_parameter(task, task_type);

    display_task_impl(task_impl);

    //allocate memory for the successor graph adjacency matrix
    succ_adj_mat = (int*)malloc(sizeof ( int)*(task->width + 2)*(task->width + 2));

    //create the successor matrix
    //and exit on unsuccessful execution of the parse_aif function
    if((err = create_graph(task, task_interface, succ_adj_mat)))
        print_error(err);

    //allocate memory for the reuse matrix
    reuse_mat = (int*)malloc(sizeof ( int)*(task->width + 2)*(task->width + 2));

    //create the reuse matrix
    if((err = create_reuse_mat(task, reuse_mat)))
        print_error(err);

    fprintf(stderr, "T = %d\n", calc_T(task, &T));
    //T = 20;

    if(!(grid_strm = fopen("outScheduler.txt", "w")))
        print_error(__LOG_FILE);

    for(j = 0; j < 20; j++){
        for(i = 1; i <= task->width; i++){
            switch((task + i)->type){
                case __ADD:
                {
                    srand(time(NULL) * i * (j + 1));
                    //srand(time(NULL)*rand());
                    random_num = (rand() % 2) + 1;
                    (task + i)->impl = (task_impl + random_num)->impl;
                    (task + i)->latency = (task_impl + random_num)->latency;
                    (task + i)->reconfig_time = (task_impl + random_num)->reconfig_time;
                    (task + i)->columns = (task_impl + random_num)->columns;
                    (task + i)->rows = (task_impl + random_num)->rows;
                    (task + i)->reconfig_pwr = (task_impl + random_num)->reconfig_pwr;
                    (task + i)->exec_pwr = (task_impl + random_num)->exec_pwr;
                    break;
                }
                case __MULT:
                {
                    srand(time(NULL) * i * (j + 1));
                    //srand(time(NULL)*rand());
                    random_num = (rand() % 3) + 4;
                    (task + i)->impl = (task_impl + random_num)->impl;
                    (task + i)->latency = (task_impl + random_num)->latency;
                    (task + i)->reconfig_time = (task_impl + random_num)->reconfig_time;
                    (task + i)->columns = (task_impl + random_num)->columns;
                    (task + i)->rows = (task_impl + random_num)->rows;
                    (task + i)->reconfig_pwr = (task_impl + random_num)->reconfig_pwr;
                    (task + i)->exec_pwr = (task_impl + random_num)->exec_pwr;
                    break;
                }
                case __SUB:
                {
                    srand(time(NULL) * i * (j + 1));
                    //srand(time(NULL)*rand());
                    random_num = (rand() % 4) + 8;
                    (task + i)->impl = (task_impl + random_num)->impl;
                    (task + i)->latency = (task_impl + random_num)->latency;
                    (task + i)->reconfig_time = (task_impl + random_num)->reconfig_time;
                    (task + i)->columns = (task_impl + random_num)->columns;
                    (task + i)->rows = (task_impl + random_num)->rows;
                    (task + i)->reconfig_pwr = (task_impl + random_num)->reconfig_pwr;
                    (task + i)->exec_pwr = (task_impl + random_num)->exec_pwr;
                    break;
                }
                case __DIV:
                {
                    srand(time(NULL) * i * (j + 1));
                    //srand(time(NULL)*rand());
                    random_num = (rand() % 1) + 13;
                    (task + i)->impl = (task_impl + random_num)->impl;
                    (task + i)->latency = (task_impl + random_num)->latency;
                    (task + i)->reconfig_time = (task_impl + random_num)->reconfig_time;
                    (task + i)->columns = (task_impl + random_num)->columns;
                    (task + i)->rows = (task_impl + random_num)->rows;
                    (task + i)->reconfig_pwr = (task_impl + random_num)->reconfig_pwr;
                    (task + i)->exec_pwr = (task_impl + random_num)->exec_pwr;
                    break;
                }
            }
            (task + i)->exec_sched = 0;
            (task + i)->reconfig_sched = 0;
            (task + i)->leftmost_column = 0;
            (task + i)->bottommost_row = 0;
            //(task+i)->reconfig_pwr = 0;
            //(task+i)->exec_pwr = 0;


        }
        
        display_task(task, task_interface);

        //call the napoleon scheduler
        Napoleon(grid_strm, succ_adj_mat, task->width, task);
    }
    if(!(ilp_strm = fopen("ilp_equations.lp", "w")))
        print_error(__LOG_FILE);

    //uncomment the next line to generate the ILP equations file.
    //ilp_equations(ilp_strm, task, T, succ_adj_mat, reuse_mat);

    fclose(ilp_strm);
    fclose(grid_strm);

    free(reuse_mat);
    free(succ_adj_mat);
    free(task_type);
    free(task_interface);
    free(task);

    exit(0);
}


void print_help(void){
    fprintf(stderr, "hyperspace <options>\n");
    fprintf(stderr, "<options>: -aif <aif file>\n");
    fprintf(stderr, "           -res_file <resource constraints file>\n");
    fprintf(stderr, "          [-log_file <log file>]\n");
    fprintf(stderr, "          [-options_file <options file>]\n");
}


int parse_cmd_line_opts(int argc, char *argv[], t_config *config){
    int err = 0;
    int count = 1;

    strcpy(config->log_fname, "hs.log");

    while(count < argc){
        if(!strcasecmp(*(argv + count), "-aif")){
            char *aif_fname;
            if((aif_fname = *(argv + count + 1)))
                strcpy(config->aif_fname, aif_fname);
            else
                err = __AIF_FILE;
            count++;
        }else if(!strcasecmp(*(argv + count), "-log_file")){
            char *log_fname;
            if ((log_fname = *(argv + count + 1)))
                strcpy(config->log_fname, log_fname);
            else
                err = __LOG_FILE;
            count++;
        } else if ((!strcasecmp(*(argv + count), "-res_file"))) {
            char * res_fname;
            if ((res_fname = *(argv + count + 1)))
                strcpy(config->res_fname, res_fname);
            else
                err = __RES_FILE;
            count++;
        } else if ((!strcasecmp(*(argv + count), "-options_file"))) {
            char *opt_fname;
            if ((opt_fname = *(argv + count + 1)))
                strcpy(config->opt_fname, opt_fname);
            else
                err = __OPT_FILE;
            count++;
        }else{
            err = __UNRECOPT;
        }

        count++;
    }

    return err;
}


/******************   Jenn's File Parsing   *****************/

int conf_validate_value(cfg_t *cfg, cfg_opt_t *opt){
    int value = cfg_opt_getnint(opt, 0);
    
    if (value <= 0) {
        cfg_error(cfg, "Integer option[%d] '%s' must be positive in section '%s'",
        		value,cfg_opt_name(opt) ,cfg_name(cfg));
        return -1;
    }
    
    return 0;
}

// FIX - Needs a function header comment
cfg_t * parse_conf_dfg(char *filename){
    cfg_opt_t task_opts[] = {
        CFG_INT("type", 0, CFGF_NODEFAULT ),
        CFG_STR("output",0 , CFGF_NODEFAULT),
        CFG_STR_LIST("inputs", 0, CFGF_NODEFAULT),
        CFG_END()
    };

    cfg_opt_t opts[] = {
        CFG_SEC("task", task_opts, CFGF_MULTI | CFGF_TITLE | CFGF_NODEFAULT | CFGF_NO_TITLE_DUPES),
        CFG_STR("Name", "Unknown DFG file!", CFGF_NONE),
        CFG_STR("Date", "Unknown Creation Date", CFGF_NONE),
        
        CFG_STR_LIST("inputs", 0, CFGF_NODEFAULT),
        CFG_STR_LIST("outputs", 0, CFGF_NODEFAULT),
        CFG_STR_LIST("regs", 0, CFGF_NODEFAULT),
        CFG_END()
    };

    cfg_t * cfg = cfg_init(opts, CFGF_NODEFAULT);
    // FIX - I have no idea what to validate in the DFG files!
        // FIX - validate that each node's input and ouput is one of the options listed at the top
        // FIX - validate that each node has exactly two inputs and only one output?
        // FIX - validate that each node's output is another node's input etc. 
    // cfg_set_validate_func(cfg, "inputs", conf_validate_processor);

    switch (cfg_parse(cfg, filename)) {
        case CFG_SUCCESS:
            break;
        
        case CFG_FILE_ERROR:
            // FIX - Make this error message better (or terminate / return -1 if the file could not be correctly read)
            printf("warning: configuration file '%s' could not be read: %s\n", 
                    filename, strerror(errno));
            break;
            
        case CFG_PARSE_ERROR:
            return NULL;
    }

    return cfg;
}


DFG initDFG(char * filename){
    DFG task_schedule;
    cfg_t * cfgDFG, * node;
    int i, j;
    
    if(filename == NULL)
        return NULL;
    
    cfgDFG = parse_conf_dfg(filename);
    if(cfgDFG == NULL)
        return NULL;       // FAILED TO PARSE THE FILE!

    // FIX - Change the strncpy size to a #define static thing
    strncpy(task_schedule.name, cfg_getstr(cfgDFG, "Name"), BUFF_SIZE);
    strncpy(task_schedule.date, cfg_getstr(cfgDFG, "Date"), BUFF_SIZE);
    
    // read in all the inputs
    task_schedule.inputs = malloc(sizeof(char *) * (cfg_size(cfgDFG, "inputs") + 1));
    for(i=0; i < cfg_size(cfgDFG, "inputs"); i++){
        task_schedule.inputs[i] = malloc(sizeof(char) * (strlen(cfg_getnstr(cfgDFG, "inputs", i)) + 1));
        strcpy(task_schedule.inputs[i], cfg_getnstr(cfgDFG, "inputs", i));
    }
    task_schedule.inputs[i] = NULL;
    
    // read in all the outputs
    task_schedule.outputs = malloc(sizeof(char *) * (cfg_size(cfgDFG, "outputs") + 1));
    for(i=0; i < cfg_size(cfgDFG, "outputs"); i++){
        task_schedule.outputs[i] = malloc(sizeof(char) * (strlen(cfg_getnstr(cfgDFG, "outputs", i)) + 1));
        strcpy(task_schedule.outputs[i], cfg_getnstr(cfgDFG, "outputs", i));
    }
    task_schedule.outputs[i] = NULL;
        
    // read in all the registers
    task_schedule.regs = malloc(sizeof(char *) * (cfg_size(cfgDFG, "regs") + 1));
    for(i=0; i < cfg_size(cfgDFG, "regs"); i++){
        task_schedule.regs[i] = malloc(sizeof(char) * (strlen(cfg_getnstr(cfgDFG, "regs", i)) + 1));
        strcpy(task_schedule.regs[i], cfg_getnstr(cfgDFG, "regs", i));
    }
    task_schedule.regs[i] = NULL;
    
    // Read in all the nodes
    task_schedule.num_nodes = cfg_size(cfgDFG, "task");
    task_schedule.node = malloc(sizeof(Node) * task_schedule.num_nodes);
    for(j=0; j < cfg_size(cfgDFG, "task"); j++){
        node = cfg_getnsec(cfgDFG, "task", j);
        
        strncpy(task_schedule.node[j].name, cfg_title(node), BUFF_SIZE);
        task_schedule.node[j].task_type = cfg_getint(node, "type");
        strncpy(task_schedule.node[j].output, cfg_getstr(node, "output"), BUFF_SIZE);
        
        // read in all the inputs
        task_schedule.node[j].inputs = malloc(sizeof(char *) * (cfg_size(node, "inputs") + 1));
        for(i=0; i < cfg_size(node, "inputs"); i++){
            task_schedule.node[j].inputs[i] = malloc(sizeof(char) * (strlen(cfg_getnstr(node, "inputs", i)) + 1));
            strcpy(task_schedule.node[j].inputs[i], cfg_getnstr(node, "inputs", i));
        }
        task_schedule.node[j].inputs[i] = NULL;
    }
    
    cfg_free(cfgDFG);
    return task_schedule;
}

void freeDFG(DFG * task_schedule){
    int i, j;

    for(i = 0; i < task_schedule->num_nodes; i++){
        for(j=0; task_schedule->node[i].inputs[j]!= NULL; j++)
            free(task_schedule->node[i].inputs[j]);
        
        free(task_schedule->node[i].inputs);
    }
        
    free(task_schedule->node);
    
    for(i=0; task_schedule->inputs[i] != NULL; i++)
        free(task_schedule->inputs[i]);
    free(task_schedule->inputs);
    
    for(i=0; task_schedule->outputs[i] != NULL; i++)
        free(task_schedule->outputs[i]);
    free(task_schedule->outputs);
    
    for(i=0; task_schedule->regs[i] != NULL; i++)
        free(task_schedule->regs[i]);
    free(task_schedule->regs);
}

void printDFG(DFG * task_schedule){
    int i, j;
    
    fprintf(stdout, "Name = %s\n", task_schedule->name);
    fprintf(stdout, "Date = %s\n\n", task_schedule->date);

    fprintf(stdout, "Inputs = { ");
    for(i=0; task_schedule->inputs[i] != NULL; i++)
        fprintf(stdout, "%s, ", task_schedule->inputs[i]);
    fprintf(stdout, "}\n");
    
    fprintf(stdout, "Outputs = { ");
    for(i=0; task_schedule->outputs[i] != NULL; i++)
        fprintf(stdout, "%s, ", task_schedule->outputs[i]);
    fprintf(stdout, "}\n");
    
    fprintf(stdout, "regs = { ");
    for(i=0; task_schedule->regs[i] != NULL; i++)
        fprintf(stdout, "%s, ", task_schedule->regs[i]);
    fprintf(stdout, "}\n");
    
    fprintf(stdout, "\nNODES:\n");
    for(i = 0; i < task_schedule->num_nodes; i++){
        fprintf(stdout, "\tName = %s\n", task_schedule->node[i].name);
        
        fprintf(stdout, "\tInputs = { ");
        for(j=0; task_schedule->node[i].inputs[j]!= NULL; j++)
            fprintf(stdout, "%s, ", task_schedule->node[i].inputs[j]);
        fprintf(stdout, "}\n");
        
        fprintf(stdout, "\tOutput = %s\n", task_schedule->node[i].output);
        fprintf(stdout, "\tType = %d\n\n", task_schedule->node[i].task_type);
    }
}