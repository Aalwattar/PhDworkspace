/*
 * drawTaskAllocation.h
 *
 *  Created on: Jun 7, 2013
 *      Author: aalwatta
 */

#ifndef DRAWTASKALLOCATION_H_
#define DRAWTASKALLOCATION_H_
#include "data.h"
enum SchedState {
	Empty, Reconfig, Exec
};

struct Draw {
	enum SchedState mode;
	unsigned int Type;
	unsigned int ID;

};



struct Draw ** CreateDraw(unsigned int time, int noPE);
void CleanDraw(struct Draw ** graph, unsigned int time);
unsigned int CheckMaxSize(struct node *dFG) ;
void DrawGraph(struct Draw **graph, unsigned int time, FILE *fd, int noPRR, int noGPP);
void GenerateGraph(struct Draw **graph, struct node *dFG, int scale);
void InitDraw(struct Draw ** graph, unsigned int time, int noPE);

#endif /* DRAWTASKALLOCATION_H_ */
