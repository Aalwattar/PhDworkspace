/*
 * displayResults.c
 *
 *  Created on: Jun 3, 2013
 *      Author: aalwatta
 */
#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include"PlatformConfig.h"





void print_DFG(void)
{
        struct node * DFG=dfg1;
        int i=0;
        struct Simulation Sim;
        char tmpstr[255];
        unsigned int  totalConfT=0;

        do {
				getTaskSimulation(i,&Sim);
				sprintf(tmpstr,"%u",Sim.ConfigTime.start);
                fprintf (stdout,"Node [%3d] -->T[%5u] R[%5s]  %s Config %4d Exec %4d %s[%d] Reuse[%3s] Prio[%d] Type[%d] \n",
								DFG[i].id,
								Sim.ExecTime.start,Sim.PRRUsed<NO_OF_PRRS?tmpstr:"-",
								Sim.PRRUsed<NO_OF_PRRS?"RECONF": "SW COM",
								Sim.ConfigTime.end-Sim.ConfigTime.start,
								Sim.ExecTime.end-Sim.ExecTime.start,
								Sim.PRRUsed<NO_OF_PRRS?"PRR":"GPP",
								Sim.PRRUsed<NO_OF_PRRS?Sim.PRRUsed:Sim.PRRUsed-NO_OF_PRRS,
                        		Sim.Reused?"YES":"NO",
                        		getTaskTypeSWPrio(DFG[i].TypeID),
                        		DFG[i].TypeID	);

                totalConfT+=Sim.ConfigTime.end-Sim.ConfigTime.start;

        } while (DFG[i++].next);

              printf(" Total Conf Time [%4u] , \n",	totalConfT);

}
