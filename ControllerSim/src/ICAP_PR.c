/*
 * ICAP_PR.c
 *
 *  Created on: Jun 19, 2011
 *      Author: ahmed
 */

#include "headers.h"



/**************************************************
 *    Global variables
 *    TODO Move those variables to the Data.c
 *     Make sure to update o
 ***************************************************/
/*

struct PRRHW
{
	const u32 BaseAddr;
	const u32 IntID ;
};

struct PRR {
	const char * name;
	const enum PRRID ID;
	enum PRModules Current;
	const char  *Files[MAX_PR_MODULES];
	const unsigned int ModulesNo;
	const u32 ModDelay[MAX_PR_MODULES];
	u8 Busy;
	 struct PRRHW HW;
};



*/

/* ConfigCount counts the number of configuration */
static u32 ConfigCount;

struct PRR PRRs[PRR_NUMBER]= {
        /***** PRR Math0 ***************/
        {
                .name ="Math0",
                .ID=Math0,
                .Current= blank,
                .CurrentTypeID=0,
                .ModulesNo=5,
                .Busy = NO,
                .ConfigTime=0,
                .HW=
                {
                        .BaseAddr = MATH0_BASEADDRESS,
                        .IntID= MATH0_INT_ID
                },

                //.ModDelay={0x01000000,0x01ffffff,0x02ff0000,0x00ff0000,0x00000000},
                .ModDelay={20000000,20000000,70000000,20000000,0x00000000},
                .BitstreamsLib={{0,0},{0,0},{0,0},{0,0},{0,0}},
                //.ModAddr={MATH0_ADDER,MATH0_SUB,MATH0_MULT,MATH0_SHIFT,MATH0_ADDER},
                .Files={"M0A.bit","M0S.bit","M0M.bit","M0F.bit","M0B.bit"}

                },
                /***** PRR Math1 ***************/
        {
                .name ="Math1",
                .ID=Math1,
                .Current= blank,
                .CurrentTypeID=0,
                .ModulesNo=5,
                .Busy = NO,
                .ConfigTime=0,
                .HW=
                {
                        .BaseAddr = MATH1_BASEADDRESS,
                        .IntID= MATH1_INT_ID
                },
                //.ModDelay={0x01000000,0x01ffffff,0x02ff0000,0x00ff0000,0x00000000},
                .ModDelay={20000000,20000000,70000000,20000000,0x00000000},
                .BitstreamsLib={{0,0},{0,0},{0,0},{0,0},{0,0}},
                 .Files={"M1A.bit","M1S.bit","M1M.bit","M1F.bit","M1B.bit"}
                },

                /***** PRR Math2 ***************/
        {
                .name ="Math2",
                .ID=Math2,
                .Current= blank,
                .CurrentTypeID=0,
                .ModulesNo=5,
                .Busy = NO,
                .ConfigTime=0,
                .HW=
                {
                        .BaseAddr = MATH2_BASEADDRESS,
                        .IntID= MATH2_INT_ID
                },
                //.ModDelay={0x01000000,0x01ffffff,0x02ff0000,0x00ff0000,0x00000000},
                .ModDelay={20000000,20000000,70000000,20000000,0x00000000},
                 .Files={"M2A.bit","M2S.bit","M2M.bit","M2F.bit","M2B.bit"}
                },

                /***** PRR Math3 ***************/
        {
                .name ="Math3",
                .ID=Math3,
                .Current= blank,
                .CurrentTypeID=0,
                .ModulesNo=5,
                .Busy = NO,
                .ConfigTime=0,
                .HW=
                {
                        .BaseAddr = MATH3_BASEADDRESS,
                        .IntID= MATH3_INT_ID
                },
                //.ModDelay={0x01000000,0x01ffffff,0x02ff0000,0x00ff0000,0x00000000},
                .ModDelay={20000000,20000000,70000000,20000000,0x00000000},
                 .Files={"M3A.bit","M3S.bit","M3M.bit","M3F.bit","M3B.bit"}
                },

                /***** PRR Math4 ***************/
        {
                .name ="Math4",
                .ID=Math4,
                .Current= blank,
                .CurrentTypeID=0,
                .ModulesNo=5,
                .Busy = NO,
                .ConfigTime=0,
                .HW=
                {
                        .BaseAddr = MATH4_BASEADDRESS,
                        .IntID= MATH4_INT_ID
                },
                //.ModDelay={0x01000000,0x01ffffff,0x02ff0000,0x00ff0000,0x00000000},
                .ModDelay={20000000,20000000,70000000,20000000,0x00000000},
                 .Files={"M4A.bit","M4S.bit","M4M.bit","M4F.bit","M4B.bit"}
                }
        };


//enum PRModules CurrentMod = blank;
//char * PRFiles  []= {"M0Add.bit","M0Sub.bit","M0Mult.bit","M0ShftR.bit","M0Blnk.bit"};



