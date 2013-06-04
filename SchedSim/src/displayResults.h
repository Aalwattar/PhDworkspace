/*
 * displayResults.h
 *
 *  Created on: Jun 3, 2013
 *      Author: aalwatta
 */

#ifndef DISPLAYRESULTS_H_
#define DISPLAYRESULTS_H_
#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include"PlatformConfig.h"




void print_DFG(void)
{
        struct node * DFG=dfg1;
        int i=0;
        struct Simulation Sim;
        unsigned int  totalConfT=0;

        do {
				getTaskSimulation(i,&Sim);

                fprintf (stdout,"Node [%4d] -->  %s Config %4u Exec %4u PRR[%d] Reuse[%3s] Prio[%d] Type[%d] \r\n",
								DFG[i].id,
								Sim.PRRUsed!=NO_OF_PRRS?"RECONF": "SW COM",
								Sim.ConfigTime.end-Sim.ConfigTime.start,
								Sim.ExecTime.end-Sim.ExecTime.start,
                        		Sim.PRRUsed,
                        		Sim.Reused?"YES":"NO",
                        		TasksTypes[DFG[i].TypeID].SWPriority,
                        		DFG[i].TypeID	);

                totalConfT+=Sim.ConfigTime.end-Sim.ConfigTime.start;

        } while (DFG[i++].next);

              printf(" Total Conf Time [%4u] , \n",	totalConfT);

}


#endif /* DISPLAYRESULTS_H_ */
