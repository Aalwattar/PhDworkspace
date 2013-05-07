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
#include <stdbool.h>

#ifndef PROBLEM_H
#define	PROBLEM_H

/***********************************************
 * ARCHITECTURE PERFORMANCE INFORMATION STORAGE
 ***********************************************/
#define arch_filename "performance_data.txt"

typedef struct architecture{
    double area;
    double runtime;
    double power;
} Architecture; 

enum operation { a, s, m, d };
static Architecture ** performance;

/********************************
 * RANDOM NUMBER GENERATION
 ********************************/
void seedRandGenerator(int);
void randSeed(void);

double randomNumber(void);

/********************************
 * FILE READING FUNCTIONS
 ********************************/

void initPerformanceInfo();

#endif	/* PROBLEM_H */

