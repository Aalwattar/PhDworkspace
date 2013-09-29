/*
 * dfggen.c
 *
 *  Created on: Jun 20, 2012
 *      Author: Ahmed Al-Wattar
 */



/*
 * DFG automatically generated file,
 * Created by a tasksgenerated written by
 * 			Ahmed Al-Wattar
 * 		 	aalwatta@uoguelph.ca
 */

#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "tasksTemplate.h"
#include "dfggen.h"
#include <string.h>
#include "genConfig.h"
#include "parseArgs.h"



#define HEADER_COMMENT	" \n/*" \
						"* DFG automatically generated file, \n" \
						 "* Created by a tasksgenerated written by\n" \
						 "* 			Ahmed Al-Wattar \n"\
						 "* 		 	aalwatta@uoguelph.ca \n" \
						 "*/ \n\n "

#define HEADER0	 	 	 "#ifndef DFGTEMPLATE_H_ \n " \
			 	 	 	 "#define DFGTEMPLATE_H_ \n\n"\
						 "#include \"data.h\" \n\n"
//#define  HEADER1(value)	"/* DONT FORGET TO ADD (#define LAST_NODE %d ) in platform_config.h */" , (value)

#define HEADER1(value) 	 "\n\t{\n\t .size=%d, \n",(value)
#define HEADER2  	"\n\t .dfg  = {\n\n"
#define FOOTER1  	"#endif /* DFGTEMPLATE_H_ */\n"
//struct type
//{
//	int id;
//	float percentage;
//	char color[50];
//	char name[50];
//};
/*FIXME fix this and change the size to the right global variable
 * TODO Try to move this to a configuration file or something.
 *
 */
const struct Types GTasksTypes=
		{.size=MAX_TASKS_TYPES ,
				.task[0]={.id=0, .groupID=0,.color="bisque1",.name="T1"},
				.task[1]={.id=1, .groupID=0,.color="green",.name="T2"},
				.task[2]={.id=2, .groupID=0,.color="cyan",.name="T3"},
				.task[3]={.id=3, .groupID=0,.color="pink",.name="T4"},
				.task[4]={.id=4, .groupID=0,.color="blueviolet",.name="T5"},
				.task[5]={.id=5, .groupID=0,.color="coral1",.name="T6"},
				.task[6]={.id=6, .groupID=0,.color="firbrick1",.name="T7"},
				.task[7]={.id=7, .groupID=0,.color="darkolivegreen3",.name="T8"},
				.task[8]={.id=0, .groupID=0,.color="coral3",.name="T9"},
				.task[9]={.id=1, .groupID=0,.color="gray50",.name="T10"},
				.task[10]={.id=2, .groupID=0,.color="grey100",.name="T11"},
				.task[11]={.id=3, .groupID=0,.color="hotpink3",.name="T12"},
				.task[12]={.id=4, .groupID=0,.color="mediumspringgreen",.name="T13"},
				.task[13]={.id=5, .groupID=0,.color="slateblue2",.name="T14"},
				.task[14]={.id=6, .groupID=0,.color="tomato2",.name="T15"},
				.task[15]={.id=7, .groupID=0,.color="yellow3",.name="T16"},



				.groups={.size=1,
						.percentage[0]=1.0,
						.percentage[1]=0.0,
						.percentage[2]=0.0,
						.percentage[3]=0.0

				},

		};



void writeheader(FILE *fp, char *dfgFileName)
{ 
    extern struct globalArgs globalArgs;
    int error;
	error =fprintf(fp,HEADER_COMMENT);
	if (error<0)
	{
		fprintf(stderr, " Error writing header to the DFG file \n EXITING ...." );
		exit(1);
	}
	//fprintf(fp,HEADER0);
	fprintf(fp,"\t /*  Do NOT forget to add #include \"templates/%s\"   \n "
			"\t and copy the generated .txt file to the template directory */ \n",dfgFileName+2); // to remove the ./ at the beginning of the filename
    
	fprintf(fp,HEADER1(globalArgs.no_of_nodes));
	fprintf(fp,HEADER2);
}


