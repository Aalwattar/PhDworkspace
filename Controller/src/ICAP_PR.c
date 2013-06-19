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

/* Parse Bitfile header */
XHwIcap_Bit_Header XHwIcap_ReadHeader(Xuint8 *Data, Xuint32 Size)
{
        Xuint32 I;
        Xuint32 Len;
        Xuint32 Tmp;
        XHwIcap_Bit_Header Header;
        Xuint32 Index;

        /* Start Index at start of bitstream */
        Index=0;

        /* Initialize HeaderLength.  If header returned early inidicates
         * failure.
         */
        Header.HeaderLength = XHI_BIT_HEADER_FAILURE;

        /* Get "Magic" length */
        Header.MagicLength = Data[Index++];
        Header.MagicLength = (Header.MagicLength << 8) | Data[Index++];

        /* Read in "magic" */
        for (I=0; I<Header.MagicLength-1; I++) {
                Tmp = Data[Index++];
                if (I%2==0 && Tmp != XHI_EVEN_MAGIC_BYTE) {
                        return Header;   /* INVALID_FILE_HEADER_ERROR */
                }
                if (I%2==1 && Tmp != XHI_ODD_MAGIC_BYTE) {
                        return Header;   /* INVALID_FILE_HEADER_ERROR */
                }
        }

        /* Read null end of magic data. */
        Tmp = Data[Index++];

        /* Read 0x01 (short) */
        Tmp = Data[Index++];
        Tmp = (Tmp << 8) | Data[Index++];

        /* Check the "0x01" half word */
        if (Tmp != 0x01) {
                return Header;   /* INVALID_FILE_HEADER_ERROR */
        }


        /* Read 'a' */
        Tmp = Data[Index++];
        if (Tmp != 'a') {
                return Header;    /* INVALID_FILE_HEADER_ERROR  */
        }

        /* Get Design Name length */
        Len = Data[Index++];
        Len = (Len << 8) | Data[Index++];

        /* allocate space for design name and final null character. */
        Header.DesignName = (Xuint8 *)malloc(Len);

        /* Read in Design Name */
        for (I=0; I<Len; I++) {
                Header.DesignName[I] = Data[Index++];
        }

        /* Read 'b' */
        Tmp = Data[Index++];
        if (Tmp != 'b') {
                return Header;  /* INVALID_FILE_HEADER_ERROR */
        }

        /* Get Part Name length */
        Len = Data[Index++];
        Len = (Len << 8) | Data[Index++];

        /* allocate space for part name and final null character. */
        Header.PartName = (Xuint8 *)malloc(Len);

        /* Read in part name */
        for (I=0; I<Len; I++) {
                Header.PartName[I] = Data[Index++];
        }

        /* Read 'c' */
        Tmp = Data[Index++];
        if (Tmp != 'c') {
                return Header;  /* INVALID_FILE_HEADER_ERROR */
        }

        /* Get date length */
        Len = Data[Index++];
        Len = (Len << 8) | Data[Index++];

        /* allocate space for date and final null character. */
        Header.Date = (Xuint8 *)malloc(Len);

        /* Read in date name */
        for (I=0; I<Len; I++) {
                Header.Date[I] = Data[Index++];
        }

        /* Read 'd' */
        Tmp = Data[Index++];
        if (Tmp != 'd') {
                return Header;  /* INVALID_FILE_HEADER_ERROR  */
        }

        /* Get time length */
        Len = Data[Index++];
        Len = (Len << 8) | Data[Index++];

        /* allocate space for time and final null character. */
        Header.Time = (Xuint8 *)malloc(Len);

        /* Read in time name */
        for (I=0; I<Len; I++) {
                Header.Time[I] = Data[Index++];
        }

        /* Read 'e' */
        Tmp = Data[Index++];
        if (Tmp != 'e') {
                return Header;  /* INVALID_FILE_HEADER_ERROR */
        }

        /* Get byte length of bitstream */
        Header.BitstreamLength = Data[Index++];
        Header.BitstreamLength = (Header.BitstreamLength << 8) | Data[Index++];
        Header.BitstreamLength = (Header.BitstreamLength << 8) | Data[Index++];
        Header.BitstreamLength = (Header.BitstreamLength << 8) | Data[Index++];

        Header.HeaderLength = Index;

        return Header;

}

