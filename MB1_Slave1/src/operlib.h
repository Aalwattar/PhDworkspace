/*
 * operlib.h
 *
 *  Created on: Jun 1, 2011
 *      Author: ahmed
 */

#ifndef OPERLIB_H_
#define OPERLIB_H_

#define ADDER_DELAY  2000
#define SUB_DELAY   2500
#define MUL_DELAY   4000
#define DIV_DELAY   5000
#define SHIFTR_DELAY 2100
#define NEG_DELAY 1500

int addI(int , int );

int subI(int , int );
int mulI(int , int );
int divI(int , int );
int shlI(int , int );
int negI(int );


#endif /* OPERLIB_H_ */
