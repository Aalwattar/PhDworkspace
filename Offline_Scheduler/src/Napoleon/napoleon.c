/*************************************************************************
 * Author   : Ziad Abuowaimer
 * Modifier : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 * 
 * Created  : May 22, 2013
 * Modified : May 22, 2013
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
#include <main.h>
#include <io.h>


int evaluate_ASAP(short int, short int *, short int *, t_task *);
int evaluate_ALAP(short int, short int *, short int *, short int, t_task *);
int opqueue(short int, short int *, short int);
int sortbyALAP(short int *, short int *, short int, short int *);
int findAvailableTasks(short int *, short int, short int *, short int *, short int *);
bool findFarthestCompatibleModule(short int, short int, short int *, t_task *);
int schedule(short int, short int, bool, short int *, t_task *, short int *);
bool findFarthestAvailableSpace(short int, short int, t_task *);
short int grid[__NUM_FPGA_COLUMNS][__NUM_FPGA_ROWS];
short int grid_config[__NUM_FPGA_COLUMNS][__NUM_FPGA_ROWS];

/************************************************
Function name: sort

inputs:
        short int *val_ALAP_u
        short int num_nodes
outputs:
        short int *p_list

This sort algorithm implements the INSERTION sort technique. 
Sorting is done in NON_DECREASING order of two keys: 
  a. ALAP value(primary)
  

 *(val_ALAP_u+i) has the ALAP value
    
 ************************************************/

