#include "jdp.h"
typedef struct{
	int standardId;
    unsigned double data; //4 bytes of data is supported 
}txMsgPacket;
typedef unsigned char byte;

#define TASK3_3 0
#define ENABLE 1
#define DISABLE 0

void CANMsgBufInit(void)
{
/********************************************************************
 *               CAN message buffer configuration                   *
 *                    _____  ___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 ********************************************************************/
    /* MB Code */
    CAN_0.BUF[0].CS.B.CODE = 0x0C;
    /* Standard format */
    CAN_0.BUF[0].CS.B.IDE =0x00;
    /* SRR */
    //CAN_0.BUF[].CS.B.SRR = ;
    /* Data Frame */
    CAN_0.BUF[0].CS.B.RTR = 0x00;
    /* Data Length */
    CAN_0.BUF[0].CS.B.LENGTH =0x04 ; //sending 4 bytes
    /* STD_ID */
    //CAN_0.BUF[].ID.B.STD_ID = ;

/********************************************************************
 *                   AN masking configuration                       *
 *                    _____  ___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 ********************************************************************/
    #if TASK3_3==1
        //configure the filter masks for fifo
        CAN_0.RXFIFO.IDTABLE[0].R = 0x08F; //enter valid id 4 bytes
        CAN_0.RXIMR[0].R = 0x040FFFFF;
    #elif TASK3_3==0
        //allow all kind message to pass
        CAN_0.RXFIFO.IDTABLE[0].R = 0x047;
        CAN_0.RXIMR[0].R = 0x00000000;
    #endif

    
    
}

void CAN_Write(int messageBufferNo,txMsgPacket inputMsgPacket){
    /* Standard format */
    CAN_0.BUF[messageBufferNo].CS.B.IDE =0;
    /* SRR */
    //CAN_0.BUF[].CS.B.SRR = ;
    /* Data Frame */
    CAN_0.BUF[messageBufferNo].CS.B.RTR = 0;
    /* Data Length */
    CAN_0.BUF[messageBufferNo].CS.B.LENGTH =0x02 ; //sending 4 bytes
    /* STD_ID */
    CAN_0.BUF[messageBufferNo].ID.B.STD_ID = inputMsgPacket.standardId ;
    CAN_0.BUF[messageBufferNo].DATA.B[0] = inputMsgPacket.data & 0x00ff;
    CAN_0.BUF[messageBufferNo].DATA.B[1] = (inputMsgPacket.data & 0x0100)>>8;
    CAN_0.BUF[messageBufferNo].CS.B.CODE = 0x0C;

}

/********************************************************************
 *                          CAN driver                              *
 *                                                                  *
 *                   Don't touch anything below!                    *
 ********************************************************************/

void CANInit(void)
{
    unsigned int i;  

    /* clock source 16Mhz oszillator */
    CAN_0.CR.B.CLKSRC = 1;    
    
    /* module configuration register (MCR) */
    /* enable CAN */
    CAN_0.MCR.B.MDIS = 0;
    /* backwards compatibility enabled */
    CAN_0.MCR.B.BCC = 1;      
    /* warning interrupt enabled */
    CAN_0.MCR.B.WRNEN = 1;    
    /* self reception disabled */
    CAN_0.MCR.B.SRXDIS = 1; 
    /* FIFO enabled */
    CAN_0.MCR.B.FEN = 1;
    /* ID Acceptance Mode - One full message ID is stored in every ID table entry */
    CAN_0.MCR.B.IDAM = 0;
    /* abort mechanism enabled */
    CAN_0.MCR.B.AEN = 1; 
    /* local priority enabled */
    CAN_0.MCR.B.LPRIO_EN = 0;
    /* maximum count of message buffers +1 */
    CAN_0.MCR.B.MAXMB = 17;

    /* 500 kbit/s */
    /* propagation segment */
    CAN_0.CR.B.PROPSEG = 2;
    /* PSEG1 */
    CAN_0.CR.B.PSEG1 = 1;    
    /* PSEG2 */
    CAN_0.CR.B.PSEG2 = 1;
    /* resync jump width */
    CAN_0.CR.B.RJW = 0;
    /* prescaler */
    CAN_0.CR.B.PRESDIV = 3;
    /* Der MB mit der geringsten Nummer wird zuerst versendet */
    CAN_0.CR.B.LBUF = 1;
    /* loop back disabled */
    CAN_0.CR.B.LPB = 0;

    /* FIFO ID Table */
    for(i=0; i<8; i++)
    {
        CAN_0.RXFIFO.IDTABLE[i].R = 0;    
    }

    // Set Mask Register here
    // Global Mask Register for FIFO (except IDT 6 and 7) 
    // or for all MB (except 14 und 15)
    CAN_0.RXGMASK.R = 0x00000000;
    // Mask for FIFO ID Table 6 or MB 14
    CAN_0.RX14MASK.R = 0x00000000;
    // Mask for FIFO Id Table 7 or MB 15
    CAN_0.RX15MASK.R = 0x00000000;
    
    /* Bus Off Interrupt */
    CAN_0.CR.B.BOFFMSK = 1;    
    /* Error Mask Interrupt */
    CAN_0.CR.B.ERRMSK = 1;
    /* Transmit Warning Interrupt */
    CAN_0.CR.B.TWRNMSK = 1;
    /* Receive Warning Interrupt */
    CAN_0.CR.B.RWRNMSK = 1;
    /* enable Wake Up interrupt */
    CAN_0.MCR.B.WAKMSK = 1;

    /* enable all message buffer interrupts */
    CAN_0.IMRL.R = 0xFFFFFFFF;
    /* clear all message buffer interrupts */
    CAN_0.IFRL.R = 0xFFFFFFFF;

    /* reset individual mask registers */
    for(i=0; i<64; i++)
    {
        CAN_0.RXIMR[i].R = 0xFFFFFFFF;
    }

    CANMsgBufInit();

    /* leave freeze mode */
    CAN_0.MCR.B.HALT = 0;
}