/** Loads a partial bitstream from CF into ICAP */
int XHwIcap_CF2Icap(XHwIcap *hwicap, char* filename )
{
        int i, numCharsRead, ace_buf_count, rc, SectorNumber;
        SYSACE_FILE *stream;
        XHwIcap_Bit_Header bit_header;
        Xuint8 data3,data2,data1,data0;
        Xuint8 systemACE_Buffer[XSA_CF_SECTOR_SIZE];
        Xuint32 word[2];
        XStatus Status;

        ///Ahmed Al-Wattar

//      Xuint32 *FileP=(Xuint32 *)MATH0_SHIFT;
 //       int Count=0;
        //Ahmed
        //            xil_printf("Count = %d , FileP=%X",Count,FileP);
#if DEBUG
        print("In CF2ICAP\r\n");
        xil_printf("filename = %s\r\n",filename);
#endif

        /* Opening file */
        if ((stream = sysace_fopen(filename, "r")) == NULL) {
                xil_printf("Can't open file (%s)\r\n", filename);
                return -1;
        }

#if DEBUG
        print("In CF2ICAP ..File Opened..\r\n");
#endif

        /* Read from systemAce */
        numCharsRead = sysace_fread(systemACE_Buffer, 1, XSA_CF_SECTOR_SIZE,
                                    stream);
        if (numCharsRead <= 0) {
                xil_printf("Error reading from system ace (%d)\r\n", numCharsRead);
                return -1;
        }

#if DEBUG
        print("In CF2ICAP ..Header Sector Read..\r\n");
#endif

        /* Read the bitstream header */
        bit_header = XHwIcap_ReadHeader(systemACE_Buffer,0);

        /* close systemAce file handle */
        rc = sysace_fclose (stream);
        if (rc < 0) {
                /* Can't close */
                xil_printf("can't close file\r\n");
                return -1;
        }

#if DEBUG
        print("In CF2ICAP ..File Closed..\r\n");
#endif

        /* Now that we have info about the bitstream,
         * re-open and skip the header.
         */

        if ((stream = sysace_fopen(filename, "r")) == NULL) {
                /* Can't open file */
                xil_printf("can't open file\r\n");
                return -1;
        }

#if DEBUG
        print("In CF2ICAP ..File Opened..\r\n");
#endif

        /* Read the header (effectively skipping it) */
        numCharsRead = sysace_fread(systemACE_Buffer, 1, bit_header.HeaderLength,
                                    stream);
#if DEBUG
        xil_printf("Number of chars read = %d, Sector Number = %d\r\n",numCharsRead,SectorNumber);
        print("In CF2ICAP ..Skip Header..\r\n");
#endif

        numCharsRead = sysace_fread(systemACE_Buffer, 1, XSA_CF_SECTOR_SIZE,
                                    stream);

        SectorNumber = 1;

        /* Loop through all bitstream data and write to ICAP */
        ace_buf_count = 0;
        for (i=0; i<bit_header.BitstreamLength; i+=4) {

                /* Convert 4 chars into an integer */
                data3 = systemACE_Buffer[ace_buf_count++];
                data2 = systemACE_Buffer[ace_buf_count++];
                data1 = systemACE_Buffer[ace_buf_count++];
                data0 = systemACE_Buffer[ace_buf_count++];
                word[0] = ((data3 << 24) | (data2 << 16) | (data1 << 8) | (data0));
                i+=4;
                if(i<bit_header.BitstreamLength) { // store another word if even number of words
                        /* Convert 4 chars into an integer */
                        data3 = systemACE_Buffer[ace_buf_count++];
                        data2 = systemACE_Buffer[ace_buf_count++];
                        data1 = systemACE_Buffer[ace_buf_count++];
                        data0 = systemACE_Buffer[ace_buf_count++];
                        word[1] = ((data3 << 24) | (data2 << 16) | (data1 << 8) | (data0));
                } else {
                        word[1] = 0x0; // store 0- this is work around for hwicap bug in 11.4
                }


//                * (FileP)=word[0];FileP++;
//                * (FileP)=word[1];FileP++;
//                Count+=2;

                 Status = XHwIcap_DeviceWrite(hwicap, word, 2);

                if (Status != XST_SUCCESS) {
                        /* Error writing to ICAP */
                        xil_printf("error writing to ICAP (%d)\r\n", Status);
                        return -1;
                }
#if DEBUG
                xil_printf("In CF2ICAP ..Writing Word Number %d from current sector to ICAP..\r\n",ace_buf_count);
#endif

                /* Check to see if we need to read from CF again */
                if (ace_buf_count == XSA_CF_SECTOR_SIZE) {
#if DEBUG
                        print("In CF2ICAP ..Reading Next Sector..\r\n");
#endif
                        /* read next sector from CF */
                        numCharsRead = sysace_fread(systemACE_Buffer, 1, XSA_CF_SECTOR_SIZE,
                                                    stream);
                        ace_buf_count = 0;
#if DEBUG
                        xil_printf("Number of chars read = %d, sector number = %d\r\n",numCharsRead, SectorNumber);
#endif
                        SectorNumber++;
                }
        }

#if DEBUG
        print("In CF2ICAP ..All Words Written to ICAP..\r\n");
#endif
        /* close systemAce file handle */
        rc = sysace_fclose (stream);
        if (rc < 0) {
                /* Can't close */
                xil_printf("can't close file\r\n");
                return -1;
        }

        //Ahmed
     //          xil_printf("Count = %d , FileP=%X",Count,FileP);
        return 0;
}
////////////////////////////////////

