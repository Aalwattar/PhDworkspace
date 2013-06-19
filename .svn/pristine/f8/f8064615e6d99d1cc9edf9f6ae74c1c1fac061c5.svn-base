/*
 * ICAP_PR.H
 *
 *  Created on: Jun 19, 2011
 *      Author: ahmed
 */

#ifndef ICAP_PR_H_
#define ICAP_PR_H_

/* TODO move those to Data.c
 * update other projects as well
 */

/* PRModuels represent PR modules for that PRR
 * PRR Represent the Partial Reconfigurable Regions
 */

#define PRR_NUMBER 5 // # of PRR Regions
#define YES 1
#define NO 0
#define MAX_PR_MODULES 5


/* TODO get rid of all global variables */


enum PRModules {adder,sub,mult,shift,blank};
enum PRRID {Math0,Math1,Math2,Math3,Math4};

struct PRRHW {
        const u32 BaseAddr;
        const u32 IntID ;
};

struct PRRLIB
{
	u32 Address;
	u32 Size;

};
struct PRR {
        const char * name;
        const enum PRRID ID;
        enum PRModules Current;
        const char  *Files[MAX_PR_MODULES];
        const unsigned int ModulesNo;
        const u32 ModDelay[MAX_PR_MODULES];
        struct PRRLIB BitstreamsLib[MAX_PR_MODULES];
        u8 Busy;
        Xuint32 DFGID;
        struct PRRHW HW;
        Xuint32 CurrentTypeID;
        Xuint32 ConfigTime;
};

struct PRRMOD {
        enum PRRID PRR_ID;
        enum PRModules Module;
        Xuint32 TypeID;
        Xuint32 CanRun;

};
struct PRRIntData {
        Xuint32 Address;
        enum PRRID ID;
};




extern struct PRR PRRs[];
//extern char * PRFiles  [];
//extern enum PRModules CurrentMod;
//extern u32 PRMDelay[];

/***********************************************
 * 			Function Declerations
 ************************************************/
XHwIcap_Bit_Header XHwIcap_ReadHeader(Xuint8 *, Xuint32 );
int XHwIcap_CF2Icap(XHwIcap *, char* );
int Config(struct PRRMOD * prr, XHwIcap *hwicap,u32 delay);
void  HWMath(u32 op1, u32 op2, enum PRRID prr);
int Init_CF_Memory();
void IncConfigCount(void);
void ResetConfigCount(void);
u32 GetConfigCount(void);
void reInit_PRRs(int NoPRR);
#endif /* ICAP_PR_H_ */
