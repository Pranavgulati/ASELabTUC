#include "init.h"

#define TIMER_CHANNEL_NO 0
#define TIMER_PERIOD 500
#define ENABLE 1
#define DISABLE 0

#define TASK3 0
#define TASK2 0 // change value for subtasks subtask1:1 subtask2:2
#define TASK4 1

void showData(int value);
void changeTimerState(int state);
void writeLedArrayByte();
void clearAll();
volatile char ledArrayByte=0;
volatile char addingValue = 1;


/********************************************************************
 *                              main                                *
 ********************************************************************/
void main(void)
{
    /* board initialization */
    Init();
    clearAll();
    changeTimerState(ENABLE);
    /* forever */
    for(;;)
    {
    #if TASK4 ==1
            
    #endif
    #if TASK2!=0
        #if TASK2==1 || TASK2==3
            //subtask1
            showData(ADC_0.CDR[2].B.CDATA);
        #elif TASK2==21
            //subtask2
            showData(ADC_0.CDR[4].B.CDATA);
        #endif
    #endif
    #if TASK3==1
        //subtask 1
        // using separate if...else makes each I/O independent of each other
        if(SIU.GPDI[0].R==0){
            //button 6 is pressed,turn ON led 6
            SIU.GPDO[59].R=0;
        } else {
            SIU.GPDO[59].R=1;
        }
        if(SIU.GPDI[1].R==0){
            //button 5 is pressed,turn ON led 5
            SIU.GPDO[61].R=0;
        }else{
            //turning off leds
            SIU.GPDO[61].R=1;
        }

        //subtask 2
        if(SIU.GPDI[44].R==0){
            //switch turned on
            changeTimerState(ENABLE);
        }else{
            //switch turned off
            changeTimerState(DISABLE);    
        }
    #endif
    }
}

void writeLedArrayByte(char ledArrayByte){
    if(ledArrayByte&&0x01){
        SIU.GPDO[9].R=0;
    }
    if(ledArrayByte&&0x02){
        SIU.GPDO[42].R=0;
    }
    if(ledArrayByte&&0x04){
        SIU.GPDO[13].R=0;
    }
    if(ledArrayByte&&0x08){
        SIU.GPDO[12].R=0;
    }
    if(ledArrayByte&&0x10){
        SIU.GPDO[62].R=0;
    }
    if(ledArrayByte&&0x20){
        SIU.GPDO[61].R=0;
    }
    if(ledArrayByte&&0x40){
        SIU.GPDO[59].R=0;
    }
    if(ledArrayByte&&0x80){
        SIU.GPDO[11].R=0;
    }
    
}
static void clearAll(){
SIU.GPDO[11].R=1;
SIU.GPDO[59].R=1;
SIU.GPDO[61].R=1;
SIU.GPDO[62].R=1;
SIU.GPDO[12].R=1;
SIU.GPDO[13].R=1;
SIU.GPDO[42].R=1;
SIU.GPDO[9].R=1;
}
void changeTimerState(int state){
    if(state==ENABLE){
        PIT_ConfigureTimer(TIMER_CHANNEL_NO,TIMER_PERIOD);
        PIT_StartTimer(TIMER_CHANNEL_NO);
    }else{
        PIT_StopTimer(TIMER_CHANNEL_NO);
    }
}
void showData(int value)
{
/********************************************************************
 *                    _____  ___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 ********************************************************************/  
 
#if TASK2==3
    value= 1024-value;
#endif
  if(value <= 204)
  {   
    /* LED0 pcr9 - on, LED1 pcr42 - off, LED2 pcr13- off, LED3 pcr12 - off */
    SIU.GPDO[9].R=0;
    SIU.GPDO[42].R=1;
    SIU.GPDO[13].R=1;
    SIU.GPDO[12].R=1;
    SIU.GPDO[62].R=1;
  }
  else if(value > 204 && value <= 408)
  {  
    /* LED0 - on, LED1 - on, LED2 - off, LED3 - off */
    SIU.GPDO[9].R=0;
    SIU.GPDO[42].R=0;
    SIU.GPDO[13].R=1;
    SIU.GPDO[12].R=1;
    SIU.GPDO[62].R=1;
  }
  else if(value > 408 && value <= 612)
  {  
    /* LED0 - on, LED1 - on, LED2 - on, LED3 - off */
    SIU.GPDO[9].R=0;
    SIU.GPDO[42].R=0;
    SIU.GPDO[13].R=0;
    SIU.GPDO[12].R=1;
    SIU.GPDO[62].R=1;
  }    
  else if(value > 612 && value <= 816)
  {  
    SIU.GPDO[9].R=0;
    SIU.GPDO[42].R=0;
    SIU.GPDO[13].R=0;
    SIU.GPDO[12].R=0;
    SIU.GPDO[62].R=1;
    /* LED0 - on, LED1 - on, LED2 - on, LED3 - on */
  }
  else
  {  
    SIU.GPDO[9].R=0;
    SIU.GPDO[42].R=0;
    SIU.GPDO[13].R=0;
    SIU.GPDO[12].R=0;
    SIU.GPDO[62].R=0;
    /* LED0 - on, LED1 - on, LED2 - on, LED3 - on */
  }
}

/********************************************************************
 *                      Interrupt Functions                         *
 *                    _____  ___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 *                Interrupts can be handled below.                  *
 ********************************************************************/  

void PITCHANNEL0() {
    /* clear flag */
    PIT.CH[0].TFLG.B.TIF = 1;
    clearAll(); //clearing the LED display    
    if(ledArrayByte==0xFF){addingValue=0;}
    if(ledArrayByte==0x00){addingValue=1;}
    ledArrayByte = ledArrayByte<<1;
    ledArrayByte = ledArrayByte+addingValue;
    writeLedArrayByte(ledArrayByte);
    
}

void PITCHANNEL1() {
    /* clear flag */
    PIT.CH[1].TFLG.B.TIF = 1;
}

/********************************************************************
 *                   Interrupt Vector Table                         *
 *                                                                  *
 *                 Don't touch anything below!                      *
 ********************************************************************/
#pragma interrupt Ext_Isr
#pragma section IrqSect RX address=0x040
#pragma use_section IrqSect Ext_Isr

void Ext_Isr() {
    switch(INTC.IACKR.B.INTVEC)
    {
        case 59:
            /* Timer Interrupt */
            PITCHANNEL0();
            break;
        case 60:
            /* Timer Interrupt */
            PITCHANNEL1();
            break;            
        default:
            break;
    }
    /* End of Interrupt Request */
    INTC.EOIR.R = 0x00000000;
}