void reInit_PRRs(int NoPRR)
{
	int i;
	for(i=0;i<NoPRR;i++)
	{
		PRRs[i].Busy=NO;
		PRRs[i].ConfigTime=0;
		PRRs[i].Current=blank;
		PRRs[i].CurrentTypeID=0;
	}
}

//u32 PRMDelay[]={0x10000000,0x1fffffff,0x2fff0000,0x0fff0000,0x00000000};

/**************************************************
 *   Functions started here
 ***************************************************/


////////////////////////////////////////////////////
///////////Perform the configuration//////////////////
/*
 * Copy bitstreams from CFlash to memory
 * Available_PRR and MAX_PR_MODULES desides number of prr and modules to copy to memory
 */



////////////////////////////////////////////////
int Config_memory(XHwIcap *hwicap ,struct PRRMOD * prr)
{
	int i;
	 //Xuint32 word[2];
	int Status;
	 Xuint32 *FileP=(Xuint32*)PRRs[prr->PRR_ID].BitstreamsLib[prr->Module].Address;
	for (i=0;i<PRRs[prr->PRR_ID].BitstreamsLib[prr->Module].Size;i++)
	{
//		word[0]=FileP[0];
//		word[1]=FileP[1];
//		FileP+=2;
		 Status = XHwIcap_DeviceWrite(hwicap, FileP, 2);
		 FileP+=2;
//         if (Status != XST_SUCCESS) {
//                 /* Error writing to ICAP */
//                 xil_printf("error writing to ICAP (%d)\r\n", Status);
//                 return -1;
//         }
	}
	return XST_SUCCESS;
}

int Config(struct PRRMOD * prr, XHwIcap *hwicap, u32 delay)
{
        u32 result;
        //enum PRModule MOD;
#if CONF_REUSE

        //if (PRRs[prr->PRR_ID].Current !=prr->Module) {
        if (PRRs[prr->PRR_ID].CurrentTypeID !=prr->TypeID) {
#endif
#if DEBUG_PRINT
                xil_printf(" Recon  %s PRR with %s  \n\r\n\r", PRRs[prr->PRR_ID].name , PRRs[prr->PRR_ID].Files[prr->Module]);
#endif
    			/*Increase Configuration counter ConfigCount*/
    			IncConfigCount();

    			//XHwIcap_CF2Icap(hwicap, (char *) PRRs[0].Files[adder]);

    			//print("before \n\r");
#if CONFIG_FROM_DDR
    			Config_memory(hwicap,prr );
#else

    			XHwIcap_CF2Icap(hwicap, (char *) PRRs[prr->PRR_ID].Files[prr->Module]);


#endif
    			//print("After \r\n");
    			//XHwIcap_CF2Icap(hwicap, (char *) PRRs[prr->PRR_ID].Files[prr->Module]);
                Xil_Out32(PRRs[prr->PRR_ID].HW.BaseAddr+0x00001000,0x0000000A);  // assert reset
               // Xil_Out32(PRRs[prr->PRR_ID].HW.BaseAddr+8,PRRs[prr->PRR_ID].ModDelay[prr->Module]);
                Xil_Out32(PRRs[prr->PRR_ID].HW.BaseAddr+8,delay);
                result=Xil_In32(PRRs[prr->PRR_ID].HW.BaseAddr+8);
                if (result!=delay) {
                        print("Error setting the counter Exiting...\r\n");
                        return XST_FAILURE;
               }

                PRRs[prr->PRR_ID].Current = prr->Module;
                PRRs[prr->PRR_ID].CurrentTypeID = prr->TypeID ;
#if CONF_REUSE
        } else
        {
#if DEBUG_PRINT
                xil_printf("\r\n    No config needed on the [%s] PRR using the same  [%s] Module \n\r\n\r",  PRRs[prr->PRR_ID].name , PRRs[prr->PRR_ID].Files[prr->Module]);
#endif
        }
#endif



        return XST_SUCCESS;
}

///////////////////////////////
////////////////////////////

void  HWMath(u32 op1, u32 op2, enum PRRID prr)
{

        Xil_Out32(PRRs[prr].HW.BaseAddr,op1);
        Xil_Out32(PRRs[prr].HW.BaseAddr+4,op2);
        //Xil_Out32(XPAR_MATH_0_BASEADDR+8,0x30000000);
        //result1=Xil_In32(XPAR_MATH_0_BASEADDR);
        //xil_printf("Result: %X\n\r",result1);

}


/*****************************************************************************
* This function retrieves the Configuration  Counter It's been used a way to
* hide the variable
*
* @param	None
*
* @return	u32 ConfigCount Value
*
* @note		None.
*
******************************************************************************/
u32 GetConfigCount(void)
{
        return ConfigCount;
}


/*****************************************************************************
* This function resets (=0) the ConfigCount Counter It's been used a way to
* hide the variable
*
* @param	None
*
* @return	none
*
* @note		None.
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
* @param	None
*
* @return	None
*
* @note		None.
*
******************************************************************************/
 void IncConfigCount(void)
{
	ConfigCount++;
}
//////////////////////////end of PR////////////////////
