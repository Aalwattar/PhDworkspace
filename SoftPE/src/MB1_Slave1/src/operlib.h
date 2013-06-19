/*
 * operlib.h
 *
 *  Created on: Jun 1, 2011
 *      Author: ahmed
 */

#ifndef OPERLIB_H_
#define OPERLIB_H_

#define ADDER_DELAY  10000000LU
#define SUB_DELAY    20000000LU
#define MUL_DELAY    70000000LU
#define DIV_DELAY    20000000LU
#define SHIFTR_DELAY 20000000LU
#define NEG_DELAY 1500

int addI(int , int , unsigned long);

int subI(int , int, unsigned long );
int mulI(int , int , unsigned long);
int divI(int , int , unsigned long);
int shlI(int , int , unsigned long);
int negI(int , unsigned long);


#endif /* OPERLIB_H_ */
