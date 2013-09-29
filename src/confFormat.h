/*
 ============================================================================
 Name        : confFormat.h
 Author      : Ahmed Al-Wattar
 Version     :
 Copyright   : Your copyright notice
 Description : convert the dfg to libconfuse's .conf format
 ============================================================================
 */

#ifndef CONF_FORMAT_H
#define CONF_FORMAT_H

#define BUFF_SIZE 256

enum DepType {
	REG = 'r', OUT = 'o', INP = 'c'
};

struct output {
	int node;
	char * nodeName[10];
	int dep_id;
};

struct iotype {
	enum DepType type;
	int node;
};

struct confNode {
	struct iotype input[2];
	struct iotype output;
	int size;
};

struct nodeData {
	int op1;
	int op2;
	unsigned isAdd_op1 :1;
	unsigned isAdd_op2 :1;
	unsigned :0;
};
struct Nodes{

		 unsigned int next;
		 struct nodeData D;
		 int TypeID;
};
int writeConf(char *, struct Nodes *, unsigned long);

#endif
