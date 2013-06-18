/*
 * argparse.c
 *
 *  Created on: Jun 11, 2013
 *      Author: aalwatta
 */

#ifndef ARGPARSE_C_
#define ARGPARSE_C_


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "argparse.h"
#include "data.h"
#include "PlatformConfig.h"

void displayVersion()
{
	fprintf(stdout,"Scheduler Simulator Version [%s] \n ",VERSION_NUMBER);
	exit(EXIT_SUCCESS);
}

/*
	 * * FIXME  THIS FUNCTION NEED TO BE CHANGED COMPLETELY
	 * Does not check any error conditions,
	 * Does not check getting the right number of nodes,
	 * Need to be in this order..
	 * -n 10 -d 30 -p 33 -u 0 -s 233 -t 1 2 3 4 5
	 */

  int  parseArgs(int argc, char *argv[],struct ArgData * argdatar){


	   int index;
	   int c;
	   opterr = 0;
  argdatar->TaskTypeFlag=0;
	   while ((c = getopt(argc, argv, "d:n:t:p:u:s:v")) != -1)
		   switch (c) {
		   case 'n':
			   argdatar->NodesNo = atoi(optarg);
			   break;
		   case 'd':
			   argdatar->DFGNo = atoi(optarg);
			   break;

		   case 'p':
			   argdatar->PRRsNo = atoi(optarg);
			   break;
		   case 'u':
			   argdatar->Uniformity = atoi(optarg);
			   break;
		   case 's':
			   argdatar->Sync = atoi(optarg);
			   break;
		   case 'V':
		   case 'v':
			   displayVersion();
			   break;
		   case 't':

			   argdatar->TaskTypeFlag = 1;
			   break;

		   case '?':
			   if (optopt == 'n' || optopt == 'd' || optopt == 'p' || optopt == 'u'
					   || optopt == 's' || optopt == 't')
				   fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			   else if (isprint (optopt))
				   fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			   else
				   fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			   return 1;
		   default:
			   abort();
		   }

//	   printf(
//			   "Nodes = %d, PRR = %d, Uniformity = %d , Sync= %d , DFG = %d , TaskTypes = %d \n",
//			   argdatar->NodesNo, argdatar->PRRsNo, argdatar->Uniformity,
//			   argdatar->Sync, argdatar->DFGNo, argdatar->TaskTypeFlag);
	   int i = 0;
	   for (index = optind - 1; index < argc; index++, i++) {
		   argdatar->Tasktypes[i] = atoi(argv[index]);
		  // setNodeTaskType(dFG,i,argdatar);
		 //  fprintf(stderr,"data  %d   [%d] \n", argdatar->Tasktypes[i], atoi(argv[index]));

	   }
	   return 0;
   }

  void InitArgdatar(struct ArgData *arg)
  {
  	arg->DFGNo=0;
  	arg->NodesNo=0;
  	arg->PRRsNo=0;
  	arg->TaskTypeFlag=0;
//  	arg->Tasktypes;
  	arg->Uniformity=0;
  }

#endif /* ARGPARSE_C_ */