void initGenMatrix(struct genmatrix * matstruct, int size , int dim,unsigned int typesNo)
{  int i;
	matstruct->matrix= (unsigned int **) calloc(size,sizeof (unsigned int*));
	if(matstruct->matrix==NULL)
	{
		fprintf(stderr,"initmatrix: Error in allocating memory\n Exiting .. \n");
		exit(1);
	}
	matstruct->hasRelation= (unsigned char *) calloc(size,sizeof (unsigned char));
	if(matstruct->hasRelation==NULL)
	{
		fprintf(stderr,"initmatrix: Error in allocating memory\n Exiting .. \n");
		exit(1);
	}
	matstruct->types= (unsigned int *) calloc(size,sizeof (unsigned int));
	if(matstruct->types==NULL)
	{
		fprintf(stderr,"initmatrix: Error in allocating memory\n Exiting .. \n");
		exit(1);
	}
	matstruct->size=size;
	matstruct->dim=dim;
	matstruct->typesNo=typesNo;
    for (i=0;i<size;i++)
    {

    	matstruct->matrix[i] =calloc(2,sizeof (unsigned int));
    	if(matstruct->matrix[i]==NULL)
    	{
    		fprintf(stderr,"initmatrix: Error in allocating memory matrix[i]\n exiting \n ");
    		exit(1);
    	}

    }
}


void freeGenMatrix(struct genmatrix * matstruct)
{ 	int i;
 	 int size;
 	 size=matstruct->size;
    for (i=0;i<size;i++)
    {
    	free(matstruct->matrix[i]);
    }
    free(matstruct->matrix);
    free(matstruct->hasRelation);
    free(matstruct->types);
}


void gentypes(gsl_rng *r,struct genmatrix * matstruct)
{	int i,k,found=0;
	int taskType;
	int tmpCount=0;
	double randNo;
  	 int *memberPerGroup=(int*)calloc(GTasksTypes.groups.size,sizeof(int ));
  	int *groupOffset=(int*)calloc(GTasksTypes.groups.size,sizeof(int ));
	//for (i=0 ;i < GTasksTypes.size;i++)
  	for (i=0 ;i < matstruct->typesNo;i++)
	{
		memberPerGroup[GTasksTypes.task[i].groupID]++;
	}
	// calculate offset
	for (k=0; k<GTasksTypes.groups.size;k++)
	{

		groupOffset[k]= tmpCount;
		tmpCount+=memberPerGroup[k];
	}


	for (i=0;i < matstruct->size ;i++)
	{
		do {found=0;
		randNo=gsl_rng_uniform(r);
			for (k=0; k<GTasksTypes.groups.size;k++)
			{
				if ( randNo<=GTasksTypes.groups.percentage[k])
				{
					if (memberPerGroup[k]==1)
					{
						taskType=groupOffset[k];
						found=1;
//					printf("[1]  type %d \n",taskType);
						break;
					} else if(memberPerGroup[k]==2)
					{
						taskType=groupOffset[k]+gsl_ran_bernoulli(r,0.5);
						found=1;
//						printf("[2]  type %d \n",taskType);
						break;
					} else if (memberPerGroup[k]>2)
					{
						taskType=groupOffset[k]+gsl_rng_uniform_int(r,memberPerGroup[k]);
						found=1;
//						printf("[>2]  type %d \n",taskType);
						break;
					} else
					{
					  fprintf(stderr,"gentypes: ERROR GROUP ID check your groups\n EXITING \n");
					  exit(1);
					}

				}

			}

		}while(!found);

		//matstruct->types[i]=gsl_rng_uniform_int(r,GTasksTypes.size+1);

		matstruct->types[i]=taskType;
		//printf("type %d \n",matstruct->types[i]);
	}

	free(memberPerGroup);
}

