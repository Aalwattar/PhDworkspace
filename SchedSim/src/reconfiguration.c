/*
 * reconfiguration.c
 *
 *  Created on: 2013-05-23
 *      Author: aalwatta
 */
#include <stdio.h>
#include <stdlib.h>
#include "processors.h"


static int Reconfiguring =NO;
static int ReconfigPRRID=0;
static struct NodeData ND;
static int LoadTask=NO;
static struct Processor *PE;


int IsReconfiguring(void)
{
		return Reconfiguring;
}


int Reconfigure(struct Processor *processor, int PRRID, unsigned long time)
{
	if ( Reconfiguring )
	{
		printf("ERROR [Reconfigure] .. Reconfiguration is in process (ONLY one reconfiguration at a time is allowed) \n");
		return -1;
	}
	if ( processor->Type!=HW )
	{
		printf("ERROR [Reconfigure] You cannot reconfigure a GPP (Are  you sure you know what you are doing !!!)  \n");
		return -2;
	}


	if ( processor->Busy )
	{
		printf("WARNING [Reconfigure]  the PRR is currently Busy  \n");
		return -3;
	}


	ReconfigPRRID=PRRID;
	Reconfiguring = YES;

	processor->ConfigCount=time;
	processor->ExecCount= 0;
	processor->Busy=YES;
	return 0;


}

int ReconfignLoad(struct Processor *processor, int PRRID, unsigned long time, struct NodeData nd)
{
	Reconfigure(processor,PRRID,time);

	/*
	 * TODO  move all this mess to one static structure.
	 */
	printf("reconfiguring Task [%d] on PRR[%d]\n",nd.TaskID,PRRID);
	PE=processor;
	ND.ExecCount=nd.ExecCount;
	ND.Module=nd.Module;
	ND.TaskID=nd.TaskID;
	LoadTask=YES;

	return EXIT_SUCCESS;

}


int TickConfiguration( struct Processor *processor)
{
		if ( processor->Type !=HW)
	{
		printf("ERROR[TickReconfiguration] You cannot reconfigure a GPP (Are  you sure you know what  you are  doing !!!) %d  \n", processor->Type);
		return -2;
	}
	if (processor->Busy && !processor->ExecCount && processor->ConfigCount)
	{
		if(!(--processor->ConfigCount))
		{
			fprintf(stderr, "finish reconfiguration \n");
		processor->Busy=NO;
		Reconfiguring=NO;

			if (LoadTask)
			{
				LoadTask=NO;
				LoadProcessor(PE,ND);
			}

		}
		return processor->ConfigCount;

	}

	 return -1;
}

int TickConfig(struct Processor *processor)
{
	/*
	 * TODO check if there is configuration in the first place
	 * if not exit immediately.
	 * We need  a way to calculate the number reconfiguration cycle for each task
	 */

		if(!TickConfiguration(processor+ReconfigPRRID))
			{
				printf("Task Reconfiguration on PRR[%d] is completed \n", ReconfigPRRID);
				return ReconfigPRRID;
		//	task completed TODO call a function that represent the complete of this task
			}



	return -1;
}
