/*
 * utility.c
 *
 *  Created on: Jun 20, 2012
 *      Author: Ahmed Al-Wattar
 */

#include<stdio.h>
#include <stdlib.h>

FILE * openfile ( char * filename, char * openmode )
{
	FILE *fptmp=fopen(filename, openmode);
	if ( fptmp== NULL)
	{
		fprintf(stderr," Error opening File %s for [%s]\n\n Exiting ... \n\n",filename,openmode);
		exit(1);
	}
	return fptmp;
}