void genTasksMatrix_new(struct genmatrix * matstruct, int relationNo)
	{
	int relationCount =0;
	int k,l,onoff;

    const gsl_rng_type * T;
    gsl_rng * r;

    gsl_rng_env_setup();

    T = gsl_rng_default;
    r = gsl_rng_alloc (T);


	while(relationCount<relationNo)
	{
		k=gsl_rng_uniform_int(r,matstruct->size);
		l=gsl_rng_uniform_int(r,matstruct->size);
		onoff=gsl_ran_bernoulli(r,0.5);
		if(l-k>0)
		{
			if (matstruct->dim==1)
			{
				if (matstruct->matrix[l][0]) continue;
				matstruct->matrix[l][0]=k+1;
				//printf("[%d] tasks[%d]= %d\n ",relation,l,k);
	    		   if(matstruct->hasRelation[matstruct->matrix[l][0]-1]==0)
	    			   matstruct->hasRelation[matstruct->matrix[l][0]-1]=1;
				relationCount++;

			}else if (matstruct->dim==2)
			{
				if (matstruct->matrix[l][0] && matstruct->matrix[l][1]) continue;

				if (!matstruct->matrix[l][0] && !matstruct->matrix[l][1])
				{

					matstruct->matrix[l][onoff]=k+1;
				} else if (matstruct->matrix[l][0])
				{
					if (matstruct->matrix[l][0]==k+1) continue;
					matstruct->matrix[l][1]=k+1;
				}else if (matstruct->matrix[l][1])
				{
					if (matstruct->matrix[l][1]==k+1) continue;
					matstruct->matrix[l][0]=k+1;
				} else
				{
					puts(" genTaskMatrix:Something went horribly wrong !! \n exiting");
					exit(1);
				}


				if (matstruct->matrix[l][0])
	    		   if(matstruct->hasRelation[matstruct->matrix[l][0]-1]==0){
	    			   matstruct->hasRelation[matstruct->matrix[l][0]-1]=1;
	    		   }

				if (matstruct->matrix[l][1])
	    		   if(matstruct->hasRelation[matstruct->matrix[l][1]-1]==0){
	    		   }

				relationCount++;

			}else
			{
				fprintf(stderr,"Dimensions other than 1 and 2 is not supported ! \n"
						"This has to function has to be changed it's so dumb bud I didn't have time\n"
						"Exiting ...\n");
				exit(1);
			}
		}

	}

	gentypes(r,matstruct);


	gsl_rng_free (r);

}

/*
 * TODO
 *  	Change this dumb function to a unified multi-dimensions generator!
 */
void genTasksMatrix(struct genmatrix * matstruct, int relationNo, unsigned long int seed )
	{
	int relationCount =0;
	int k,l,onoff;

    const gsl_rng_type * T;
    gsl_rng * r;

    gsl_rng_env_setup();

    T = gsl_rng_default;
    T=gsl_rng_knuthran2;
    r = gsl_rng_alloc (T);

    gsl_rng_set(r,seed);

    printf ("generator type: %s\n", gsl_rng_name (r));



	while(relationCount<relationNo)
	{
		k=gsl_rng_uniform_int(r,matstruct->size);
		l=gsl_rng_uniform_int(r,matstruct->size);
		onoff=gsl_ran_bernoulli(r,0.5);
		if(l-k>0)
		{
			if (matstruct->dim==1)
			{
				if (matstruct->matrix[l][0]) continue;
				matstruct->matrix[l][0]=k+1;
				//printf("[%d] tasks[%d]= %d\n ",relation,l,k);
	    		   if(matstruct->hasRelation[matstruct->matrix[l][0]-1]==0)
	    			   matstruct->hasRelation[matstruct->matrix[l][0]-1]=1;
				relationCount++;

			}else if (matstruct->dim==2)
			{
				if (matstruct->matrix[l][0] && matstruct->matrix[l][1]) continue;

				if (!matstruct->matrix[l][0] && !matstruct->matrix[l][1])
				{

					matstruct->matrix[l][onoff]=k+1;
				} else if (matstruct->matrix[l][0])
				{
					if (matstruct->matrix[l][0]==k+1) continue;
					matstruct->matrix[l][1]=k+1;
				}else if (matstruct->matrix[l][1])
				{
					if (matstruct->matrix[l][1]==k+1) continue;
					matstruct->matrix[l][0]=k+1;
				} else
				{
					puts(" genTaskMatrix:Something went horribly wrong !! \n exiting");
					exit(1);
				}


				if (matstruct->matrix[l][0])
	    		   if(matstruct->hasRelation[matstruct->matrix[l][0]-1]==0){
	    			   matstruct->hasRelation[matstruct->matrix[l][0]-1]=1;
	    		   }

				if (matstruct->matrix[l][1])
	    		   if(matstruct->hasRelation[matstruct->matrix[l][1]-1]==0){
	    		   }

				relationCount++;

			}else
			{
				fprintf(stderr,"Dimensions other than 1 and 2 is not supported ! \n"
						"This has to function has to be changed it's so dumb bud I didn't have time\n"
						"Exiting ...\n");
				exit(1);
			}
		}

	}

	gentypes(r,matstruct);


	gsl_rng_free (r);

}

