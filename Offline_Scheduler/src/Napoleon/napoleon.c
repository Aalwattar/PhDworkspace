/*************************************************************************
 * Author   : Ziad Abuowaimer
 * Modifier : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 * 
 * Created  : May 22, 2013
 * Modified : May 28, 2013
 *************************************************************************/

//global header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>

//local header files
#include <bounds.h>
#include <types.h>
#include <ecodes.h>
#include <napoleon.h>
#include <functions.h>
#include <main.h>
#include <io.h>
#include "fitness.h"


int evaluate_ASAP(int, int *, int *, t_task *);
int evaluate_ALAP(int, int *, int *, int, t_task *);
int opqueue(int, int *, int);
int sortbyALAP(int *, int *, int, int *);
int findAvailableTasks(int *, int, int *, int *, int *);
bool findFarthestCompatibleModule(int, int, int *, t_task *);
int schedule(int, int, bool, int *, t_task *, int *);
bool findFarthestAvailableSpace(int, int, t_task *);
bool overlap(int, int, t_task *);
int grid[__NUM_FPGA_COLUMNS][__NUM_FPGA_ROWS];
int grid_config[__NUM_FPGA_COLUMNS][__NUM_FPGA_ROWS];


/************************************************
Function name: sort

inputs:
      int *val_ALAP_u
      int num_nodes
outputs:
      int *p_list

This sort algorithm implements the INSERTION sort technique. 
Sorting is done in NON_DECREASING order of two keys: 
  a. ALAP value(primary)
  

    *(val_ALAP_u+i) has the ALAP value
    
************************************************/

int sortbyALAP(int *val_ALAP_u,
               int *val_ASAP_u,
               int num_nodes,
               int *p_list){
    int err = 0;
    int held_ALAP = 0, held_u = 0, held_node = 0;
    int candidate_ALAP = 0, candidate_u = 0;
    int i = 0, j = 0, swap_flag = 0;
    int *val_ALAP_u_copy, *val_u;

    //Make a copy of the ALAP and mobility list before sorting because
    //the order changes when the sorting finishes
    val_ALAP_u_copy = (int *)malloc(sizeof ( int) * (num_nodes + 2));
    val_u = (int *)malloc(sizeof ( int)* (num_nodes + 2));

    for(i = 0; i <= num_nodes; i++){
        *(val_ALAP_u_copy + i) = *(val_ALAP_u + i);
        *(val_u + i) = *(val_ALAP_u + i) - *(val_ASAP_u + i);
    }

    for(i = 1; i <= num_nodes; i++){
        held_ALAP = *(val_ALAP_u_copy + i);
        held_u = *(val_u + i);
        held_node = *(p_list + i);

        j = i - 1;

        while(1){
            swap_flag = 0;
            candidate_ALAP = *(val_ALAP_u_copy + j);
            candidate_u = *(val_u + j);

            if(candidate_ALAP > held_ALAP)
                swap_flag = 1;
            else if(candidate_ALAP == held_ALAP)
                if(candidate_u > held_u)
                    swap_flag = 1;

            if(j < 0 || !swap_flag)
                break;

            *(p_list + j + 1) = *(p_list + j);
            *(val_ALAP_u_copy + j + 1) = *(val_ALAP_u_copy + j);
            *(val_u + j + 1) = *(val_u + j);

            j--;

        }
        *(p_list + j + 1) = held_node;
        *(val_ALAP_u_copy + j + 1) = held_ALAP;
        //*(val_ASAP_u_copy+2*(j+1)+2) = held_u;
    }

    //free the temporary memory that was allocated for ALAP copy
    free(val_ALAP_u_copy);
    free(val_u);

    return err;
}

/************************************************
Function name: opqueue

inputs:
      int mode
      int *queue
      int node

outputs:
        INITIALIZE - -1
        ENQUEUE    - index of the tail ptr
        DEQUEUE    - index of the head ptr, if the Queue is not empty
                     -1 if the, Queue is empty

This function implements the "initialize", "dequeue" and "enqueue" 
functions for a queue. This queue is used while running BFS. 
A parameter is passed to this function to select which of the 
functions to perform on the queue. 
 ************************************************/

