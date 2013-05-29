/*
 * reconfiguration.c
 *
 *  Created on: 2013-05-23
 *      Author: aalwatta
 */
#include <stdio.h>
#include "processors.h"


static int Reconfiguring =NO;
static int ReconfigPRRID=0;

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
		printf("WARNING [Reconfigure]  the PRR is currently Busy \n) \n");
		return -3;
	}


	ReconfigPRRID=PRRID;
	Reconfiguring = YES;

	processor->ConfigCount=time;
	processor->ExecCount= 0;
	processor->Busy=YES;
	return 0;


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
		processor->Busy=NO;

		}
		return processor->ConfigCount;

	}

	 return -1;
}

int TickConfig(struct Processor *processor)
{


		if(!TickConfiguration(processor+ReconfigPRRID))
			{
				printf("Task Reconfiguration on PRR[%d] is completed ", ReconfigPRRID);
				return ReconfigPRRID;
		//	task completed TODO call a function that represent the complete of this task
			}



	return -1;
}
