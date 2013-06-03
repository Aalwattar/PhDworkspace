/*
 * processors.c
 *
 *  Created on: 2013-05-14
 *      Author: aalwatta
*/

#include <stdio.h>
#include <stdlib.h>
#include "processors.h"
#include "data.h"
#include "reconfiguration.h"

static unsigned int Timer;


/*
 * TODO change small Functions to inline
 */


/*****************************************************************************
* This function retrieves the Timer
*
* @param        None
*
* @return       u32 ConfigCount Value
*
* @note         None.
*
******************************************************************************/
unsigned int GetTimer(void)
{
        return Timer;
}


/*****************************************************************************
* This function resets (=0) the Timer Counter
*
* @param        None
*
* @return       none
*
* @note         None.
*
******************************************************************************/

void ResetTimer(void)
{
	Timer =0;
}

/*****************************************************************************
* This function Increments the Timer It's
*
* @param        None
*
* @return       None
*
* @note         None.
*
******************************************************************************/
 void IncTimer(void)
{
	 Timer++;
}




struct Processor * InitProcessors( int size, enum ProcessorType type)
{
	 int i;
struct Processor *processors;
	processors=(struct Processor *)malloc(sizeof(struct Processor ) *size);
  if (!processors) 
  {
    fprintf(stderr, "ERROR [InitProcessors] Allocating memory to processors \n");
    exit(EXIT_FAILURE); 
  }
	for (i=0; i<size; i++)
	{
		processors[i].Busy=NO;
		processors[i].CurrentModule=0;
		processors[i].ExecCount=0;
		processors[i].ConfigCount=0;
		processors[i].ID=0;
		processors[i].Type=type;
		processors[i].ID=i;

	}

	return processors;
}

 void FreeProcessors(struct Processor * processor)
{
	free(processor);
}

 int IsProcessorBusy( struct Processor *processor)
{
	return processor->Busy;
}

void SetProcessorBusy( struct Processor *processor)
{
	processor->Busy=YES;
}

void SetProcessorNotBusy( struct Processor *processor)
{
	processor->Busy=NO;
}

int CheckCurrentModule( struct Processor *processor)
{
	return processor->CurrentModule;
}


int TickProcessor( struct Processor *processor)
{

	if (processor->Busy && processor->ExecCount)
	{//fprintf(stderr,"exec Time for task [%d], is [%lu]\n", processor->ID, processor->ExecCount);

		if(!(--processor->ExecCount))
		{
		 //fprintf(stderr,"Task Done %d !!\n", processor->ExecCount);
		processor->Busy=NO;
		}
		return (int) processor->ExecCount;

	}

	 return -1;
}

int LoadProcessor( struct Processor *processor, struct NodeData node )
{
	if (processor->Busy)
	{
		fprintf(stderr,"ERROR[LoadProcessor] Processor is busy\n");
		return 1;
	}
	processor->Busy=YES;
	processor->CurrentModule=node.Module; /*TODO fix this */
	processor->ExecCount=node.ExecCount;
	processor->CurrentTaskID=node.TaskID;
#if DEBUG_PRINT
	fprintf(stderr,"loading task [%d] Type [%d] with ExecCount [%lu] \n", \
			processor->CurrentTaskID, processor->CurrentModule, processor->ExecCount);
#endif
	return 0;
}

int TickAllProcessors(struct Processor *processor, int size)
{
	int i;


	for (i =0 ; i< size ; i ++ )
	{

		if(!TickProcessor(processor+i))
			{
#if DEBUG_PRINT
				fprintf(stderr,"Task [%d] running on PRR[%d] is completed \n", processor[i].CurrentTaskID,i);
#endif
				State=TaskDone;
				taskDone(processor[i].CurrentTaskID);
				decTaskCounter();
		//	task completed TODO call a function that represent the complete of this task
			}

	}

	return 0;
}


unsigned int Ticker(struct PEs *pEs)
{
	TickAllProcessors(pEs->HW->pe, pEs->HW->size);
	TickAllProcessors(pEs->SW->pe, pEs->SW->size);
	TickConfig(pEs->HW->pe);
	IncTimer();
	return GetTimer();
}

void CreateAllPEs(struct PEs *pEs,int noOfPRRs, int noOfGPPs)
{
	pEs->HW=(struct PE*) malloc(sizeof(struct PE));
	pEs->SW=(struct PE*) malloc(sizeof(struct PE));
	pEs->HW->size=noOfPRRs;
	pEs->SW->size =noOfGPPs;
	pEs->HW->pe=InitProcessors(pEs->HW->size,HW );
	pEs->SW->pe=InitProcessors(pEs->SW->size, SW);

}

void CleanAllPEs(struct PEs *pEs)
{
	FreeProcessors(pEs->HW->pe);
	FreeProcessors(pEs->SW->pe);
    free(pEs->HW);
	free(pEs->SW);

}

