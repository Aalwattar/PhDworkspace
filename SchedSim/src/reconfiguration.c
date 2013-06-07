/*
 * reconfiguration.c
 *
 *  Created on: 2013-05-23
 *      Author: aalwatta
 */
#include <stdio.h>
#include <stdlib.h>
#include "processors.h"
#include "PlatformConfig.h"
#include "data.h"
static int Reconfiguring =NO;
static int ReconfigPRRID=0;
static struct NodeData ND;
static int LoadTask=NO;
static struct Processor *PE;
static unsigned int ConfigCount=0;




/*****************************************************************************
* This function retrieves the Configuration  Counter It's been used a way to
* hide the variable
*
* @param        None
*
* @return       u32 ConfigCount Value
*
* @note         None.
*
******************************************************************************/
unsigned int GetConfigCount(void)
{
        return ConfigCount;
}


/*****************************************************************************
* This function resets (=0) the ConfigCount Counter It's been used a way to
* hide the variable
*
* @param        None
*
* @return       none
*
* @note         None.
*
******************************************************************************/

void ResetConfigCount(void)
{
        ConfigCount =0;
}

/*****************************************************************************
* This function Increments the ConfigCount Counter It's been used a way to
* hide the variable
*
* @param        None
*
* @return       None
*
* @note         None.
*
******************************************************************************/
 void IncConfigCount(void)
{
        ConfigCount++;
}





int IsReconfiguring(void)
{
		return Reconfiguring;
}


int Reconfigure(struct Processor *processor, int PRRID, unsigned long time)
{

	if ( Reconfiguring )
	{
		fprintf(stderr,"ERROR [Reconfigure] .. Reconfiguration is in process (ONLY one reconfiguration at a time is allowed) \n");
		return -1;
	}
	if ( processor->Type!=TypeHW )
	{
		fprintf(stderr,"ERROR [Reconfigure] You cannot reconfigure a GPP (Are  you sure you know what you are doing !!!)  \n");
		return -2;
	}


	if ( processor->Busy )
	{
		fprintf(stderr,"WARNING [Reconfigure]  the PRR is currently Busy  \n");
		return -3;
	}




	ReconfigPRRID=PRRID;
	Reconfiguring = YES;

	processor->ConfigCount=time;
	processor->ExecCount= 0;
	processor->Busy=YES;
	IncConfigCount();
	return 0;


}

int ReconfignLoad(struct Processor *processor, int PRRID, unsigned long time, struct NodeData nd)
{
	Reconfigure(processor,PRRID,time);

	/*
	 * TODO  move all this mess to one static structure.
	 */
#if DEBUG_PRINT
	fprintf(stderr,"reconfiguring Task [%d] on PRR[%d]\n",nd.TaskID,PRRID);
#endif


	PE=processor;
	ND.ExecCount=nd.ExecCount;
	ND.Module=nd.Module;
	ND.TaskID=nd.TaskID;
	LoadTask=YES;

	return EXIT_SUCCESS;

}


int TickConfiguration( struct Processor *processor)
{

		if ( processor->Type !=TypeHW)
	{
		fprintf(stderr,"ERROR[TickReconfiguration] You cannot reconfigure a GPP (Are  you sure you know what  you are  doing !!!) %d  \n", processor->Type);
		return -2;
	}
	if (processor->Busy && !processor->ExecCount && processor->ConfigCount)
	{
		if(!(--processor->ConfigCount))
		{
#if DEBUG_PRINT
			fprintf(stderr, "finish reconfiguration \n");
#endif
		processor->Busy=NO;
		Reconfiguring=NO;

			if (LoadTask)
			{
				LoadTask=NO;
				setTaskSimConfTimeEnd(ND.TaskID, GetTimer());
				setTaskSimPrrUsed(ND.TaskID,processor->ID);
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
#if DEBUG_PRINT
				fprintf(stderr,"Task Reconfiguration on PRR[%d] is completed \n", ReconfigPRRID);
#endif
				return ReconfigPRRID;
		//	task completed TODO call a function that represent the complete of this task
			}



	return -1;
}


