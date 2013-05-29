/*
 * reconfiguration.h
 *
 *  Created on: 2013-05-23
 *      Author: aalwatta
 */

#ifndef RECONFIGURATION_H_
#define RECONFIGURATION_H_

int Reconfigure(struct Processor *processor, int PRRID, unsigned long time) ;
int TickConfiguration( struct Processor *processor);
int TickConfig(struct Processor *processor);



#endif /* RECONFIGURATION_H_ */
