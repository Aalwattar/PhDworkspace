/*******************************************************************************
 * FILE NAME : problem.h
 * 
 * Genetic Algorithm practice for Ahmed Al-Watter
 * 
 * PURPOSE : a header for problem.c
 * 
 * AUTHOR : Jennifer Winer
 * Email  : jwiner@uoguelph.ca
 * 
 * DATE CREATED : May 7, 2013
 * LAST MODIFIED : May 7, 2013
 ******************************************************************************/

#ifndef PROBLEM_H
#define	PROBLEM_H

#define addTable  "add_table.txt"
#define subTable  "sub_table.txt"
#define multTable "mult_table.txt"
#define divTable  "div_table.txt"

typedef struct architecture{
    double area;
    double runtime;
    double power;
} Architecture; 

void initRandGenerator(int);
double randomNumber(void);

// FILE READING FUNCTIONS

#endif	/* PROBLEM_H */