int opqueue(int mode, int *queue, int node){
    static int head = 0, tail = 0;
    int ret_val = 0;

    switch(mode){
        case __INIT:
            head = 0;
            tail = 0;
            ret_val = -1;
            
            break;
        case __ENQUEUE:
            *(queue + tail) = node;
            tail = (tail + 1) % __SIZE_Q;
            ret_val = tail;
            break;
        case __DEQUEUE:
            if(head != tail){
                ret_val = *(queue + head);
                head = (head + 1) % __SIZE_Q;
            }else ret_val = -1;
            break;
    }

    return ret_val;
}


int opqueue2(int mode, int *queue, int node){
    static int head = 0, tail = 0;
    int ret_val = 0;

    switch(mode){
        case __INIT:
            head = 0;
            tail = 0;
            ret_val = -1;
            break;
        case __ENQUEUE:
            *(queue + tail) = node;
            tail = (tail + 1) % __SIZE_Q;
            ret_val = tail;
            break;
        case __DEQUEUE:
            if(head != tail){
                ret_val = *(queue + head);
                head = (head + 1) % __SIZE_Q;
            }else ret_val = -1;
            break;
    }

    return ret_val;
}

/************************************************
Function name: evaluate_ASAP

inputs:
      int num_nodes
      int *succ_adj_mat

outputs:
      int *val_ASAP_u

The function that evaluates the ASAP for each operator in the DFG.
Evaluating ASAP is accomplished by running DFS from the source node
keep on enqueing the nodes neighboring to the nodes for which the ASAP
has been determined (or the BFS depth has been determined). Keep on
running BFS till the queue is empty.
 ************************************************/

int evaluate_ASAP(int num_nodes,
                  int *succ_adj_mat,
                  int *val_ASAP_u,
                  t_task *task){
    int err = 0;
    int *queue;
    int i = 0;

    //allocate memory for the queue
    queue = (int *)malloc(sizeof(int) * __SIZE_Q);

    //  intialize the queue
    opqueue(__INIT, queue, __INVALID);

    for(i = 0; i <= (num_nodes + 1); i++)
        *(val_ASAP_u + i) = -1;

    *val_ASAP_u = 0;
    //  enqueue the source node
    opqueue(__ENQUEUE, queue, 0);

    //  execute the following loop, till the queue is not-empty
    while((i = opqueue(__DEQUEUE, queue, __INVALID)) != -1){
        int j;
        int curr_ASAP;

        curr_ASAP = 0;
        for(j = 1; j <= num_nodes + 1; j++){
            if(*(succ_adj_mat + (num_nodes + 2) * i + j) == 1){
                int new_ASAP;

                new_ASAP = i ? *(val_ASAP_u + i)+((task + i)->latency + (task + i)->reconfig_time) : 1;
                if(new_ASAP > curr_ASAP)
                    curr_ASAP = new_ASAP;
                if(*(val_ASAP_u + j) < curr_ASAP){
                    *(val_ASAP_u + j) = curr_ASAP;
                    if(j < (num_nodes + 1))
                        opqueue(__ENQUEUE, queue, j);
                }
            }
        }
    }


    //free the temporary memory used for the queue
    free(queue);

    return err;
}

/************************************************
Function name: evaluate_ALAP

inputs:
      int num_nodes
      int *succ_adj_mat

outputs:
      int *val_ALAP_u

Evaluating reverseASAP is accomplished by running DFS 
from the sink node after reversing all the edges.
The rest of the algo is similar to the evaluate_ASAP algo.
 ************************************************/

int evaluate_ALAP(int num_nodes,
                  int *succ_adj_mat,
                  int *val_ALAP_u,
                  int h,
                  t_task *task){
    int err = 0;
    int *queue;
    int i = 0;

    //  allocate memory for the queue
    queue = (int *)malloc(sizeof(int) * __SIZE_Q);

    //  intialize the queue
    opqueue(__INIT, queue, __INVALID);

    for(i = 0; i <= (num_nodes + 1); i++)
        *(val_ALAP_u + i) = -1;

    *(val_ALAP_u + num_nodes + 1) = h;
    *val_ALAP_u = 0;
    //  enqueue the sink node
    opqueue(__ENQUEUE, queue, num_nodes + 1);

    //  execute the following loop, till the queue is not-empty
    while((i = opqueue(__DEQUEUE, queue, __INVALID)) != -1){
        int j = 0;
        int curr_ALAP = 0;

        curr_ALAP = 0;
        for(j = 1; j <= num_nodes; j++){
            if(*(succ_adj_mat + (num_nodes + 2) * j + i) == 1){
                int new_ALAP = 0;

                new_ALAP = (i != num_nodes + 1) ? *(val_ALAP_u + i)+((task + i)->latency + (task + i)->reconfig_time) : 1;
                if(new_ALAP > curr_ALAP)
                    curr_ALAP = new_ALAP;

                if(*(val_ALAP_u + j) < curr_ALAP){
                    *(val_ALAP_u + j) = curr_ALAP;
                    opqueue(__ENQUEUE, queue, j);
                }
            }
        }
    }

    for(i = 1; i <= num_nodes; i++)
        *(val_ALAP_u + i) = (h + 1) - *(val_ALAP_u + i) - ((task + i)->latency + (task + i)->reconfig_time);

    //free the temporary memory used for the queue
    free(queue);

    return err;
}


