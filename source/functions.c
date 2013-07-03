/*************************************************************************
 * Author   : Ziad Abuowaimer
 * Modifier : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 * 
 * Created  : May 22, 2013
 * Modified : July 3, 2013
 *************************************************************************/

/*******************************************************************
  Compile with -D__DEBUG option for enabling debugging jigs in the code

  Filename: functions.c

  Description:

  Two independent functions are implemented in this file.
    1. create_graph - 	This function creates an adjacency 
            matrix from the operators' list and the 
            interface list of the operators. It also 
            adds a source node and a sink node based 
            on the primary inputs and the primary outputs.

 *********************************************************************/

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
#include <io.h>
#include <main.h>


/************************************************
Function name: create_graph

inputs: 
    t_task *task
    t_task_interface *task_interface
output:
      int *succ_adj_mat (2-D array)

This function creates an adjacency matrix from the tasks' 
list and the interface list of the tasks. It also adds a
source node and a sink node based on the primary inputs and
the primary outputs.
 ************************************************/

int create_graph(t_task *task,
                 t_task_interface *task_interface,
                 int *succ_adj_mat){
    int err = 0;
    int num_nodes = task->width;
    int i = 0, j = 0;
    //  int *dummy;

    //matrix size is (num_nodes+2) x (num_nodes+2) because of 
    //the source and the sink nodes

    //initialize the matrix to all zeros
    for(i = 0; i <= num_nodes + 1; i++)
        for(j = 0; j <= num_nodes + 1; j++)
            *(succ_adj_mat + (num_nodes + 2) * i + j) = 0;

    //connect edges from the source node to the node having primary inputs
    for(j = 1; j <= num_nodes; j++)
        if(((task_interface + (task + j)->input1)->mode) == __INPUT
           || ((task_interface + (task + j)->input2)->mode) == __INPUT)
            *(succ_adj_mat + j) = 1;

    //connect edges from the sink node to the node having primary outputs
    for(j = 1; j <= num_nodes; j++)
        if(((task_interface + (task + j)->output)->mode) == __OUTPUT)
            *(succ_adj_mat + j * (num_nodes + 2) + (num_nodes + 1)) = 1;

    //connect the intermediate edges - register edges
    for(i = 1; i < num_nodes + 1; i++){
        for(j = 1; j < num_nodes + 1; j++){
            t_task *i_node = task + i;
            t_task *j_node = task + j;

            if(i_node->output == j_node->input1
               || i_node->output == j_node->input2)
                *(succ_adj_mat + (num_nodes + 2) * i + j) = 1;
        }
    }

#ifdef __DEBUG
    //display the task info
    display_task(task, task_interface);

    int dim_size[2];

    *dim_size = num_nodes + 2;
    *(dim_size + 1) = num_nodes + 2;

    //display the array
    printf("The successor adjacency matrix\n");
    display_array(succ_adj_mat, 2, dim_size);
#endif

    return err;
}


int create_reuse_mat(t_task *task,
                     int *reuse_mat){
    int err = 0;
    int num_nodes = task->width;
    int i = 0, j = 0;

    //matrix size is (num_nodes+2) x (num_nodes+2) because of 
    //the source and the sink nodes

    //initialize the matrix to all zeros
    for(i = 0; i <= num_nodes + 1; i++)
        for(j = 0; j <= num_nodes + 1; j++)
            *(reuse_mat + (num_nodes + 2) * i + j) = 0;

    //if two nodes have the same type they can exploit module reuse
    for(i = 1; i < num_nodes + 1; i++){
        for(j = 1; j <= i; j++){
            t_task *i_node = task + i;
            t_task *j_node = task + j;

            if(i_node->type == j_node->type){
                *(reuse_mat + (num_nodes + 2) * i + j) = 1;
                *(reuse_mat + (num_nodes + 2) * j + i) = 1;
            }
        }
    }

#ifdef __DEBUG
    int dim_size[2];

    *dim_size = num_nodes + 2;
    *(dim_size + 1) = num_nodes + 2;

    //display the array
    printf("The module reuse matrix\n");
    display_array(reuse_mat, 2, dim_size);
#endif

    return err;
}
