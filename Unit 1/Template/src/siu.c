#include "jdp.h"

void SIU_Init(void) {
  /***************************************************************************
   *                                  Task 1                                 *
   *    Please enter the PCR register numbers and initialitation code here   *
   ***************************************************************************/
  /* LEDs */
  /* SIU.PCR[].R = ; */   /* LED0 */
  /* SIU.PCR[].R = ; */   /* LED1 */
  /* ... */
    //PORT A led pins set to output
    SIU.PCR[9].R=0x0200;    
    SIU.PCR[11].R=0x0200;
    SIU.PCR[12].R=0x0200;
    SIU.PCR[13].R=0x0200;
    //PORT D led pins set to output
    SIU.PCR[59].R=0x0200;
    SIU.PCR[61].R=0x0200;
    SIU.PCR[62].R=0x0200;
    //PORT C led pins set to output
    SIU.PCR[42].R=0x0200;
   
  /***************************************************************************
   *                                  Task 2                                 *
   *                       Analog inputs configuration                       *
   ***************************************************************************/  
  /* SIU.PCR[].R = ; */
  /* ... */
    SIU.PCR[33].R=0x2500;
    SIU.PCR[65].R=0x2500;

  /***************************************************************************
   *                                  Task 3                                 *
   *                            Buttons & switches                           *
   ***************************************************************************/    
  /* SIU.PCR[].R = ; */   /* BT5 */
  /* ... */
      //PORT A bt5 bt6 buttons as input
    SIU.PCR[0].R=0x0100;
    SIU.PCR[1].R=0x0100;
    //PORT C SW1 switch as input
    SIU.PCR[44].R=0x0100;
}