int sortbyALAP(short int *val_ALAP_u,
        short int *val_ASAP_u,
        short int num_nodes,
        short int *p_list) {
    int err = 0;
    short int held_ALAP = 0, held_u = 0, held_node = 0;
    short int candidate_ALAP = 0, candidate_u = 0;
    short int i = 0, j = 0, swap_flag = 0;
    short int *val_ALAP_u_copy, *val_u;

    //Make a copy of the ALAP and mobility list before sorting because
    //the order changes when the sorting finishes
    val_ALAP_u_copy = (short int *) malloc(sizeof (short int)*num_nodes + 2);
    val_u = (short int *) malloc(sizeof (short int)*num_nodes + 2);

    for (i = 0; i <= num_nodes; i++) {
        *(val_ALAP_u_copy + i) = *(val_ALAP_u + i);
        *(val_u + i) = *(val_ALAP_u + i) - *(val_ASAP_u + i);
    }

    for (i = 1; i <= num_nodes; i++) {
        held_ALAP = *(val_ALAP_u_copy + i);
        held_u = *(val_u + i);
        held_node = *(p_list + i);

        j = i - 1;

        while (1) {
            swap_flag = 0;
            candidate_ALAP = *(val_ALAP_u_copy + j);
            candidate_u = *(val_u + j);

            if (candidate_ALAP > held_ALAP)
                swap_flag = 1;
            else if (candidate_ALAP == held_ALAP)
                if (candidate_u > held_u)
                    swap_flag = 1;

            if (j < 0 || !swap_flag)
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
        short int mode
        short int *queue
        short int node

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

int opqueue(short int mode, short int *queue, short int node) {
    static short int head = 0, tail = 0;
    short int ret_val = 0;

    switch (mode) {
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
            if (head != tail) {
                ret_val = *(queue + head);
                head = (head + 1) % __SIZE_Q;
            } else ret_val = -1;
            break;
    }

    return ret_val;
}

int opqueue2(short int mode, short int *queue, short int node) {
    static short int head = 0, tail = 0;
    short int ret_val = 0;

    switch (mode) {
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
            if (head != tail) {
                ret_val = *(queue + head);
                head = (head + 1) % __SIZE_Q;
            } else ret_val = -1;
            break;
    }

    return ret_val;
}

/************************************************
Function name: evaluate_ASAP

inputs:
        short int num_nodes
        short int *succ_adj_mat

outputs:
        short int *val_ASAP_u

The function that evaluates the ASAP for each operator in the DFG.
Evaluating ASAP is accomplished by running DFS from the source node
keep on enqueing the nodes neighboring to the nodes for which the ASAP
has been determined (or the BFS depth has been determined). Keep on
running BFS till the queue is empty.
 ************************************************/

int evaluate_ASAP(short int num_nodes,
        short int *succ_adj_mat,
        short int *val_ASAP_u,
        t_task *task) {
    int err = 0;
    short int *queue;
    short int i = 0;

    //allocate memory for the queue
    queue = (short int *) malloc(sizeof (short int)*__SIZE_Q);

    //  intialize the queue
    opqueue(__INIT, queue, __INVALID);

    for (i = 0; i <= (num_nodes + 1); i++)
        *(val_ASAP_u + i) = -1;

    *val_ASAP_u = 0;
    //  enqueue the source node
    opqueue(__ENQUEUE, queue, 0);

    //  execute the following loop, till the queue is not-empty
    while ((i = opqueue(__DEQUEUE, queue, __INVALID)) != -1) {
        short int j;
        short int curr_ASAP;

        curr_ASAP = 0;
        for (j = 1; j <= num_nodes + 1; j++) {
            if (*(succ_adj_mat + (num_nodes + 2) * i + j) == 1) {
                short int new_ASAP;

                new_ASAP = i ? *(val_ASAP_u + i)+((task + i)->latency + (task + i)->reconfig_time) : 1;
                if (new_ASAP > curr_ASAP)
                    curr_ASAP = new_ASAP;
                if (*(val_ASAP_u + j) < curr_ASAP) {
                    *(val_ASAP_u + j) = curr_ASAP;
                    if (j < (num_nodes + 1))
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
        short int num_nodes
        short int *succ_adj_mat

outputs:
        short int *val_ALAP_u

Evaluating reverseASAP is accomplished by running DFS 
from the sink node after reversing all the edges.
The rest of the algo is similar to the evaluate_ASAP algo.
 ************************************************/

int evaluate_ALAP(short int num_nodes,
        short int *succ_adj_mat,
        short int *val_ALAP_u,
        short int h,
        t_task *task) {
    int err = 0;
    short int *queue;
    short int i = 0;

    //  allocate memory for the queue
    queue = (short int *) malloc(sizeof (short int)*__SIZE_Q);

    //  intialize the queue
    opqueue(__INIT, queue, __INVALID);

    for (i = 0; i <= (num_nodes + 1); i++)
        *(val_ALAP_u + i) = -1;

    *(val_ALAP_u + num_nodes + 1) = h;
    *val_ALAP_u = 0;
    //  enqueue the sink node
    opqueue(__ENQUEUE, queue, num_nodes + 1);

    //  execute the following loop, till the queue is not-empty
    while ((i = opqueue(__DEQUEUE, queue, __INVALID)) != -1) {
        short int j = 0;
        short int curr_ALAP = 0;

        curr_ALAP = 0;
        for (j = 1; j <= num_nodes; j++) {
            if (*(succ_adj_mat + (num_nodes + 2) * j + i) == 1) {
                short int new_ALAP = 0;

                new_ALAP = (i != num_nodes + 1) ? *(val_ALAP_u + i)+((task + i)->latency + (task + i)->reconfig_time) : 1;
                if (new_ALAP > curr_ALAP)
                    curr_ALAP = new_ALAP;

                if (*(val_ALAP_u + j) < curr_ALAP) {
                    *(val_ALAP_u + j) = curr_ALAP;
                    opqueue(__ENQUEUE, queue, j);
                }
            }
        }
    }

    for (i = 1; i <= num_nodes; i++)
        *(val_ALAP_u + i) = (h + 1) - *(val_ALAP_u + i) - ((task + i)->latency + (task + i)->reconfig_time);

    //free the temporary memory used for the queue
    free(queue);

    return err;
}

int findAvailableTasks(short int *queue, short int num_nodes, short int *p_list, short int *N_sched, short int *succ_adj_mat) {
    int err = 0;
    short int i = 0, j = 0, ready = 0, index = 0;

    for (i = 1; i <= num_nodes + 1; i++) {
        ready = 0;
        index = *(p_list + i);
        if (*(N_sched + index) == -1) {
            ready = 1;
            for (j = 0; j <= num_nodes + 1; j++) {
                if (*(succ_adj_mat + (num_nodes + 2) * j + index) == 1 && *(N_sched + j) == -1)
                    ready = 0;
            }
        }

        if (ready == 1)
            opqueue(__ENQUEUE, queue, index);

    }

    return err;
}

bool findFarthestCompatibleModule(short int x, short int t, short int *N_sched, t_task *task) {
    bool compatible = false;
    short int i = 0, j = 0, kc = 0, mc = 0, max_i = 0, max_j = 0, l = 0, r = 0;
    double max_d = 0.0, curr_d = 0.0, curr_kc = 0.0, curr_mc = 0.0;

    //calculate the center of the grid
    kc = floor(__NUM_FPGA_COLUMNS / 2);
    mc = floor(__NUM_FPGA_ROWS / 2);

    for (i = 0; i < __NUM_FPGA_COLUMNS; i++) {
        for (j = 0; j < __NUM_FPGA_ROWS; j++) {
            if (grid[i][j] != -1 &&
                    *(N_sched + grid[i][j]) != -1 &&
                    (*(N_sched + grid[i][j])+(task + grid[i][j])->latency - 1) < t &&
                    (task + grid[i][j])->type == (task + x)->type) {
                compatible = true;
                curr_kc = (i + (task + grid[i][j])->columns) / 2;
                curr_mc = (j + (task + grid[i][j])->rows) / 2;
                curr_d = sqrt(pow(curr_kc - kc, 2.0) + pow(curr_mc - mc, 2.0));
                if (max_d <= curr_d) {
                    max_i = i;
                    max_j = j;
                    max_d = curr_d;
                }
            }
        }
    }

    if (compatible && x != (task->width + 1)) {
        grid[max_i][max_j] = x;
        (task + x)->leftmost_column = max_i;
        (task + x)->bottommost_row = max_j;
        for (l = max_i; l < (max_i + (task + x)->columns); l++)
            for (r = max_j; r < (max_j + (task + x)->rows); r++)
                grid_config[l][r] = 1;
    } else if (x == (task->width + 1)) {
        compatible = true;
    }

    return compatible;
}

int schedule(short int x, short int t, bool reuse, short int *N_sched, t_task *task, short int *succ_adj_mat) {
    int err = 0;
    short int i = 0;
    //short int max_t=0;
    short int num_nodes = task->width;

    if (reuse) {
        *(N_sched + x) = t;
        (task + x)->exec_sched = t;
    } else {
        (task + x)->reconfig_sched = t;
        *(N_sched + x) = t + (task + x)->reconfig_time;
        (task + x)->exec_sched = t + (task + x)->reconfig_time;
    }

    for (i = 0; i <= num_nodes + 1; i++) {
        if (*(succ_adj_mat + (num_nodes + 2) * i + x) == 1) {
            *(N_sched + x) = *(N_sched + x)<(*(N_sched + i)+(task + i)->latency) ? (*(N_sched + i)+(task + i)->latency) : *(N_sched + x);
            (task + x)->exec_sched = *(N_sched + x);
        }
    }

    return err;
}

bool findFarthestAvailableSpace(short int x, short int t, t_task *task) {
    bool found = false, free_av = false;
    short int i = 0, j = 0, k = 0, m = 0, l = 0, r = 0, max_i = 0, max_j = 0, kc = 0, mc = 0;
    double curr_kc = 0.0, curr_mc = 0.0, curr_d = 0.0, max_d = 0.0;

    //calculate the center of the grid
    kc = floor(__NUM_FPGA_COLUMNS / 2.0);
    mc = floor(__NUM_FPGA_ROWS / 2.0);

    for (i = 0; i <= (__NUM_FPGA_COLUMNS - (task + x)->columns); i++) {
        for (j = 0; j <= (__NUM_FPGA_ROWS - (task + x)->rows); j++) {
            if (grid_config[i][j] == -1) {
                found = true;
                for (k = i; k < i + (task + x)->columns; k++) {
                    for (m = j; m < j + (task + x)->rows; m++) {
                        if (grid_config[k][m] != -1)
                            found = false;
                    }
                }
            } else {
                found = false;
            }

            if (found) {
                free_av = true;
                curr_kc = (i + (task + x)->columns) / 2.0;
                curr_mc = (j + (task + x)->rows) / 2.0;
                curr_d = sqrt(pow(curr_kc - kc, 2.0) + pow(curr_mc - mc, 2.0));
                if (max_d <= curr_d) {
                    max_i = i;
                    max_j = j;
                    max_d = curr_d;
                }
            }

        }
    }

    if (free_av) {
        grid[max_i][max_j] = x;
        (task + x)->leftmost_column = max_i;
        (task + x)->bottommost_row = max_j;
        for (l = max_i; l < (max_i + (task + x)->columns); l++) {
            for (r = max_j; r < (max_j + (task + x)->rows); r++) {
                if (l != max_i || r != max_j) {
                    grid[l][r] = -1;
                }
                grid_config[l][r] = 1;
            }
        }
    }

    return free_av;
}

short int Napoleon(FILE *grid_strm, short int *succ_adj_mat, short int num_nodes, t_task *task) {
    short int err = 0;
    short int *val_ASAP_u, *val_ALAP_u, *p_list;
    short int *Na, *t_Na;
    short int *N_sched;
    short int i = 0, j = 0, t = 0, x = 0, l = 0, r = 0;
    bool reuse = false, flag = false;
    short int free_reconfig = 0;


    //initialize the grid
    for (i = 0; i < __NUM_FPGA_COLUMNS; i++) {
        for (j = 0; j < __NUM_FPGA_ROWS; j++) {
            grid[i][j] = -1;
            grid_config[i][j] = -1;
        }
    }

    //create memory for Napoleon Scheduler and initialize it with -1
    N_sched = (short int *) malloc(sizeof (short int)*(num_nodes + 2));
    for (i = 0; i <= num_nodes + 1; i++)
        *(N_sched + i) = -1;

    //Sechedule the source node at t=0
    *(N_sched) = t;

    //increment the time step by 1
    t = 1;

    //create memory for ASAP and schedule the tasks based-on ASAP
    val_ASAP_u = (short int *) malloc(sizeof (short int)*(num_nodes + 2));
    if ((err = evaluate_ASAP(num_nodes, succ_adj_mat, val_ASAP_u, task)))
        print_error(err);

    //create memory for ALAP and schedule the tasks based-on ALAP
    val_ALAP_u = (short int *) malloc(sizeof (short int)*(task->width + 2));
    if ((err = evaluate_ALAP(num_nodes, succ_adj_mat, val_ALAP_u, *(val_ASAP_u + (num_nodes + 1)), task)))
        print_error(err);

    short int dim_size[1];
    *dim_size = (num_nodes + 2);

    //display the ASAP values
    printf("The ASAP Schedule\n");
    display_array(val_ASAP_u, 1, dim_size);

    //display the ALAP values
    printf("The ALAP Schedule\n");
    display_array(val_ALAP_u, 1, dim_size);

    //create memory for priority list
    p_list = (short int *) malloc(sizeof (short int)*(num_nodes + 2));

    //initialize p_list 
    for (i = 0; i <= (num_nodes + 1); i++)
        *(p_list + i) = i;

    //sort p_list based-on ALAP
    sortbyALAP(val_ALAP_u, val_ASAP_u, num_nodes, p_list);

    printf("Tasks sorted by ALAP:\n");
    for (i = 0; i <= (num_nodes + 1); i++)
        printf("%d ", *(p_list + i));
    printf("\n");

    //allocate memory for the queue
    Na = (short int *) malloc(sizeof (short int)*__SIZE_Q);
    t_Na = (short int *) malloc(sizeof (short int)*__SIZE_Q);

    //intialize the queue
    opqueue(__INIT, Na, __INVALID);

    //intialize the queue2
    opqueue2(__INIT, t_Na, __INVALID);

    //store the available tasks in Na
    findAvailableTasks(Na, num_nodes, p_list, N_sched, succ_adj_mat);

    fprintf(grid_strm, "T\t\t C0\t\tC1\t\tC2\t\tC3\t\tC4\n");

    while (*(N_sched + num_nodes + 1) == -1) {
        reuse = true;
        //printf("Current Time is %d\n", t);
        while ((x = opqueue(__DEQUEUE, Na, __INVALID)) != -1) {
            //printf("x=%d \n", x);
            if ((flag = findFarthestCompatibleModule(x, t, N_sched, task))) {
                schedule(x, t, reuse, N_sched, task, succ_adj_mat);
                //printf("reuse=true x=%d is scheduled on %d\n", x, (task+x)->exec_sched);
            } else {
                opqueue2(__ENQUEUE, t_Na, x);
                //printf("reuse=true x=%d not scheduled \n", x);
            }
        }

        while ((x = opqueue2(__DEQUEUE, t_Na, __INVALID)) != -1) {
            opqueue(__ENQUEUE, Na, x);
        }

        reuse = false;
        while ((x = opqueue(__DEQUEUE, Na, __INVALID)) != -1) {
            if ((free_reconfig < t) && (findFarthestAvailableSpace(x, t, task))) {
                schedule(x, t, reuse, N_sched, task, succ_adj_mat);
                free_reconfig = (task + x)->reconfig_sched + (task + x)->reconfig_time - 1;
                //printf("reuse=false x=%d is reconfigured at %d and scheduled on %d\n", x, (task+x)->reconfig_sched, (task+x)->exec_sched);
                break;
            } else {
                opqueue2(__ENQUEUE, t_Na, x);
                //printf("reuse=false x=%d not scheduled \n", x);
            }
        }

        while ((x = opqueue(__DEQUEUE, Na, __INVALID)) != -1);
        while ((x = opqueue2(__DEQUEUE, t_Na, __INVALID)) != -1);

        fprintf(grid_strm, "t=%3d\t\t", t);
        for (i = 0; i < __NUM_FPGA_COLUMNS; i++) {
            if (grid[i][0] != -1) {
                if ((((task + grid[i][0])->reconfig_sched + (task + grid[i][0])->reconfig_time - 1) >= t) && ((task + grid[i][0])->reconfig_sched <= t)) // reconfig period
                {
                    fprintf(grid_strm, "\"T%d/R/%d\"\t", grid[i][0], (task + grid[i][0])->type);
                } else if ((((task + grid[i][0])->exec_sched + (task + grid[i][0])->latency - 1) >= t) && ((task + grid[i][0])->exec_sched <= t)) // exec period
                {
                    fprintf(grid_strm, "\"T%d/E/%d\"\t", grid[i][0], (task + grid[i][0])->type);
                } else {
                    fprintf(grid_strm, "\t\t");
                }
            } else {
                fprintf(grid_strm, "\t\t");
            }
        }
        fprintf(grid_strm, "\n");

        t = t + 1;
        findAvailableTasks(Na, num_nodes, p_list, N_sched, succ_adj_mat);

        for (i = 0; i < __NUM_FPGA_COLUMNS; i++) {
            for (j = 0; j < __NUM_FPGA_ROWS; j++) {
                if (grid[i][j] > 0 && (*(N_sched + grid[i][j])+(task + grid[i][j])->latency - 1) < t) {
                    for (l = i; l < (i + (task + grid[i][j])->columns); l++)
                        for (r = j; r < (j + (task + grid[i][j])->rows); r++)
                            grid_config[l][r] = -1;
                }
            }
        }

    }

    do {
        fprintf(grid_strm, "t=%3d\t\t", t);
        for (i = 0; i < __NUM_FPGA_COLUMNS; i++) {
            if (grid[i][0] != -1) {
                if ((((task + grid[i][0])->reconfig_sched + (task + grid[i][0])->reconfig_time - 1) >= t) && ((task + grid[i][0])->reconfig_sched <= t)) // reconfig period
                {
                    fprintf(grid_strm, "\"T%d/R/%d\"\t", grid[i][0], (task + grid[i][0])->type);
                } else if ((((task + grid[i][0])->exec_sched + (task + grid[i][0])->latency - 1) >= t) && ((task + grid[i][0])->exec_sched <= t)) // exec period
                {
                    fprintf(grid_strm, "\"T%d/E/%d\"\t", grid[i][0], (task + grid[i][0])->type);
                } else {
                    fprintf(grid_strm, "\t\t");
                }
            } else {
                fprintf(grid_strm, "\t\t");
            }
        }
        fprintf(grid_strm, "\n");
        t++;
    } while (t <= *(N_sched + num_nodes + 1));

    printf("N_sched: \n");
    for (i = 0; i <= num_nodes + 1; i++)
        printf("%d ", *(N_sched + i));
    printf("\n");

    printf("task reconfigured at: \n");
    for (i = 0; i <= num_nodes + 1; i++)
        printf("%d ", (task + i)->reconfig_sched);
    printf("\n");

    printf("task scheduled at: \n");
    for (i = 0; i <= num_nodes + 1; i++)
        printf("%d ", (task + i)->exec_sched);
    printf("\n");

    printf("leftmost column: \n");
    for (i = 0; i <= num_nodes + 1; i++)
        printf("%d ", (task + i)->leftmost_column);
    printf("\n");

    printf("bottommost rows: \n");
    for (i = 0; i <= num_nodes + 1; i++)
        printf("%d ", (task + i)->bottommost_row);
    printf("\n");

#ifdef __DEBUG
    printf("grid: \n");
    for (i = 0; i < __NUM_FPGA_COLUMNS; i++) {
        for (j = __NUM_FPGA_ROWS - 1; j >= 0; j--) {
            if (grid[i][j] != -1)
                printf("%d %d %d\n", i, j, grid[i][j]);
        }
    }

    printf("grid_config: \n");
    for (i = 0; i < __NUM_FPGA_COLUMNS; i++) {
        for (j = __NUM_FPGA_ROWS - 1; j >= 0; j--) {
            if (grid_config[i][j] != -1)
                printf("%d %d %d\n", i, j, grid_config[i][j]);
        }
    }
#endif

    short int cnt_reuse = 0, cnt_prefetch = 0;
    bool prefetch_flag = false;

    for (j = 1; j <= num_nodes; j++) {
        prefetch_flag = false;
        for (i = 1; i <= num_nodes; i++) {
            if (*(succ_adj_mat + (num_nodes + 2) * i + j) == 1) {
                if ((task + j)->reconfig_sched != 0 && (task + j)->reconfig_sched <= ((task + i)->exec_sched + (task + i)->latency - 1)) {
                    prefetch_flag = true;
                }
            }
        }
        if (prefetch_flag)
            cnt_prefetch++;
    }

    printf("Total Prefetching is %d\n", cnt_prefetch);

    for (i = 1; i <= num_nodes; i++)
        if ((task + i)->reconfig_sched == 0)
            cnt_reuse++;

    printf("Total Number of Reuse is %d\n", cnt_reuse);

    printf("TaskID\tReconfig At\tExec At\t\tColumn\t\tRow\t\tType\n");
    for (i = 1; i <= num_nodes; i++)
        printf("T%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", i, (task + i)->reconfig_sched, (task + i)->exec_sched, (task + i)->leftmost_column, (task + i)->bottommost_row, (task + i)->type);

    printf("***** TOTAL NUMBER OF CYCLES = %d *****\n", (task + num_nodes + 1)->exec_sched - 1);

    free(t_Na);
    free(Na);
    free(p_list);
    free(val_ALAP_u);
    free(val_ASAP_u);
    free(N_sched);

    //return err;
    return (task + num_nodes + 1)->exec_sched - 1;
}


