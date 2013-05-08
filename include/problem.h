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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef PROBLEM_H
#define	PROBLEM_H

/***********************************************
 * ARCHITECTURE PERFORMANCE INFORMATION STORAGE
 ***********************************************/
#define ARCH_FILENAME "performance_data.txt"

typedef struct {
    double area;
    double runtime;
    double power;
} Architecture; 

typedef struct{
    int num_arch;
    Architecture * arch;
}Operation;

enum operation { a=0, s, m, d };

/********************************
 * RANDOM NUMBER GENERATION
 ********************************/
void seedRandGenerator(int);
void randSeed(void);

double randomNumber(void);

/********************************
 * FILE READING FUNCTIONS
 ********************************/

Operation * initProblem(char *);
Operation * initPerformanceInfo(FILE *);
void initArchInfo(FILE *, Architecture *);

void freePerformanceInfo(Operation *);

void printProblem(Operation *);

#endif	/* PROBLEM_H */

