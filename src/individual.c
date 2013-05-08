/*******************************************************************************
 * FILE NAME : Individual.c
 * 
 * Genetic Algorithm practice for Ahmed Al-Watter
 * 
 * PURPOSE : library for the representation and manipulation of a possible
 *      solution to the problem (an individual in the population)
 * 
 * AUTHOR : Jennifer Winer
 * Email  : jwiner@uoguelph.ca
 * 
 * DATE CREATED : May 7, 2013
 * LAST MODIFIED : May 8, 2013
 ******************************************************************************/

/* NOTES
 * - each gene will correspond to one task (eg. addition, multiplication, etc.)
 * - each allele will correspond to a specific hardware implementation of that task
 *      - the allele will be an integer = the row number of a table provided
 */

/* PRECONDITIONS  
 * 
 * - the file that contains the table of values (areas, times, and power) has been
 *      previously initialized and set in the #DEFINE statements
 */

#include "individual.h"

