/*
 * argparse.h
 *
 *  Created on: Jun 11, 2013
 *      Author: aalwatta
 */

#ifndef ARGPARSE_H_
#define ARGPARSE_H_

#define MAX_NUMBER_NODES 500
struct ArgData {
	int NodesNo;
	int Tasktypes[MAX_NUMBER_NODES];
	int PRRsNo;
	int Uniformity;
	int Sync;
	int TaskTypeFlag;
	int DFGNo;

};


int  parseArgs(int argc, char *argv[],struct ArgData * argdatar);
#endif /* ARGPARSE_H_ */
