/*
 * drawDFG.c
 *
 *  Created on: Jun 7, 2013
 *      Author: aalwatta
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "PlatformConfig.h"
#include "drawTaskAllocation.h"
#include "data.h"



void InitDraw(struct Draw ** graph, unsigned int time, int noPE)
{int i,j;
	for (i = 0; i < time; i++) {

			for (j = 0; j < noPE; j++) {

				graph[i][j].ID = 0;
				graph[i][j].Type = 0;
				graph[i][j].mode = Empty;

			}
}
}


struct Draw ** CreateDraw(unsigned int time, int noPE) {
	int i = 0, j;
	struct Draw ** graph;

	graph = (struct Draw **) malloc(sizeof(struct Draw *) * time);
	if (!graph) {
		fprintf(stderr, "ERROR [initDraw]  Error allocating memory \n");
		exit(EXIT_FAILURE);

	}

	for (i = 0; i < time; i++) {
		graph[i] = (struct Draw *) malloc(sizeof(struct Draw) * noPE);
		if (!graph[i]) {
			fprintf(stderr, "ERROR: initDraw  Error allocating memory \n");
			exit(EXIT_FAILURE);

		}
		for (j = 0; j < noPE; j++) {

			graph[i][j].ID = 0;
			graph[i][j].Type = 0;
			graph[i][j].mode = Empty;

		}
	}

	return graph;

}

void CleanDraw(struct Draw ** graph, unsigned int time) {
	int i;
	for (i = 0; i < time; i++) {
		free(graph[i]);
	}
	free(graph);
}

unsigned int CheckMaxSize(struct node *dFG) {
	int i = 0;

	struct Simulation sim;
	unsigned int max = 0;

	do {
		getTaskSimulation(i, &sim);

		if (max < sim.ExecTime.end) {
			max = sim.ExecTime.end;
		}

	} while (dFG[i++].next);

	return max;
}


void DrawGraph(struct Draw **graph, unsigned int time, FILE *fd, int noPRR, int noGPP) {

	int i, j;

	for (j = 0; j < noPRR; j++) {
		if (!j) {
			fprintf(fd, " ");
		}
		fprintf(fd, "\tPRR%d", j);
	}

	for (j = 0; j < noGPP; j++) {
		if (!j) {
			fprintf(fd, "\tGPP%d", j);
		}
	}

	fprintf(fd, "\n");

	for (i = 0; i < time; i++) {
		for (j = 0; j < noPRR; j++) {
			if (!j) {
				fprintf(fd, "%4d--", i);
			}
			if (graph[i][j].mode == Empty) {
				fprintf(fd, "\t.....");
				continue;
			}

			fprintf(fd, "\t%u%s", graph[i][j].ID,
					graph[i][j].mode == Reconfig ? "#####" :
					graph[i][j].mode == Exec ? "*****" : ".....");
		}

		for (j = 0; j < noGPP; j++) {
			if (!j)

				if (graph[i][j].mode == Empty) {
					fprintf(fd, "\t......");
					continue;
				}
			fprintf(fd, "\t%u%s", graph[i][j].ID,
					graph[i][j].mode == Reconfig ? "#####" :
					graph[i][j].mode == Exec ? "*****" : ".....");
		}

		fprintf(fd, "\n");

	}
}

void GenerateGraph(struct Draw **graph, struct node *dFG, int scale) {
	int i = 0, j = 0;
	unsigned int tmpConfigTime;
	unsigned int tmpExecTime;

	struct Simulation sim;

	do {
		getTaskSimulation(i, &sim);

		if (!sim.Reused) {
			tmpConfigTime =  sim.ConfigTime.end - sim.ConfigTime.start;

			for (j = lroundf((float)sim.ConfigTime.start / scale);
					j <lroundf((float)(sim.ConfigTime.start + tmpConfigTime) / scale); j++) {
				graph[j][sim.PRRUsed].ID = i;
				graph[j][sim.PRRUsed].Type = dFG[i].TypeID;
				graph[j][sim.PRRUsed].mode = Reconfig;

			}
		}

		tmpExecTime = sim.ExecTime.end - sim.ExecTime.start;
		for (j = lroundf((float)sim.ExecTime.start / scale);
				j <lroundf((float)(sim.ExecTime.start + tmpExecTime) / scale); j++) {
			graph[j][sim.PRRUsed].ID = i;
			graph[j][sim.PRRUsed].Type = dFG[i].TypeID;
			graph[j][sim.PRRUsed].mode = Exec;
		}

	} while (dFG[i++].next);

}