int findAvailableTasks(int *queue, int num_nodes, int *p_list, int *N_sched, int *succ_adj_mat){
    int err = 0;
    int i = 0, j = 0, ready = 0, index = 0;

    for(i = 1; i <= num_nodes + 1; i++){
        ready = 0;
        index = *(p_list + i);
        if(*(N_sched + index) == -1){
            ready = 1;
            for(j = 0; j <= num_nodes + 1; j++){
                if(*(succ_adj_mat + (num_nodes + 2) * j + index) == 1 && *(N_sched + j) == -1)
                    ready = 0;
            }
        }

        if(ready == 1)
            opqueue(__ENQUEUE, queue, index);

    }

    return err;
}


bool findFarthestCompatibleModule(int x, int t, int *N_sched, t_task *task){
    bool compatible = false;
    int i = 0, j = 0, kc = 0, mc = 0, max_i = 0, max_j = 0, l = 0, r = 0;
    double max_d = 0.0, curr_d = 0.0, curr_kc = 0.0, curr_mc = 0.0;

    //calculate the center of the grid
    kc = floor(__NUM_FPGA_COLUMNS / 2);
    mc = floor(__NUM_FPGA_ROWS / 2);

    for(i = 0; i < __NUM_FPGA_COLUMNS; i++){
        for(j = 0; j < __NUM_FPGA_ROWS; j++){
            if(grid[i][j] != -1 &&
                    *(N_sched + grid[i][j]) != -1 &&
                    (*(N_sched + grid[i][j])+(task + grid[i][j])->latency - 1) < t &&
               (((task + grid[i][j])->type == (task + x)->type) && ((task + grid[i][j])->impl == (task + x)->impl))){
                compatible = true;
                curr_kc = (i + (task + grid[i][j])->columns) / 2;
                curr_mc = (j + (task + grid[i][j])->rows) / 2;
                curr_d = sqrt(pow(curr_kc - kc, 2.0) + pow(curr_mc - mc, 2.0));
                if(max_d <= curr_d){
                    max_i = i;
                    max_j = j;
                    max_d = curr_d;
                }
            }
        }
    }

    if(compatible && x != (task->width + 1)){
        grid[max_i][max_j] = x;
        (task + x)->leftmost_column = max_i;
        (task + x)->bottommost_row = max_j;
        for(l = max_i; l < (max_i + (task + x)->columns); l++)
            for(r = max_j; r < (max_j + (task + x)->rows); r++)
                grid_config[l][r] = 1;
    }else if(x == (task->width + 1)){
        compatible = true;
    }

    return compatible;
}


int schedule(int x, int t, bool reuse, int *N_sched, t_task *task, int *succ_adj_mat){
    int err = 0;
    int i = 0;
    int num_nodes = task->width;

    if(reuse){
        *(N_sched + x) = t;
        (task + x)->exec_sched = t;
    }else{
        (task + x)->reconfig_sched = t;
        *(N_sched + x) = t + (task + x)->reconfig_time;
        (task + x)->exec_sched = t + (task + x)->reconfig_time;
    }

    for(i = 0; i <= num_nodes + 1; i++){
        if(*(succ_adj_mat + (num_nodes + 2) * i + x) == 1){
            *(N_sched + x) = *(N_sched + x)<(*(N_sched + i)+(task + i)->latency) ? (*(N_sched + i)+(task + i)->latency) : *(N_sched + x);
            (task + x)->exec_sched = *(N_sched + x);
        }
    }

    return err;
}


