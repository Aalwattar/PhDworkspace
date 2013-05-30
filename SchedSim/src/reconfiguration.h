/*
 * reconfiguration.h
 *
 *  Created on: 2013-05-23
 *      Author: aalwatta
 */

#ifndef RECONFIGURATION_H_
#define RECONFIGURATION_H_

#include "processors.h"

int IsReconfiguring(void);
int Reconfigure(struct Processor *processor, int PRRID, unsigned long time) ;
int TickConfiguration( struct Processor *processor);
int TickConfig(struct Processor *processor);
int ReconfignLoad(struct Processor *processor, int PRRID, unsigned long time, struct NodeData nd);


#endif /* RECONFIGURATION_H_ */