int XHwIcap_CF2Memory( char* filename ,Xuint32 Addr)
{
        int i, numCharsRead, ace_buf_count, rc, SectorNumber;
        SYSACE_FILE *stream;
        XHwIcap_Bit_Header bit_header;
        Xuint8 data3,data2,data1,data0;
        Xuint8 systemACE_Buffer[XSA_CF_SECTOR_SIZE];
        Xuint32 *AddrP;
        Xuint32 word[2];
        AddrP=(Xuint32 *) Addr;
    //    XStatus Status;

        ///Ahmed Al-Wattar
 //       Xuint32 *FileP=(Xuint32 *)MATH0_SHIFT;
        int Count=0;
        //Ahmed
         //           xil_printf(" AddrP=%X , Addr=%X \r\n",AddrP,Addr);
#if DEBUG
        print("In CF2memory\r\n");
        xil_printf("filename = %s\r\n",filename);
#endif

        /* Opening file */
        if ((stream = sysace_fopen(filename, "r")) == NULL) {
                xil_printf("Can't open file (%s)\r\n", filename);
                return -1;
        }

#if DEBUG
        print("In CF2memory ..File Opened..\r\n");
#endif

        /* Read from systemAce */
        numCharsRead = sysace_fread(systemACE_Buffer, 1, XSA_CF_SECTOR_SIZE,
                                    stream);
        if (numCharsRead <= 0) {
                xil_printf("Error reading from system ace (%d)\r\n", numCharsRead);
                return -1;
        }

#if DEBUG
        print("In CF2memory ..Header Sector Read..\r\n");
#endif

        /* Read the bitstream header */
        bit_header = XHwIcap_ReadHeader(systemACE_Buffer,0);

        /* close systemAce file handle */
        rc = sysace_fclose (stream);
        if (rc < 0) {
                /* Can't close */
                xil_printf("can't close file\r\n");
                return -1;
        }

#if DEBUG
        print("In CF2memory ..File Closed..\r\n");
#endif

        /* Now that we have info about the bitstream,
         * re-open and skip the header.
         */

        if ((stream = sysace_fopen(filename, "r")) == NULL) {
                /* Can't open file */
                xil_printf("can't open file\r\n");
                return -1;
        }

#if DEBUG
        print("In CF2memory ..File Opened..\r\n");
#endif

        /* Read the header (effectively skipping it) */
        numCharsRead = sysace_fread(systemACE_Buffer, 1, bit_header.HeaderLength,
                                    stream);
#if DEBUG
        xil_printf("Number of chars read = %d, Sector Number = %d\r\n",numCharsRead,SectorNumber);
        print("In CF2memory ..Skip Header..\r\n");
#endif

        numCharsRead = sysace_fread(systemACE_Buffer, 1, XSA_CF_SECTOR_SIZE,
                                    stream);

        SectorNumber = 1;

        /* Loop through all bitstream data and write to ICAP */
        ace_buf_count = 0;
        for (i=0; i<bit_header.BitstreamLength; i+=4) {

                /* Convert 4 chars into an integer */
                data3 = systemACE_Buffer[ace_buf_count++];
                data2 = systemACE_Buffer[ace_buf_count++];
                data1 = systemACE_Buffer[ace_buf_count++];
                data0 = systemACE_Buffer[ace_buf_count++];
                word[0] = ((data3 << 24) | (data2 << 16) | (data1 << 8) | (data0));
                i+=4;
                if(i<bit_header.BitstreamLength) { // store another word if even number of words
                        /* Convert 4 chars into an integer */
                        data3 = systemACE_Buffer[ace_buf_count++];
                        data2 = systemACE_Buffer[ace_buf_count++];
                        data1 = systemACE_Buffer[ace_buf_count++];
                        data0 = systemACE_Buffer[ace_buf_count++];
                        word[1] = ((data3 << 24) | (data2 << 16) | (data1 << 8) | (data0));
                } else {
                        word[1] = 0x0; // store 0- this is work around for hwicap bug in 11.4
                }
/*
 * Copy to memeory
 */
                * (AddrP)=word[0];AddrP++;
                * (AddrP)=word[1];AddrP++;
                Count+=2;
                if ((Xuint32) AddrP > (Xuint32) BITSTREAMS_END_ADDRESS)
                {
                	xil_printf("ERROR: Out of bitsreams memory [%X] > [%X].... \r\n",(Xuint32) AddrP,(Xuint32)BITSTREAMS_END_ADDRESS);
                	return -1;
                }
                /*
                 * TODO add  a check for end of memory
                 */
//                 Status = XHwIcap_DeviceWrite(hwicap, word, 2);
//                if (Status != XST_SUCCESS) {
//                        /* Error writing to ICAP */
//                        xil_printf("error writing to ICAP (%d)\r\n", Status);
//                        return -1;
//                }
#if DEBUG
                xil_printf("In CF2memory ..Writing Word Number %d from current sector to ICAP..\r\n",ace_buf_count);
#endif

                /* Check to see if we need to read from CF again */
                if (ace_buf_count == XSA_CF_SECTOR_SIZE) {
#if DEBUG
                        print("In CF2memory ..Reading Next Sector..\r\n");
#endif
                        /* read next sector from CF */
                        numCharsRead = sysace_fread(systemACE_Buffer, 1, XSA_CF_SECTOR_SIZE,
                                                    stream);
                        ace_buf_count = 0;
#if DEBUG
                        xil_printf("Number of chars read = %d, sector number = %d\r\n",numCharsRead, SectorNumber);
#endif
                        SectorNumber++;
                }
        }

#if DEBUG
        print("In CF2memory ..All Words Written to ICAP..\r\n");
#endif
        /* close systemAce file handle */
        rc = sysace_fclose (stream);
        if (rc < 0) {
                /* Can't close */
                xil_printf("can't close file\r\n");
                return -1;
        }

        //Ahmed
     //          xil_printf("Count = %d , FileP=%X",Count,FileP);
        return Count;
}