bool findFarthestAvailableSpace(int x, int t, t_task *task){
    bool found = false, free_av = false;
    int i = 0, j = 0, k = 0, m = 0, l = 0, r = 0, max_i = 0, max_j = 0, kc = 0, mc = 0;
    double curr_kc = 0.0, curr_mc = 0.0, curr_d = 0.0, max_d = 0.0;

    //calculate the center of the grid
    kc = floor(__NUM_FPGA_COLUMNS / 2.0);
    mc = floor(__NUM_FPGA_ROWS / 2.0);

    for(i = 0; i <= (__NUM_FPGA_COLUMNS - (task + x)->columns); i++){
        for(j = 0; j <= (__NUM_FPGA_ROWS - (task + x)->rows); j++){
            if(grid_config[i][j] == -1){
                found = true;
                for(k = i; k < i + (task + x)->columns; k++){
                    for(m = j; m < j + (task + x)->rows; m++){
                        if(grid_config[k][m] != -1)
                            found = false;
                    }
                }
            }else{
                found = false;
            }

            if(found){
                free_av = true;
                curr_kc = (i + (task + x)->columns) / 2.0;
                curr_mc = (j + (task + x)->rows) / 2.0;
                curr_d = sqrt(pow(curr_kc - kc, 2.0) + pow(curr_mc - mc, 2.0));
                if(max_d <= curr_d){
                    max_i = i;
                    max_j = j;
                    max_d = curr_d;
                }
            }

        }
    }

    if(free_av){
        grid[max_i][max_j] = x;
        (task + x)->leftmost_column = max_i;
        (task + x)->bottommost_row = max_j;
        for(l = max_i; l < (max_i + (task + x)->columns); l++){
            for(r = max_j; r < (max_j + (task + x)->rows); r++){
                if(l != max_i || r != max_j){
                    grid[l][r] = -1;
                }
                grid_config[l][r] = 1;
            }
        }
        for(i = 0; i < __NUM_FPGA_COLUMNS; i++){
            for(j = 0; j < __NUM_FPGA_ROWS; j++){
                if(grid[i][j] != -1 && grid_config[i][j] == -1){
                    if(overlap(x, grid[i][j], task)){
                        grid[i][j] = -1;
                    }
                }
            }
        }
    }

    return free_av;
}


bool overlap(int x, int y, t_task *task){
    bool intersect = false;
    int cx = (task + x)->leftmost_column;
    int cy = (task + y)->leftmost_column;
    int rx = (task + x)->bottommost_row;
    int ry = (task + y)->bottommost_row;
    int x_c = (task + x)->columns;
    int y_c = (task + y)->columns;
    int x_r = (task + x)->rows;
    int y_r = (task + y)->rows;

    if(cx <= cy){
        if(rx <= ry){
            if((ry <= rx + x_r - 1) && (cy <= cx + x_c - 1)){
                intersect = true;
            }
        }else{
            if((rx <= ry + y_r - 1) && (cx <= cy + y_c - 1)){
                intersect = true;
            }
        }
    }else{
        if(ry <= rx){
            if((rx <= ry + y_r - 1) && (cx <= cy + y_c - 1)){
                intersect = true;
            }
        }else{
            if((ry <= rx + x_r - 1) && (cy <= cx + x_c - 1)){
                intersect = true;
            }
        }
    }

    return intersect;
}


