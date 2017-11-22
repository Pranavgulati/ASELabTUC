#include "init.h"
//COPIED FROM CAN.C both should have the same copy but somehow main.h doesnt work

typedef struct{
    int standardId;
    unsigned double data; //4 bytes of data is supported 
    int dataLength;
}msgPacket;
typedef unsigned char byte;

/********************************************************************
*                          global variables                         *
********************************************************************/

msgPacket packet1,packet2,packet3;

/********************************************************************
*                    _____  ___  ___   ___                          *
*                   |_   _|/ _ \|   \ / _ \                         *
*                     | | | (_) | |) | (_) |                        *
*                     |_|  \___/|___/ \___/                         *
*                                                                   *
* If you need global variables define them here!                    *
********************************************************************/  

/********************************************************************
 *                              main                                *
 ********************************************************************/
void main(void)
{
    /* board initialization */
    Init();
    PIT_ConfigureTimer(0,100);
    PIT_ConfigureTimer(1,200);
    PIT_StartTimer(0);
    PIT_StartTimer(1);
    /********************************************************************
    *                    _____  ___  ___   ___                          *
    *                   |_   _|/ _ \|   \ / _ \                         *
    *                     | | | (_) | |) | (_) |                        *
    *                     |_|  \___/|___/ \___/                         *
    *                                                                   *
    * Assign the message ID's to the corresponding packets here         *
    ********************************************************************/  
    packet1.standardId=75; 
    packet2.standardId=2; 
    packet3.standardId=2; 

    /* turn off leds */
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    LED6 = 1;
    LED7 = 1; 

    /********************************************************************
    *                    _____  ___  ___   ___                          *
    *                   |_   _|/ _ \|   \ / _ \                         *
    *                     | | | (_) | |) | (_) |                        *
    *                     |_|  \___/|___/ \___/                         *
    *                                                                   *
    * Some configuration/initialisation must                            *
    * be done outside of the for-loop....(!)                            *
    ********************************************************************/  

    /* forever */
    for(;;)
    {
        /********************************************************************
        *                    _____  ___  ___   ___                          *
        *                   |_   _|/ _ \|   \ / _ \                         *
        *                     | | | (_) | |) | (_) |                        *
        *                     |_|  \___/|___/ \___/                         *
        *                                                                   *
        * Write down your logic here.                                       *
        ********************************************************************/  
    }
}

/********************************************************************
 *                      Interrupt Functions                         *
 ********************************************************************/  

void PITCHANNEL0(void)
{
    /* your own code above! */
    PIT.CH[0].TFLG.B.TIF = 1;
}

void PITCHANNEL1(void)
{
    /* your own code above! */
    PIT.CH[1].TFLG.B.TIF = 1;
}

void PITCHANNEL2(void)
{
    /* your own code above! */
    PIT.CH[2].TFLG.B.TIF = 1;
}

void CANMB0003(void)
{
/* No modifications needed here */
/* Receive interrupts are being cleared here */
    CAN_0.IFRL.B.BUF00I = 1;
    CAN_0.IFRL.B.BUF01I = 1;
    CAN_0.IFRL.B.BUF02I = 1;
    CAN_0.IFRL.B.BUF03I = 1;
}

void onMessageReceived(msgPacket inMsgPacket){
//use the inMsgPacket to do the intended function with the packet

}

void CANMB0407(void)
{
    /********************************************************************
    *                    _____  ___  ___   ___                          *
    *                   |_   _|/ _ \|   \ / _ \                         *
    *                     | | | (_) | |) | (_) |                        *
    *                     |_|  \___/|___/ \___/                         *
    *                                                                   *
    * CAN reception is handled here                                     *
    * The following buffers are important:                              *
    * CAN_0.RXFIFO.ID.B.STD_ID: ID of received message                  *
    * CAN_0.RXFIFO.DATA.B[i]: value of data byte 'i'                    *
    * IMPORTANT: check for the flag in CAN_0.IFRL.B.BUF05I first!       *
    ********************************************************************/  
   if(CAN_0.IFRL.B.BUF05I==1){
         LED5=~LED5;
        //read the fifo from this interrupt 
        msgPacket inputMsgPacket;
        inputMsgPacket.data=0;//init
        inputMsgPacket.standardId=CAN_0.RXFIFO.ID.B.STD_ID;
        inputMsgPacket.dataLength=CAN_0.RXFIFO.CS.B.LENGTH;
        for(int i=inputMsgPacket.dataLength; i>0;i--){
            //doesnt enter if dataLength is zero
            inputMsgPacket.data|=(CAN_0.BUF[messageBufferNo].DATA.B[i-1]);
            inputMsgPacket.data=inputMsgPacket.data<<8;
        }
        onMessageReceived(inputMsgPacket);
    }
    LED6=~LED6;
    /* end of own code! */
    /* clear flags as last step here! */
    /* don't change anything below! */
    CAN_0.IFRL.B.BUF04I = 1;
    CAN_0.IFRL.B.BUF05I = 1;
    CAN_0.IFRL.B.BUF06I = 1;
    CAN_0.IFRL.B.BUF07I = 1;
}

void CANMB0811(void)
{
/* No modifications needed here */
/* transmit interrupts are being cleared here */

    CAN_0.IFRL.B.BUF08I = 1;
    CAN_0.IFRL.B.BUF09I = 1;
    CAN_0.IFRL.B.BUF10I = 1;
    CAN_0.IFRL.B.BUF11I = 1;
}

void CANMB1215(void)
{
/* No modifications needed here */
/* transmit interrupts are being cleared here */
    CAN_0.IFRL.B.BUF12I = 1;
    CAN_0.IFRL.B.BUF13I = 1;
    CAN_0.IFRL.B.BUF14I = 1;
    CAN_0.IFRL.B.BUF15I = 1;
}

void ETIMER_00(){
    //interrupt freq = (F_CPU)/65536; since prescaler is 1
    if(ETIMER_0.CHANNEL[0].STS.B.TOF==1||ETIMER_0.CHANNEL[0].STS.B.TCF==1||ETIMER_0.CHANNEL[0].STS.B.TCF1==1||ETIMER_0.CHANNEL[0].STS.B.TCF2==1){
        //overflow to 0xFFFF has happened

    }
    //clear all timer interrupts
ETIMER_0.CHANNEL[0].STS.B.TOF=1;
ETIMER_0.CHANNEL[0].STS.B.TCF=1;
ETIMER_0.CHANNEL[0].STS.B.TCF2=1;
ETIMER_0.CHANNEL[0].STS.B.TCF1=1;
}

/********************************************************************
 *                   Interrupt Vector Table                         *
 ********************************************************************/
#pragma interrupt Ext_Isr
#pragma section IrqSect RX address=0x040
#pragma use_section IrqSect Ext_Isr

void Ext_Isr() {
    switch(INTC.IACKR.B.INTVEC)
    {
        case 59:
            PITCHANNEL0();
            break;
        case 60:
            PITCHANNEL1();
            break;
        case 61:
            PITCHANNEL2();
            break;
        case 157:
            ETIMER_00();//timer 0 channel 0 interrupt interrupt freq= (F_CPU)/0xFFFF
            break;
        case 68:
            CANMB0003();
            break;
        case 69:
            CANMB0407();
            break;
        case 70:
            CANMB0811();
            break;
        case 71:
            CANMB1215();
            break;        
        default:
            break;
    }
    /* End of Interrupt Request */
    INTC.EOIR.R = 0x00000000;
}