////////////////////////////////////////////////////
///////////Perform the configuration//////////////////
/*
 * Copy bitstreams from CFlash to memory
 * Available_PRR and MAX_PR_MODULES desides number of prr and modules to copy to memory
 */
int Init_CF_Memory()
{
 int i,j;
 //struct PRRMOD  prr;
 u32 addr=BITSTREAMS_START_ADDRESS;
 int size=0;
 for (i=0; i< AVAILABLE_PRR;i++)
 {
	 	 for (j=0; j< MAX_PR_MODULES;j++)
	 {
			//xil_printf("Bitstream [%s] of PRR[%d] of Module[%d]  \r\n",(char *) PRRs[i].Files[j],i,j);

		 size=XHwIcap_CF2Memory( (char *) PRRs[i].Files[j] , addr);
		 if (size<=0)
		 {
			 xil_printf("ERROR: in 'Init_CF_Memroy' in moving bitstream %s return size zero\r\n",(char *) PRRs[i].Files[j]);

			 return -1;
		 }

		 PRRs[i].BitstreamsLib[j].Address=addr;
		 PRRs[i].BitstreamsLib[j].Size=(u32) size/2;
		 addr+=size*4+10;
#if 1
		 xil_printf("Bitstream [%s] of PRR[%d] moved  to [%X] size %d \r\n",(char *) PRRs[i].Files[j],i,PRRs[i].BitstreamsLib[j].Address,size*4);
#endif
	 }

 }
 return XST_SUCCESS;
}




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