GA_Info Napoleon(FILE *grid_strm, int *succ_adj_mat, int num_nodes, t_task *task){
    int err = 0;
    int *val_ASAP_u, *val_ALAP_u, *p_list;
    int *Na, *t_Na;
    int *N_sched;
    int i = 0, j = 0, t = 0, x = 0, l = 0, r = 0;
    bool reuse = false, flag = false;
    int free_reconfig = 0;
    static GA_Info fitness_values;


    //initialize the grid
    for(i = 0; i < __NUM_FPGA_COLUMNS; i++){
        for(j = 0; j < __NUM_FPGA_ROWS; j++){
            grid[i][j] = -1;
            grid_config[i][j] = -1;
        }
    }

    //create memory for Napoleon Scheduler and initialize it with -1
    N_sched = (int *)malloc(sizeof ( int)*(num_nodes + 2));
    for(i = 0; i <= num_nodes + 1; i++)
        *(N_sched + i) = -1;

    //Sechedule the source node at t=0
    *(N_sched) = t;

    //increment the time step by 1
    t = 1;

    //create memory for ASAP and schedule the tasks based-on ASAP
    val_ASAP_u = (int *)malloc(sizeof ( int)*(num_nodes + 2));
    if((err = evaluate_ASAP(num_nodes, succ_adj_mat, val_ASAP_u, task)))
        print_error(err);

    //create memory for ALAP and schedule the tasks based-on ALAP
    val_ALAP_u = (int *)malloc(sizeof ( int)* (task->width + 2));
    if((err = evaluate_ALAP(num_nodes, succ_adj_mat, val_ALAP_u, *(val_ASAP_u + (num_nodes + 1)), task)))
        print_error(err);

#ifdef __DEBUG
    int dim_size[1];
    *dim_size = (num_nodes + 2);

    //display the ASAP values
    printf("The ASAP Schedule\n");
    display_array(val_ASAP_u, 1, dim_size);

    //display the ALAP values
    printf("The ALAP Schedule\n");
    display_array(val_ALAP_u, 1, dim_size);
#endif

    //create memory for priority list
    p_list = (int *)malloc(sizeof ( int)*(num_nodes + 2));

    //initialize p_list 
    for(i = 0; i <= (num_nodes + 1); i++)
        *(p_list + i) = i;

    //sort p_list based-on ALAP
    sortbyALAP(val_ALAP_u, val_ASAP_u, num_nodes, p_list);

#ifdef __DEBUG
    printf("Tasks sorted by ALAP:\n");
    for(i = 0; i <= (num_nodes + 1); i++)
        printf("%d ", *(p_list + i));
    printf("\n");
#endif

    //allocate memory for the queue
    Na = (int *)malloc(sizeof ( int)*__SIZE_Q);
    t_Na = (int *)malloc(sizeof ( int)*__SIZE_Q);

    //intialize the queue
    opqueue(__INIT, Na, __INVALID);

    //intialize the queue2
    opqueue2(__INIT, t_Na, __INVALID);

    //store the available tasks in Na
    findAvailableTasks(Na, num_nodes, p_list, N_sched, succ_adj_mat);

#ifdef __DEBUG
    fprintf(grid_strm, "T\t\t C0\t\tC1\t\tC2\t\tC3\t\tC4\n");
#endif

    while(*(N_sched + num_nodes + 1) == -1){
        reuse = true;
        
        while((x = opqueue(__DEQUEUE, Na, __INVALID)) != -1){

            if((flag = findFarthestCompatibleModule(x, t, N_sched, task))){
                schedule(x, t, reuse, N_sched, task, succ_adj_mat);

            }else{
                opqueue2(__ENQUEUE, t_Na, x);

            }
        }

        while((x = opqueue2(__DEQUEUE, t_Na, __INVALID)) != -1){
            opqueue(__ENQUEUE, Na, x);
        }

        reuse = false;
        while((x = opqueue(__DEQUEUE, Na, __INVALID)) != -1){
            if((free_reconfig < t) && (findFarthestAvailableSpace(x, t, task))){
                schedule(x, t, reuse, N_sched, task, succ_adj_mat);
                free_reconfig = (task + x)->reconfig_sched + (task + x)->reconfig_time - 1;

                break;
            }else{
                opqueue2(__ENQUEUE, t_Na, x);

            }
        }

        while((x = opqueue(__DEQUEUE, Na, __INVALID)) != -1);
        while((x = opqueue2(__DEQUEUE, t_Na, __INVALID)) != -1);

#ifdef __DEBUG    
        fprintf(grid_strm, "t=%3d\t\t", t);
        for(i = 0; i < __NUM_FPGA_COLUMNS; i++){
            if(grid[i][0] != -1){
                if((((task + grid[i][0])->reconfig_sched + (task + grid[i][0])->reconfig_time - 1) >= t) && ((task + grid[i][0])->reconfig_sched <= t)) // reconfig period
                {
                    fprintf(grid_strm, "\"T%d/R/%d\"\t", grid[i][0], (task + grid[i][0])->type);
                }else if((((task + grid[i][0])->exec_sched + (task + grid[i][0])->latency - 1) >= t) && ((task + grid[i][0])->exec_sched <= t)) // exec period
                {
                    fprintf(grid_strm, "\"T%d/E/%d\"\t", grid[i][0], (task + grid[i][0])->type);
                }else{
                    fprintf(grid_strm, "\t\t");
                }
            }else{
                fprintf(grid_strm, "\t\t");
            }
        }
        fprintf(grid_strm, "\n");
#endif 
        //printf("t=%d\n", t);
        t = t + 1;
        findAvailableTasks(Na, num_nodes, p_list, N_sched, succ_adj_mat);

        for(i = 0; i < __NUM_FPGA_COLUMNS; i++){
            for(j = 0; j < __NUM_FPGA_ROWS; j++){
                if(grid[i][j] > 0 && (grid_config[i][j] != -1) && (*(N_sched + grid[i][j])+(task + grid[i][j])->latency - 1) < t){
                    for(l = i; l < (i + (task + grid[i][j])->columns); l++)
                        for(r = j; r < (j + (task + grid[i][j])->rows); r++)
                            grid_config[l][r] = -1;
                }
            }
        }

    }

#ifdef __DEBUG
    do{
        fprintf(grid_strm, "t=%3d\t\t", t);
        for(i = 0; i < __NUM_FPGA_COLUMNS; i++){
            if(grid[i][0] != -1){
                if((((task + grid[i][0])->reconfig_sched + (task + grid[i][0])->reconfig_time - 1) >= t) && ((task + grid[i][0])->reconfig_sched <= t)) // reconfig period
                {
                    fprintf(grid_strm, "\"T%d/R/%d\"\t", grid[i][0], (task + grid[i][0])->type);
                }else if((((task + grid[i][0])->exec_sched + (task + grid[i][0])->latency - 1) >= t) && ((task + grid[i][0])->exec_sched <= t)) // exec period
                {
                    fprintf(grid_strm, "\"T%d/E/%d\"\t", grid[i][0], (task + grid[i][0])->type);
                }else{
                    fprintf(grid_strm, "\t\t");
                }
            }else{
                fprintf(grid_strm, "\t\t");
            }
        }
        fprintf(grid_strm, "\n");
        t++;
    }while(t <= *(N_sched + num_nodes + 1));

    printf("N_sched: \n");
    for(i = 0; i <= num_nodes + 1; i++)
        printf("%d ", *(N_sched + i));
    printf("\n");

    printf("task reconfigured at: \n");
    for(i = 0; i <= num_nodes + 1; i++)
        printf("%d ", (task + i)->reconfig_sched);
    printf("\n");

    printf("task scheduled at: \n");
    for(i = 0; i <= num_nodes + 1; i++)
        printf("%d ", (task + i)->exec_sched);
    printf("\n");

    printf("leftmost column: \n");
    for(i = 0; i <= num_nodes + 1; i++)
        printf("%d ", (task + i)->leftmost_column);
    printf("\n");

    printf("bottommost rows: \n");
    for(i = 0; i <= num_nodes + 1; i++)
        printf("%d ", (task + i)->bottommost_row);
    printf("\n");
#endif

    fitness_values.prefetch = 0;
    bool prefetch_flag = false;

    for(j = 1; j <= num_nodes; j++){
        prefetch_flag = false;
        for(i = 1; i <= num_nodes; i++){
            if(*(succ_adj_mat + (num_nodes + 2) * i + j) == 1){
                if((task + j)->reconfig_sched != 0 && (task + j)->reconfig_sched <= ((task + i)->exec_sched + (task + i)->latency - 1)){
                    prefetch_flag = true;
                }
            }
        }
        if(prefetch_flag)
            fitness_values.prefetch++;
    }

    fitness_values.reuse = 0;
    for(i = 1; i <= num_nodes; i++)
        if((task + i)->reconfig_sched == 0)
            fitness_values.reuse++;

    
//    printf("TaskID\tReconfig At\tExec At\t\tColumn\t\tRow\t\tType\t\tImpl\n");
//    for(i = 1; i <= num_nodes; i++)
//        printf("T%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", i, (task + i)->reconfig_sched, (task + i)->exec_sched, (task + i)->leftmost_column, (task + i)->bottommost_row, (task + i)->type, (task + i)->impl);

    
    fitness_values.power = 0;
    for(i = 0; i <= num_nodes; i++){
        fitness_values.power += (task + i)->exec_pwr;
        if((task + i)->reconfig_sched != 0){
            fitness_values.power += (task + i)->reconfig_pwr;
        }
    }
    
    fitness_values.runtime = (task + num_nodes + 1)->exec_sched - 1;
    
//    printf("***** TOTAL NUMBER OF CYCLES = %d *****\n", (task + num_nodes + 1)->exec_sched - 1);
//    printf("***** TOTAL POWER = %d *****\n", tot_pwr);

    free(t_Na);
    free(Na);
    free(p_list);
    free(val_ALAP_u);
    free(val_ASAP_u);
    free(N_sched);

    return (fitness_values);
}