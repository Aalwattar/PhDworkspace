/*
 * processors.c
 *
 *  Created on: 2013-05-14
 *      Author: aalwatta
*/

#include <stdio.h>
#include <stdlib.h>
#include "processors.h"
//#include "configuration.h"
/*
 * TODO change small Functions to inline
 */

struct Processor * InitProcessors( int size, enum ProcessorType type)
{
	 int i;
struct Processor *processors;
	processors=(struct Processor *)malloc(sizeof(struct Processor ) *size);

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
	{
		if(!(--processor->ExecCount))
		{
		processor->Busy=NO;
		}
		return processor->ExecCount;

	}

	 return -1;
}

int LoadProcessor( struct Processor *processor, struct NodeData node )
{
	if (processor->Busy)
	{
		printf("ERROR[LoadProcessor] Processor is busy\n");
		return 1;
	}
	processor->Busy=YES;
	processor->CurrentModule=node.Module; /*TODO fix this */
	processor->ExecCount=node.ExecCount;
	return 0;
}

int TickAllProcessors(struct Processor *processor, int size)
{
	int i;
	for (i =0 ; i< size ; i ++ )
	{
		if(!TickProcessor(processor))
			{
				printf("Task running on PRR[%d] is completed ", i);
		//	task completed TODO call a function that represent the complete of this task
			}
		processor++;
	}

	return 0;
}

