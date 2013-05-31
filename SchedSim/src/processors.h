#ifndef PROCESSORS_H_
#define PROCESSORS_H_


#define YES 1
#define NO 0

enum ProcessorType {HW, SW};




struct Processor
{
	unsigned long ExecCount;
	unsigned long ConfigCount;
	int Busy;
	int CurrentTaskID;
	int CurrentModule;
	unsigned int ID;
	enum ProcessorType Type;

};



struct NodeData
{
	int Module;
	unsigned long ExecCount;
};


struct PE
{
	struct Processor * pe;
	int size;
};

struct PEs
{
	struct PE * HW;
	struct PE * SW;
};


struct Processor * InitProcessors( int , enum ProcessorType );
int IsProcessorBusy( struct Processor *);
void SetProcessorBusy( struct Processor *);
void SetProcessorNotBusy( struct Processor *);
int CheckCurrentModule( struct Processor *);
int TickProcessor( struct Processor *);
int LoadProcessor( struct Processor *, struct NodeData  );
int TickAllProcessors(struct Processor *, int );
void FreeProcessors(struct Processor * processor);





#endif /* PROCESSORS_H_ */
