/*************************************************************************
 * Author   : Ziad Abuowaimer
 * Modifier : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 * 
 * Created  : May 22, 2013
 * Modified : May 30, 2013
 *************************************************************************/

/*************************************************************************
 * Filename    : io.h
 * 
 * Description : Header file which defines the functions defined in io.c
 *************************************************************************/


#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <types.h>

extern int  parse_aif(FILE *, t_task *, t_task_interface *);
extern int  parse_res(FILE *, t_task_type *);
extern int  parse_impl(FILE *, t_task_impl *);

extern void display_array(int *, int, int *);
extern void display_task(t_task *, t_task_interface *);
extern void display_task_type(t_task_type *);
extern void display_task_impl(t_task_impl *);

extern void set_task_parameter(t_task *, t_task_type *);

extern void print_error(int);

#endif /* IO_H */