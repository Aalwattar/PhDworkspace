/*
 * dfggen.h
 *
 *  Created on: Jun 20, 2012
 *      Author: Ahmed Al-Wattar
 */

#ifndef DFGGEN_H_
#define DFGGEN_H_
#define MAX_TYPES_NO 128


struct genmatrix
{
	unsigned int **matrix;
	unsigned char *hasRelation;
	unsigned int *types;
	unsigned int typesNo;
	int size ;
	int dim;
};


struct group
{
	double percentage[MAX_TYPES_NO]; // percentage of occurrence of that group
	int size; // number of groups
	int ID; // currently not used
};
struct type
{
	int id;	// no used
	int groupID; // is the index to the percentage array in group struct
	char color[50];
	char name[50];
};
struct Types
{
	struct type task[MAX_TYPES_NO];
	int size;
	struct group groups;


};

extern const struct Types GTasksTypes;


void writeheader(FILE *fp,char *dfgFileName);
void initGenMatrix(struct genmatrix * matstruct, int size , int dim,unsigned int typesNo);
void freeGenMatrix(struct genmatrix * matstruct);
void genTasksMatrix(struct genmatrix * matstruct, int relationNo);
void writeDFG(FILE *fp, char *dfgFileName,struct genmatrix * matstruct);
void typeStat(struct genmatrix * matstruct);

#endif /* DFGGEN_H_ */