int updatefield_i(char *src,char * field,int value)
{ char *strtmp2;
char strvalue[10];
//strtmp2=malloc(sizeof(char)* strlen(src));
	strtmp2=strstr(src,field);

	sprintf(strvalue,"%3d",value);
	if (strtmp2==NULL)
	{
			puts("updatefield: String not found !!");
			return -1;
	}

	memcpy (strtmp2,strvalue,strlen(strvalue));

 //free(strtmp2);
  return 0;
}

int updatefield_s(char *src,char * field,char * value)
{ char *strtmp2;
//	strtmp2=malloc(sizeof(char)* strlen(src));
	strtmp2=strstr(src,field);

	memcpy (strtmp2,value,strlen(value));

	// free(strtmp2);
  return 0;
}

void writeDFG(FILE *fp,char *dfgFileName, struct genmatrix * matstruct, struct Nodes *dFG, unsigned long seed)
{ int i;
	char strtmp[1024];
	writeheader(fp,dfgFileName);
	fprintf(fp,"/* SEED = %lu */\n\n", seed);
	for (i=0;i<matstruct->size;i++)
		{
		//strtmp=(char *)malloc(sizeof(char)* strlen(TasksTemplate[i]));
			strcpy(strtmp,TasksTemplate[matstruct->types[i]]);
			dFG[i].TypeID=matstruct->types[i];
			updatefield_i(strtmp,F_ID,i);
			if (i==matstruct->size-1){
				updatefield_i(strtmp,F_NEXT,0);
				dFG[i].next=0;
			}
			else{
				updatefield_i(strtmp,F_NEXT,i+1);
				dFG[i].next=i+1;
			}
		//	updatefield_i(strtmp,F_OP1,matstruct->matrix[i][0]?matstruct->matrix[i][0]:i+matstruct->types[i]);
		//	updatefield_i(strtmp,F_OP2,matstruct->matrix[i][1]?matstruct->matrix[i][1]:i+matstruct->types[i]);
			updatefield_i(strtmp,F_OP1,matstruct->matrix[i][0]?matstruct->matrix[i][0]-1:2);
			dFG[i].D.op1=matstruct->matrix[i][0]?matstruct->matrix[i][0]-1:2;
			updatefield_i(strtmp,F_OP2,matstruct->matrix[i][1]?matstruct->matrix[i][1]-1:1);
			dFG[i].D.op2=matstruct->matrix[i][1]?matstruct->matrix[i][1]-1:1;
			updatefield_s(strtmp,F_ISADD_OP1,matstruct->matrix[i][0]?"YES":" NO");
			dFG[i].D.isAdd_op1=matstruct->matrix[i][0]?1: 0;
			updatefield_s(strtmp,F_ISADD_OP2,matstruct->matrix[i][1]?"YES":" NO");
			dFG[i].D.isAdd_op2=matstruct->matrix[i][1]?1: 0;
			fprintf(fp,strtmp);
			fprintf(fp,",\n\n");
		//	free(strtmp);

		}

		fprintf(fp,"\t\t},\n\t},\n");
		//fprintf(fp,FOOTER1);
}

void typeStat(struct genmatrix * matstruct)
{ int i;
	int *memberPerType=(int *)calloc(GTasksTypes.size,sizeof(int));
for (i=0;i < matstruct->size ;i++)
	{
		memberPerType[matstruct->types[i]]++;
//		printf(" task[%d] type [%d]\n",i,matstruct->types[i]);
	}
//for (i=0;i <GTasksTypes.size ;i++)
	for (i=0;i <matstruct->typesNo ;i++)
{
	printf("Tasks[%d] repeated [%d] >> [%d]%%\n"
			,i,memberPerType[i],((memberPerType[i]*100)/matstruct->size));
}
free(memberPerType);
}
