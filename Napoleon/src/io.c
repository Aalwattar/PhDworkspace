/*************************************************************************/
// Author  : Ziad Abuowaimer
// Version : 0.1
// Project : A DFG Off-Line Task Scheduler for FPGA
// Team    : Ziad Abuowaimer
/*************************************************************************/

/*************************************************************************
  Compile with -D__DEBUG option for enabling debugging jigs in the code

  Filename: io.c

  Description:
  Parses the input file in AIF and creates the initial data structures
  Memory allocation of the data structures is done in this routine
    1. File pointer is always non-NULL.



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
#include <main.h>

short int task_type(char *);
short int intfc_mode(char *);
short int get_index(char *, t_task_interface *, short int);
void print_error(int);

void print_error(int ecode) {
    char err_mesg[__SIZE_MAX_BUFF];

    switch (ecode) {
        case __LESSARGS:
            strcpy(err_mesg, "Insufficient number of arguments.");
            break;
        case __AIF_FILE:
            strcpy(err_mesg, "Opening AIF file.");
            break;
        case __LOG_FILE:
            strcpy(err_mesg, "Opening Log file.");
            break;
        case __OPT_FILE:
            strcpy(err_mesg, "Opening Options file.");
            break;
        case __RES_FILE:
            strcpy(err_mesg, "Opening Resources file.");
            break;
        case __UNRECOPT:
            strcpy(err_mesg, "Unrecognised option at the command line.");
    }
    //fprintf(log_strm, "Error: %s\n", err_mesg);
    fprintf(stderr, "Error: %s\n", err_mesg);
    //fclose(log_strm);
    exit(ecode);
}

int parse_res(FILE *in_strm, t_task_type *task_type) {
    int err = 0;
    short int index = 0, count_task_type = 0;
    char buff[__SIZE_MAX_BUFF], *token;

    count_task_type = 0;

    //read through the lines of the resources file
    //fprintf(log_strm, "Reading the resources file...\n");

    while (fgets(buff, __SIZE_MAX_BUFF, in_strm)) {
        //fprintf(log_strm, "%s\n", buff);
        token = strtok(buff, " ");

        if (!(*token) || *token == '#' || *token == '\n')
            continue;
        else if (!strcasecmp(token, "TASK1"))
            index = __ADD;
        else if (!strcasecmp(token, "TASK2"))
            index = __MULT;
        else if (!strcasecmp(token, "TASK3"))
            index = __SUB;
        else if (!strcasecmp(token, "TASK4"))
            index = __DIV;
        else if (!strcasecmp(token, "TASK5"))
            index = __LT;
        else if (!strcasecmp(token, "TASK6"))
            index = __GT;
        else if (!strcasecmp(token, "TASK7"))
            index = __LTE;
        else if (!strcasecmp(token, "TASK8"))
            index = __GTE;
        else
            err = __RESFILE0;

        if (count_task_type < index)
            count_task_type = index;

        if (token = strtok(NULL, " ")) {
            (task_type + index)->columns = atoi(token);
        } else {
            err = __RESFILE1;
            break;
        }
        if (token = strtok(NULL, " ")) {
            (task_type + index)->rows = atoi(token);
        } else {
            err = __RESFILE1;
            break;
        }
        if (token = strtok(NULL, " ")) {
            (task_type + index)->latency = atoi(token);
        } else {
            err = __RESFILE2;
            break;
        }
        if (token = strtok(NULL, " \n")) {
            (task_type + index)->reconfig_time = atoi(token);
        } else {
            err = __RESFILE3;
            break;
        }
    }

    // the first element is reserved for storing the number of elements
    // in the list.

    task_type->columns = count_task_type;

    return (err);
}

int parse_aif(FILE *in_strm, t_task *task, t_task_interface *task_interface) {
    int err = 0;
    short int count_intfc = 0, count_task = 0;
    t_task *task_curr;
    t_task_interface *task_interface_curr;
    char buff[__SIZE_MAX_BUFF], *token;

    count_intfc = 1;
    count_task = 1;
    task_curr = task + count_task;
    task_interface_curr = task_interface + count_intfc;

    //fprintf(log_strm,"Reading the aif file...\n");
    while (fgets(buff, __SIZE_MAX_BUFF, in_strm)) {
        token = strtok(buff, " \n");
        if (!strcasecmp(token, "inputs")
                || !strcasecmp(token, "outputs")
                || !strcasecmp(token, "regs")) {
            short int mode = intfc_mode(token);
            while (token = strtok(NULL, " \n")) {
                strcpy(task_interface_curr->name, token);
                task_interface_curr->mode = mode;
                if (token = strtok(NULL, " \n"))
                    task_interface_curr->width = atoi(token);
                count_intfc++;
                task_interface_curr = task_interface + count_intfc;
            }
        } else if (!strcasecmp(token, "end")) {
            break;
        } else {
            short int task_attrib = 0;
            do {
                // ERROR in AIF FORMAT
                assert(task_attrib <= 6);
                switch (task_attrib) {
                    case 0: //Task name
                        strcpy(task_curr->name, token);
                        break;
                    case 1: //Task type
                        task_curr->type = task_type(token);
                        break;
                    case 2: //Task width
                        task_curr->width = atoi(token);
                        break;
                    case 3: //input 1
                        task_curr->input1 = get_index(token,
                                task_interface, count_intfc - 1);
                        break;
                    case 4: //input 2
                        task_curr->input2 = get_index(token,
                                task_interface, count_intfc - 1);
                        break;
                    case 5: //output
                        task_curr->output = get_index(token,
                                task_interface, count_intfc - 1);
                        break;
                }
                task_attrib++;
            } while (token = strtok(NULL, " \n"));
            count_task++;
            task_curr = task + count_task;
        }
    }

    task_interface->mode = __INVALID;
    task_interface->width = count_intfc - 1;
    task->type = __INVALID;
    task->width = count_task - 1;

    return err;
}

short int task_type(char *token) {
    short int type = 0;

    if (!strcasecmp(token, "TASK1")) type = __ADD;
    else if (!strcasecmp(token, "TASK2")) type = __MULT;
    else if (!strcasecmp(token, "TASK3")) type = __SUB;
    else if (!strcasecmp(token, "TASK4")) type = __DIV;
    else if (!strcasecmp(token, "TASK5")) type = __LT;
    else if (!strcasecmp(token, "TASK6")) type = __GT;
    else if (!strcasecmp(token, "TASK7")) type = __LTE;
    else if (!strcasecmp(token, "TASK8")) type = __GTE;

    return type;
}

short int intfc_mode(char *token) {
    short int mode = 0;

    if (!strcasecmp(token, "inputs")) mode = __INPUT;
    else if (!strcasecmp(token, "outputs")) mode = __OUTPUT;
    else if (!strcasecmp(token, "regs")) mode = __REGISTER;

    return mode;
}

short int get_index(char *token, t_task_interface* task_interface, short int count) {
    short int index = 0;
    short int i = 0;
    //t_task_interface *task_interface_curr;

    for (i = 1; i <= count; i++) {
        if (!strcasecmp((task_interface + i)->name, token)) {
            index = i;
            break;
        }
    }

    return (index) ? index : -1;
}

void display_array(short int *start, short int dim, short int *dim_size) {
    short int i = 0, j = 0;

    switch (dim) {
        case 1:
        {
            for (i = 0; i<*dim_size; i++) {
                printf("%d ", *(start + i));
            }
            break;
        }
        case 2:
        {
            for (i = 0; i<*dim_size; i++) {
                for (j = 0; j<*(dim_size + 1); j++) {
                    printf("%d ", *(start + i * (*(dim_size + 1)) + j));
                }
                printf("\n");
            }
            break;
        }
    }

    printf("\n");
}

void display_task(t_task *task, t_task_interface *task_interface) {
    int i = 0;
    short int num_nodes = task->width;

    for (i = 1; i <= num_nodes; i++) {
        printf("%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", (task + i)->name, (task + i)->type, (task + i)->width, (task + i)->latency, (task + i)->reconfig_time, (task + i)->columns, (task + i)->rows, (task + i)->input1, (task + i)->input2, (task + i)->output, (task_interface + (task + i)->output)->mode, (task + i)->exec_sched, (task + i)->reconfig_sched, (task + i)->leftmost_column, (task + i)->bottommost_row);
    }

}

void display_task_type(t_task_type *task_type) {
    int i = 0;
    short int num_task_types = task_type->columns;

    for (i = 1; i < num_task_types; i++) {
        printf("%d %d %d %d %d\n", i, (task_type + i)->columns, (task_type + i)->rows, (task_type + i)->latency, (task_type + i)->reconfig_time);
    }

}

void set_task_parameter(t_task *task, t_task_type *task_type) {
    int i = 0;
    short int num_nodes = task->width;

    for (i = 1; i <= num_nodes; i++) {
        (task + i)->columns = (task_type + (task + i)->type)->columns;
        (task + i)->rows = (task_type + (task + i)->type)->rows;
        (task + i)->latency = (task_type + (task + i)->type)->latency;
        (task + i)->reconfig_time = (task_type + (task + i)->type)->reconfig_time;
    }

}

